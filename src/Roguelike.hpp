#pragma once

#include <memory>
#include "Window.hpp"
#include "UtilDefines.hpp"

class MainMenu;
class Game;

enum class RoguelikeState
{
	menu,
	game,
	quit
};

class Roguelike
{
	public:
		Roguelike();
		COPY_CTOR(Roguelike) = delete;
		MOVE_CTOR(Roguelike) = default;
		COPY_ASSIGN(Roguelike) = delete;
		MOVE_ASSIGN(Roguelike) = default;
		~Roguelike();
		
		void run();
		
	private:
		void draw() const;
		void update();
		
		void menu_initialise();
		void menu_unload();
		
		void game_initialise();
		void game_unload();
		
		Window window;
		
		RoguelikeState roguelikeState;
		
		std::unique_ptr<SDL_Event> event;
		std::unique_ptr<MainMenu>  menu;
		std::unique_ptr<Game>      game;
};
