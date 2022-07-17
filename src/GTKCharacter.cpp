#include "GTKCharacter.hpp"

#include "Character.hpp"
#include "MapTile.hpp"
#include "StringUtils.hpp"
#include "Item.hpp"

// -----------------------------------------------------------------------------
// Misc
// -----------------------------------------------------------------------------
namespace Inventory
{
	const int labelColumn = 0;
	const int itemColumn  = 1;
	const int numColumns  = 2;
}

static bool disable_window_close(GtkWidget *w, GdkEvent *e, gpointer d)
{ return true; }

// -----------------------------------------------------------------------------
// Context Menu
// -----------------------------------------------------------------------------
class ContextMenuUserData
{
	public:
		ContextMenuUserData(GtkListStore *itemList, GtkTreeIter *itemIter,
			Character *charr) :
		itemList(itemList), itemIter(itemIter), charr(charr) { }
		COPY_CTOR(ContextMenuUserData) = delete;
		MOVE_CTOR(ContextMenuUserData) = default;
		COPY_ASSIGN(ContextMenuUserData) = delete;
		MOVE_ASSIGN(ContextMenuUserData) = default;
		~ContextMenuUserData() { delete itemIter; }
		
		GtkListStore *itemList;
		GtkTreeIter  *itemIter;
		Character    *charr;
};

// Removes the item right-clicked on from the inventory and places it at the 
// feet of the character. 
static void context_menu_drop_item(GtkWidget *w, gpointer userData)
{
	auto contextMenuUserData = (ContextMenuUserData *) userData;
	
	Item *item = nullptr;
	gtk_tree_model_get(GTK_TREE_MODEL(contextMenuUserData->itemList),
	                   contextMenuUserData->itemIter,
	                   Inventory::itemColumn, &item, -1);
	
	gtk_list_store_remove(contextMenuUserData->itemList, contextMenuUserData->itemIter);
	
	auto uniquePtrItem = std::unique_ptr<Item>(item);
	contextMenuUserData->charr->get_map_tile().add_item(uniquePtrItem);
	
	delete contextMenuUserData;
}

static void context_menu_create(GtkWidget *w, GdkEventButton *event, gpointer userData)
{
	GtkWidget *menu = gtk_menu_new();
	
	GtkWidget *equipMenuItem = gtk_menu_item_new_with_label("Equip");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), equipMenuItem);
	
	GtkWidget *dropMenuItem = gtk_menu_item_new_with_label("Drop");
	g_signal_connect(dropMenuItem, "activate", G_CALLBACK(context_menu_drop_item), userData);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), dropMenuItem);
	
	gtk_widget_show_all(menu);
	
	gtk_menu_popup(GTK_MENU(menu), nullptr, nullptr, nullptr, nullptr,
				 (event != nullptr) ? event->button : 0,
				 gdk_event_get_time((GdkEvent*)event));
}

// callback for mouse button events
static bool context_menu_handle(GtkWidget *w, GdkEventButton *event, gpointer userData)
{
	// if RMB is pressed
	if (event->type == GDK_BUTTON_PRESS && event->button == 3)
	{
		GtkTreePath  *path  = gtk_icon_view_get_path_at_pos(GTK_ICON_VIEW(w), event->x, event->y);
		GtkTreeModel *model = gtk_icon_view_get_model(GTK_ICON_VIEW(w));
		
		// if not freed immediately, contextMenuUserData [below] (when it's deleted) will take the honor
		GtkTreeIter *iter  = new GtkTreeIter;
		
		if (path == nullptr || model == nullptr || gtk_tree_model_get_iter(model, iter, path) == false)
		{
			delete iter;
			return false;
		}
		
		// if the cursor is over an item
		if (path != nullptr)
		{
			gtk_icon_view_unselect_all(GTK_ICON_VIEW(w));
			gtk_icon_view_select_path(GTK_ICON_VIEW(w), path);
			
			// Create the struct that will be used by context_menu_drop_item 
			// to drop the item. This struct will be free by that function.
			auto *contextMenuUserData = new ContextMenuUserData(
				GTK_LIST_STORE(model), iter, (Character *)userData);
			
			context_menu_create(w, event, contextMenuUserData);
		
			return true;
		}
	}
	
	return false;
}

