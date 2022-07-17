/*
 * Copyright 2012 Alyxr 
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with This program.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

/*
 * Todo:
 * 
 * Work on map population. could, on rand(), do one of the following:
 *     - do nothing
 *     - add a random creature or item
 *     - see if a random preset creature/item layout will fit at the current
 *         location, if so place it there, if not do nothing
 * 
 * 
 * Sort out the Stats mess.
 * 
 */

/*
 * Bugs:
 * 
 * If the map is smaller than the camera.w/h there's graphical ghosting.
 * 
 * 
 * If the window is resized so it covers some buttons, then resized so the
 * buttons are uncovered, the button backgrounds disappear and the buttons
 * can't be clicked.
 * 
 * The GTK windows don't disappear if the character dies.
 * 
 */

#include <SDL/SDL.h>
#include <gtk/gtk.h>
#include <stdexcept>
#include <iostream>

#include "Roguelike.hpp"

int main(int argc, char *argv[])
{
	srand(time(nullptr));
	
	gtk_parse_args(&argc, &argv);
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		throw std::runtime_error(LINEANDFILEB + "could not load SDL (SDL_INIT_EVERYTHING)");
	
	{
		Roguelike r;
		r.run();
	}
	
	SDL_Quit();
	
	return 0;
}
