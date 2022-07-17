#pragma once

#include <memory>
#include <vector>
#include "UtilDefines.hpp"

class Window;
class Button;
class Text;
union SDL_Event;

enum class MenuState
{
	menu,
	game,
	quit
};

class MainMenu
{
	public:
		MainMenu(const Window &window);
		COPY_CTOR(MainMenu) = delete;
		MOVE_CTOR(MainMenu) = default;
		COPY_ASSIGN(MainMenu) = delete;
		MOVE_ASSIGN(MainMenu) = default;
		~MainMenu();
		
		MenuState update(const std::unique_ptr<SDL_Event> &event);
		void draw(const Window &window);
		
	private:
		void draw_background (const Window &window);
		void reset_background(const Window &window);
		
		// callbacks for the buttons
		void btn_new();
		void btn_load();
		void btn_options();
		void btn_quit();
		
		void construct();
		void destruct();
		
		static const int MARGIN;
		
		std::vector<std::vector<int>> bg;
		std::unique_ptr<Text> title;
		std::vector<Button> buttons;
		
		MenuState ret;
};