// -----------------------------------------------------------------------------
// Tooltips
// -----------------------------------------------------------------------------
static bool item_tooltip_handle(GtkWidget *w, gint x, gint y, bool keyboardMode,
	GtkTooltip *tooltip, gpointer userData)
{
	// Convert mouse (x, y) value to GtkTreePath, then convert GtkTreePath to
	// GtkTreeIter.
	// 
	// If the path is empty, or the conversion from path to iter fails,
	// don't display a tooltip.
	GtkTreePath  *path  = gtk_icon_view_get_path_at_pos(GTK_ICON_VIEW(w), x, y);
	GtkTreeModel *model = gtk_icon_view_get_model(GTK_ICON_VIEW(w));
	GtkTreeIter iter;
	if (path == nullptr || model == nullptr || gtk_tree_model_get_iter(model, &iter, path) == false)
			return false;
	
	// Use the GtkTreeIter to retrieve the pointer to the Item stored in, 
	// and referenced by the icon (cell) the user is hovering over.
	Item *item = nullptr;
	gtk_tree_model_get(model, &iter, Inventory::itemColumn, &item, -1);
	
	gtk_tooltip_set_text(tooltip, item->get_stats().to_string().c_str());
	
	return true;
}

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
GTKCharacter::GTKCharacter(Character &charr) :
	y(0),
	charr(charr),
	window(),
	grid(),
	name(), faction(), level(),
	hp(), mp(), wt(), xp(),
	strength(), intelligence(), dexterity(), constitution(), initiative(),
	inventoryView(), inventoryScrolled(), inventoryItems()
{
	// -------------------------------------------------------------------------
	
	gdk_threads_enter();
	
	// create window, set padding, title etc
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 200);
	gtk_window_set_title(GTK_WINDOW(window), "Character");
	
	// disable window closing
	g_signal_connect(window, "delete-event", G_CALLBACK(disable_window_close), nullptr);
	
	// add a grid layout, set appropriate padding
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 25);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), true);
	
	// details -----------------------------------------------------------------
	
	name    = new_label_pair("Name:");
	faction = new_label_pair("Faction:");
	level   = new_label_pair("Level:");
	
	new_seperator(); // dynamic stats ------------------------------------------
	
	hp = new_labeled_styled_level_bar(   "css/red-level-bar.css");
	mp = new_labeled_styled_level_bar(  "css/blue-level-bar.css");
	wt = new_labeled_styled_level_bar( "css/green-level-bar.css");
	xp = new_labeled_styled_level_bar("css/yellow-level-bar.css");
	
	new_seperator(); // stats --------------------------------------------------
	
	strength     = new_label_pair("Strength:");
	intelligence = new_label_pair("Intelligence:");
	dexterity    = new_label_pair("Dexterity:");
	constitution = new_label_pair("Constitution:");
	initiative   = new_label_pair("Initiative:");
	
	new_seperator(); // inventory ----------------------------------------------
	
	// create the thing
	inventoryItems = gtk_list_store_new(Inventory::numColumns, G_TYPE_STRING, G_TYPE_POINTER, -1);
	inventoryView  = gtk_icon_view_new_with_model(GTK_TREE_MODEL(inventoryItems));
	gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(inventoryView), GTK_SELECTION_MULTIPLE);
	
	// create the columns
	gtk_icon_view_set_text_column(GTK_ICON_VIEW(inventoryView), Inventory::labelColumn);
	
	// enable scrolling
	inventoryScrolled = gtk_scrolled_window_new(nullptr, nullptr);
	
	// make it take up all available vertical space
	gtk_widget_set_vexpand(inventoryScrolled, true);
	
	// add it to our window
	gtk_grid_attach(GTK_GRID(grid), inventoryScrolled, 0, y++, 2, 2);
	gtk_container_add(GTK_CONTAINER(inventoryScrolled), inventoryView);
	
	// activate context menu
	g_signal_connect(inventoryView, "button-press-event", G_CALLBACK(context_menu_handle), &charr);
	
	// activate tooltips
	g_object_set(inventoryView, "has-tooltip", true, nullptr);
	g_signal_connect(inventoryView, "query-tooltip", G_CALLBACK(item_tooltip_handle), nullptr);
	
	// -------------------------------------------------------------------------
	
	update_private();
	
	gtk_widget_show_all(window);
	gdk_threads_leave();
}

GTKCharacter::~GTKCharacter()
{
	gdk_threads_enter();
	gtk_widget_destroy(window);
	gdk_threads_leave();
}

