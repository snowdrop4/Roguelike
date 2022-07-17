#pragma once

#include <memory>
#include <thread>
#include "UtilDefines.hpp"
#include "Cursor.hpp"

class GenericTileset;
class Map;
class Player;
class Window;
union SDL_Event;

enum class GameState
{
	menu,
	game,
	quit
};

class Game
{
	public:
		Game(Window &window, const std::unique_ptr<SDL_Event> &event);
		COPY_CTOR(Game) = delete;
		MOVE_CTOR(Game) = default;
		COPY_ASSIGN(Game) = delete;
		MOVE_ASSIGN(Game) = default;
		~Game();
		
		GameState update(const std::unique_ptr<SDL_Event> &event);
		void draw(const Window &window) const;
		
	private:
		std::unique_ptr<Map> map;
		
		std::unique_ptr<std::thread> GTKThread;
		
		Cursor cursor;
};
