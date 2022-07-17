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

#include <SDL/SDL.h>

void apply_surface(int x, int y, int xOffset, int yOffset, SDL_Surface *source,
	SDL_Surface *destination, SDL_Rect *clip)
{
	static SDL_Rect offset;
	
	offset.x = x - xOffset;
	offset.y = y - yOffset;
	
	SDL_BlitSurface(source, clip, destination, &offset);
}

void apply_surface_literal(int x, int y, SDL_Surface *source,
	SDL_Surface *destination, SDL_Rect *clip)
{
	static SDL_Rect offset;
	
	offset.x = x;
	offset.y = y;
	
	SDL_BlitSurface(source, clip, destination, &offset);
}
