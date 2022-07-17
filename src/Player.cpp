#include "Player.hpp"

#include <SDL/SDL_events.h>
#include "Cursor.hpp"
#include "Window.hpp"
#include "Map.hpp"
#include "MapTile.hpp"

// -----------------------------------------------------------------------------
// Static
// -----------------------------------------------------------------------------
static void display_battle_result(GTKConsole &gameLogWindow, Character &defender,
	const Character &attacker, bool sideWithDefender, int damage)
{
	if (damage > 0)
	{
		gameLogWindow << (sideWithDefender == true ? messageColors::bad : messageColors::good)
		              << messagePrefixes::combat << attacker.get_stats().name 
		              << " attacked " << defender.get_stats().name << " for "
		              << std::to_string(damage) << " damage.";
		
		if (defender.is_alive() == false)
			gameLogWindow << " they died.";
		
		gameLogWindow << "\n";
	}
	else gameLogWindow << (sideWithDefender == true ? messageColors::welcome : messageColors::unwelcome)
	                   << messagePrefixes::combat << attacker.get_stats().name 
	                   << " missed " << defender.get_stats().name + ".\n";
}

static int display_melee_battle(GTKConsole &gameLogWindow, Character &defender,
	const Character &attacker, bool sideWithDefender)
{
	int damage = defender.Character::be_melee_attacked(attacker);
	display_battle_result(gameLogWindow, defender, attacker, sideWithDefender, damage);
	return damage;
}

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Player::Player(const TileCoord &position, const std::string &filePath,
               const Map &map, Window &window) :
	Character(position, filePath, map),
	gameLogWindow(),
	charWindow(*this),
	window(window),
	autoTravelPath()
{
	stats->initiative = -1;
	update_camera();
}

Player::~Player() { }

bool Player::is_player() const
{ return true; }

GTKConsole &Player::get_game_log()
{ return gameLogWindow; }

bool Player::move(const std::unique_ptr<SDL_Event> &event)
{
	/// true  : the user made a move
	/// false : the user didn't make a move
	auto handle_keyboard = [&]() -> bool
	{
		switch (event->key.keysym.sym)
		{
			/** 
			 * y k u
			 *  \|/ 
			 * h-+-l
			 *  /|\ 
			 * b j n
			 * 
			 * spacebar to wait
			**/
			
			case SDLK_y: return try_move({ -1, -1 });
			case SDLK_k: return try_move({ +0, -1 });
			case SDLK_u: return try_move({ +1, -1 });
			
			case SDLK_h: return try_move({ -1, +0 });
			case SDLK_l: return try_move({ +1, +0 });
			
			case SDLK_b: return try_move({ -1, +1 });
			case SDLK_j: return try_move({ +0, +1 });
			case SDLK_n: return try_move({ +1, +1 });
			
			case SDLK_SPACE: return true;
			
			default: return false;
		}
	};
	
	/// true  : the user chose an auto travel destination
	/// false : the user didn't choose an auto travel destination
	auto handle_mouse = [&]() -> bool
	{
		if (event->button.button == SDL_BUTTON_RIGHT)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			new_auto_travel_dest(mouse_xy_to_tile_coord(x, y, window));
			auto_travel();
			return true;
		}
		return false;
	};
	
	update_camera();
	
	if (event != nullptr)
	{
		switch (event->type)
		{
			case SDL_KEYDOWN:       if (handle_keyboard() == true) return true;
			case SDL_MOUSEBUTTONUP: if (handle_mouse()    == true) return true;
			
			default: break;
		}
	}
	// Make it so that, whilst auto traveling, the user can't create loads of
	// dummy events to increase the auto travel rate.
	// 
	// See Roguelike.cpp::run().
	else if (auto_travel() == true)
		return true;
	
	return false;
}

int Player::be_melee_attacked(const Character &attacker)
{
	int ret = display_melee_battle(gameLogWindow, *this, attacker, true);
	charWindow.update();
	return ret;
}

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------
bool Player::try_move(const TileCoordDiff &diff)
{
	TileCoord newPos = position + diff;
	
	if (map.at(newPos).is_solid() == true)
		return false;
	
	if (map.at(newPos).character_is_present() == true)
	{
		display_melee_battle(gameLogWindow, *map.at(newPos).get_character(), *this, false);
		return true;
	}
	
	move_to_tile(newPos);
	return true;
}

void Player::move_to_tile(const TileCoord &newPos)
{
	Character::move_to_tile(newPos);
	update_camera();
}

bool Player::new_auto_travel_dest(const TileCoord &dest)
{
	autoTravelPath = std::move(map.get_path(position, dest, &Character::astar_valid_tile));
	
	if (autoTravelPath->size() == 0)
	{
		gameLogWindow << messageColors::vBad << messagePrefixes::warning
		              << "That destination is unreachable.\n";
		return false;
	}
	
	// remove the first position (the tile the player is currently on)
	autoTravelPath->pop_front();
	
	return true;
}

bool Player::auto_travel()
{
	if (autoTravelPath == nullptr || autoTravelPath->size() == 0)
		return false;
	
	TileCoord newPos = autoTravelPath->front();
	
	if (map.at(newPos).is_walkable() == false ||
	    map.get_path(position, autoTravelPath->back(), &Character::astar_valid_tile)->empty() == true)
	{
		gameLogWindow << messageColors::vBad << messagePrefixes::warning
		              << "Your destination is no longer reachable.\n";
		autoTravelPath.reset();
		return false;
	}
	
	autoTravelPath->pop_front();
	move_to_tile(newPos);
	
	return true;
}

void Player::handle_death()
{
	gameLogWindow << messageColors::vBad << messagePrefixes::player
	              << "Congratulations, you died!\n";
	Character::handle_death();
}

void Player::update_camera() const
{
	// half of camera.w/h normalised to a (TILE_WIDTH, TILE_HEIGHT) grid
	int camW = window.get_w() / 2 - (window.get_w() / 2) % TILE_WIDTH;
	int camH = window.get_h() / 2 - (window.get_h() / 2) % TILE_WIDTH;
	
	int camX = position.x * TILE_WIDTH  - camW;
	int camY = position.y * TILE_HEIGHT - camH;
	
	if (camX < 0) camX = 0;
	if (camY < 0) camY = 0;
	
	if      (window.get_w() > map.get_pixel_w())        camX = map.get_pixel_w() / 2 - camW;
	else if (camX > map.get_pixel_w() - window.get_w()) camX = map.get_pixel_w() - window.get_w();
	
	if      (window.get_h() > map.get_pixel_h())        camY = map.get_pixel_h() / 2 - camH;
	else if (camY > map.get_pixel_h() - window.get_h()) camY = map.get_pixel_h() - window.get_h();
	
	window.set_camera_x(camX);
	window.set_camera_y(camY);
}
