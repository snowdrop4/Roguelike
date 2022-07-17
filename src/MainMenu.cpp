#include "MainMenu.hpp"

#include <SDL/SDL_gfxPrimitives.h>
#include "Roguelike.hpp"
#include "Button.hpp"

const int MainMenu::MARGIN = 2 * TILE_WIDTH;

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
MainMenu::MainMenu(const Window &window) :
	bg(),
	title(),
	buttons(),
	ret()
{
	title.reset(new Text("Roguelike", Colors::white));
	
	int y = MARGIN + 40;
	
	buttons.emplace_back("New Game",  MARGIN, y, [&](){ btn_new();     });
	int step = buttons[0].get_h() * 1.5;                                   y += step;
	buttons.emplace_back("Load Game", MARGIN, y, [&](){ btn_load();    }); y += step;
	buttons.emplace_back("Options",   MARGIN, y, [&](){ btn_options(); }); y += step;
	buttons.emplace_back("Quit",      MARGIN, y, [&](){ btn_quit();    });
	
	reset_background(window);
}

MainMenu::~MainMenu() { }

MenuState MainMenu::update(const std::unique_ptr<SDL_Event> &event)
{
	ret = MenuState::menu;
	
	if (event != nullptr && (event->type == SDL_MOUSEMOTION ||
	                         event->type == SDL_MOUSEBUTTONUP))
		for (auto &i : buttons)
			i.handle_input(*event);
	
	return ret;
}

void MainMenu::draw(const Window &window)
{
	draw_background(window);
	title->draw_literal(window, MARGIN, MARGIN);
	
	for (auto &i : buttons)
		i.draw_literal(window);
}

// -----------------------------------------------------------------------------
// private
// -----------------------------------------------------------------------------
void MainMenu::draw_background(const Window &window)
{
	static int oldW = window.get_screen()->w;
	static int oldH = window.get_screen()->h;
	
	if (window.get_screen()->w != oldW ||
	    window.get_screen()->h != oldH)
	{
		oldW = window.get_screen()->w;
		oldH = window.get_screen()->h;
		reset_background(window);
	}
	
	int x = 0;
	int y = 0;
	
	for (auto &i : bg)
	{
		for (auto &j : i)
		{
			boxRGBA(window.get_screen(), x, y, x + TILE_WIDTH, y + TILE_WIDTH, j, j, j, 255);
			
			x += TILE_WIDTH;
		}
		x = 0;
		y += TILE_HEIGHT;
	}
}

void MainMenu::reset_background(const Window &window)
{
	bg.clear();
	bg.resize(window.get_screen()->h / TILE_HEIGHT);
	
	for (auto &i : bg)
	for (int g = 0; g < window.get_screen()->w / TILE_WIDTH; ++g)
		i.push_back(rand() % 40);
	
	for (size_t i = 0; i < bg.size(); ++i)
	for (size_t j = 0; j < bg[i].size(); ++j)
	{
		int avg = 0;
		for (int q = -1; q < 2; ++q)
		for (int w = -1; w < 2; ++w)
		{
			if (i + q >= 0 && i + q < bg.size() &&
				j + w >= 0 && j + w < bg[i + q].size())
			avg += bg[i + q][j + w];
		}
		
		avg /= 9;
		bg[i][j] = avg;
	}
}

void MainMenu::btn_new()     { ret = MenuState::game; }
void MainMenu::btn_load()    { }
void MainMenu::btn_options() { }
void MainMenu::btn_quit()    { ret = MenuState::quit; }
