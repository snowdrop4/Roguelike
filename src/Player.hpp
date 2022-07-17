#pragma once

///
/// Player class. Takes, and responds to a user's input in the get_move() method
/// inherited from Character.
///

#include <deque>
#include "Character.hpp"
#include "GTKConsole.hpp"
#include "GTKCharacter.hpp"

union SDL_Event;
class Cursor;
class Window;

class Player : public Character
{
	public:
		Player(const TileCoord &position, const std::string &filePath,
		       const Map &map, Window &window);
		COPY_CTOR(Player) = delete;
		MOVE_CTOR(Player) = delete;
		COPY_ASSIGN(Player) = delete;
		MOVE_ASSIGN(Player) = delete;
		~Player();
		
		virtual bool is_player() const;
		GTKConsole &get_game_log();
		
		/// return : if the character moved
		bool move(const std::unique_ptr<SDL_Event> &event) override;
		
		/// Forwards arguments to Character::be_melee_attacked then
		/// prints out the result of the battle to gameEvents.
		int be_melee_attacked(const Character &by) override;
		
	private:
		/// true  : The (x, y) specified by Character::position + TileCoordDiff
		///         is a valid tile to perform an action on (eg. move to if it's
		///         an empty tile or attack if there's a Character on it). 
		///         Performs the appropriate action on the tile.
		/// 
		/// false : The tile was invalid and no action is taken.
		bool try_move(const TileCoordDiff &diff);
		
		/// updates the camera then calls 'Character::move_to_tile()'
		void move_to_tile(const TileCoord &newPos) override;
		
		/// true  : The destination chosen is valid, the first move towards
		///         the destination is made.
		/// 
		/// false : The destination is invalid, the user is notified. No other
		///         action is taken.
		bool new_auto_travel_dest(const TileCoord &dest);
		
		/// true  : The player has made an auto travel move and they have not
		///         yet reached their destination.
		/// 
		/// false : The player is not auto travelling or the destination has
		///         been reached/is now unreachable.
		bool auto_travel();
		
		void handle_death() override;
		
		void update_camera() const;
		
		GTKConsole   gameLogWindow;
		GTKCharacter charWindow;
		Window       &window;
		
		std::unique_ptr<std::list<std::pair<int, int>>> autoTravelPath;
};
