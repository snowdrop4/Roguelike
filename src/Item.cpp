#include "Item.hpp"

#include "constants.hpp"
#include "ConfigFile.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Item::Item(const std::string &filePath) :
	texture(),
	statModifier(),
	filePath(filePath)
{
	ConfigFile file(filePath);
	
	// -------------------------------------------------------------------------
	// info
	// -------------------------------------------------------------------------
	std::string name        = file.parse<std::string>("name");
	std::string description = file.parse<std::string>("description");
	std::string useWord     = file.parse<std::string>("useWord");
	
	// -------------------------------------------------------------------------
	// texture
	// -------------------------------------------------------------------------
	// randomly select texture from list of possible textures:
	auto textureNames = file.parse_array<std::string>("texture");
	
	if (textureNames.size() == 0)
		throw std::runtime_error(LINEANDFILEB + "no textures set in file '" + filePath + "'");
	
	auto textureName = textureNames[rand() % textureNames.size()] + ".png";
	
	texture.load_color_key(move_up_dir(filePath) + textureName, Colors::textureColorKey);
	
	// -------------------------------------------------------------------------
	// stats
	// -------------------------------------------------------------------------
	statModifier.reset(new Stats::Item(
	      name,
	      description,
	      useWord,
		{ file.parse<int>("healthCur"), file.parse<int>("healthMax") },
		{ file.parse<int>("manaCur"),   file.parse<int>("manaMax")   },
		{ file.parse<int>("weightCur"), file.parse<int>("weightMax") },
		  file.parse<int>("strength"),
		  file.parse<int>("intelligence"),
		  file.parse<int>("dexterity"),
		  file.parse<int>("constitution"),
		  file.parse<int>("initiative"),
		  file.parse<int>("uses"),
		  file.parse<int>("effectLength")));
}

Item::~Item() { }

void Item::draw(const Window &window, int x, int y) const
{ texture.draw(window, x, y); }

void Item::draw_literal(const Window &window, int x, int y) const
{ texture.draw_literal(window, x, y); }

const Stats::Item &Item::get_stats() const
{ return *statModifier; }
