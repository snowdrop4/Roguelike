#include "Roguelike.hpp"

#include <SDL/SDL_events.h>
#include "MainMenu.hpp"
#include "Game.hpp"
#include "UpdateDrawLoop.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Roguelike::Roguelike() :
	window(25 * TILE_WIDTH, 15 * TILE_HEIGHT, false, "Roguelike"),
	roguelikeState(),
	event(),
	menu(),
	game()
{
	menu_initialise();
}

Roguelike::~Roguelike() { }

void Roguelike::run()
{
	// The game updates at least 60 times per second, but more if there are
	// input events.
	// 
	// If 'event' is not null, it's an update triggered by input, if it's null
	// it's one of the guaranteed updates.
	run_loop(60,
		[&](int fps) -> bool
		{
			// update
			SDL_Event ev;
			do
			{
				if (SDL_PollEvent(&ev))
					event.reset(new SDL_Event(ev));
				else event.reset();
				update();
			} while (event != nullptr);
			
			static int oldFPS = 0;
			if (oldFPS != fps)
			{
				oldFPS = fps;
				window.set_title("Roguelike - " + std::to_string(fps) + "fps");
			}
			
			return roguelikeState == RoguelikeState::quit;
		},
		// draw
		[&](double interp) -> void
		{
			draw();
			return;
		});
}

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------
void Roguelike::draw() const
{
	switch (roguelikeState)
	{
		case RoguelikeState::quit: { return; }
		case RoguelikeState::menu: { menu->draw(window); break; }
		case RoguelikeState::game: { game->draw(window); break; }
	}
	
	window.refresh();
}

void Roguelike::update()
{
	window.update(event);
	
	if (event != nullptr)
	{
		if ((event->type           == SDL_KEYDOWN &&
		     event->key.keysym.sym == SDLK_F4     &&
		     SDL_GetModState()     == KMOD_LALT)  ||
		     event->type == SDL_QUIT)
			roguelikeState = RoguelikeState::quit;
	}
	
	switch (roguelikeState)
	{
		case RoguelikeState::menu:
		{
			switch (menu->update(event))
			{
				case MenuState::menu: { break; }
				case MenuState::game:
				{
					menu_unload();
					game_initialise();
					roguelikeState = RoguelikeState::game;
					break;
				}
				case MenuState::quit:
				{
					roguelikeState = RoguelikeState::quit;
					break;
				}
			}
			break;
		}
		
		case RoguelikeState::game:
		{
			switch (game->update(event))
			{
				case GameState::menu:
				{
					game_unload();
					menu_initialise();
					roguelikeState = RoguelikeState::menu;
					break;
				}
				case GameState::game: { break; }
				case GameState::quit:
				{
					roguelikeState = RoguelikeState::quit;
					break;
				}
			}
		}
		
		case RoguelikeState::quit: { return; }
	}
}

void Roguelike::menu_initialise()
{
	menu.reset(new MainMenu(window));
	roguelikeState = RoguelikeState::menu;
}

void Roguelike::menu_unload()
{
	menu.reset();
}

void Roguelike::game_initialise()
{
	game.reset(new Game(window, event));
	roguelikeState = RoguelikeState::game;
}

void Roguelike::game_unload()
{
	game.reset();
}