void GTKCharacter::update()
{
	gdk_threads_enter();
	update_private();
	gdk_threads_leave();
}

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------
void GTKCharacter::update_private()
{
	// details
	gtk_label_set_text(GTK_LABEL(name.val),    charr.get_stats().name.c_str());
	gtk_label_set_text(GTK_LABEL(faction.val), charr.get_stats().faction.c_str());
	gtk_label_set_text(GTK_LABEL(level.val),   int_to_pad_str(charr.get_stats().level, 3).c_str());
	
	// dyanamic stats	
	gtk_level_bar_set_value    (GTK_LEVEL_BAR(hp.bar), charr.get_stats().hp.cur);
	gtk_level_bar_set_max_value(GTK_LEVEL_BAR(hp.bar), charr.get_stats().hp.max);
	gtk_label_set_text(GTK_LABEL(hp.lbl), ("hp: " + charr.get_stats().hp.to_string()).c_str());
	
	gtk_level_bar_set_value    (GTK_LEVEL_BAR(mp.bar), charr.get_stats().mp.cur);
	gtk_level_bar_set_max_value(GTK_LEVEL_BAR(mp.bar), charr.get_stats().mp.max);
	gtk_label_set_text(GTK_LABEL(mp.lbl), ("mp: " + charr.get_stats().mp.to_string()).c_str());
	
	gtk_level_bar_set_value    (GTK_LEVEL_BAR(wt.bar), charr.get_stats().wt.cur);
	gtk_level_bar_set_max_value(GTK_LEVEL_BAR(wt.bar), charr.get_stats().wt.max);
	gtk_label_set_text(GTK_LABEL(wt.lbl), ("wt: " + charr.get_stats().wt.to_string()).c_str());
	
	gtk_level_bar_set_value    (GTK_LEVEL_BAR(xp.bar), charr.get_stats().xp);
	gtk_level_bar_set_max_value(GTK_LEVEL_BAR(xp.bar), charr.get_stats().level * 100);
	gtk_label_set_text(GTK_LABEL(xp.lbl), ("xp: " + std::to_string(charr.get_stats().xp) + "/" + std::to_string(charr.get_stats().level * 100)).c_str());
	
	// stats
	gtk_label_set_text(GTK_LABEL(strength.val),     int_to_pad_str(charr.get_stats().strength,     3).c_str());
	gtk_label_set_text(GTK_LABEL(intelligence.val), int_to_pad_str(charr.get_stats().intelligence, 3).c_str());
	gtk_label_set_text(GTK_LABEL(dexterity.val),    int_to_pad_str(charr.get_stats().dexterity,    3).c_str());
	gtk_label_set_text(GTK_LABEL(constitution.val), int_to_pad_str(charr.get_stats().constitution, 3).c_str());
	gtk_label_set_text(GTK_LABEL(initiative.val),   int_to_pad_str(charr.get_stats().initiative,   3).c_str());
	
	// inventory
	GtkTreeIter iter;
	const std::list<std::unique_ptr<Item>> &inventory = charr.get_inventory();
	gtk_list_store_clear(inventoryItems);
	for (auto &i : inventory)
	{
		gtk_list_store_append(inventoryItems, &iter);
		gtk_list_store_set(inventoryItems, &iter,
			Inventory::labelColumn, i->get_stats().name.c_str(),
			Inventory::itemColumn, i.get(),
			-1);
	}
}

lblPair GTKCharacter::new_label_pair(const std::string &str)
{
	lblPair ret { gtk_label_new(str.c_str()), gtk_label_new(nullptr) };
	
	gtk_misc_set_alignment(GTK_MISC(ret.lbl), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(ret.val), 1.0f, 0.5f);
	
	gtk_grid_attach(GTK_GRID(grid), ret.lbl, 0, y,   1, 1);
	gtk_grid_attach(GTK_GRID(grid), ret.val, 1, y++, 1, 1);
	
	return ret;
}

lblBarPair GTKCharacter::new_labeled_styled_level_bar(const std::string &cssPath)
{
	lblBarPair ret { gtk_label_new(nullptr), gtk_level_bar_new() };
	
	GtkStyleContext *styleContext  = gtk_widget_get_style_context(ret.bar);
	GtkCssProvider  *styleProvider = gtk_css_provider_new();
	
	auto css = g_file_new_for_path(cssPath.c_str());
	gtk_css_provider_load_from_file(styleProvider, css, nullptr);
	gtk_style_context_add_provider(styleContext, GTK_STYLE_PROVIDER(styleProvider), 99999);
		
	gtk_grid_attach(GTK_GRID(grid), ret.lbl, 0, y,   2, 1);
	gtk_grid_attach(GTK_GRID(grid), ret.bar, 0, y++, 2, 1);
	
	return ret;
}

void GTKCharacter::new_seperator()
{
	GtkWidget *seperator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_margin_top   (seperator, 10);
	gtk_widget_set_margin_bottom(seperator, 10);
	gtk_grid_attach(GTK_GRID(grid), seperator, 0, y++, 2, 1);
}
