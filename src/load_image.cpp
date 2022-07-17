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

#include "load_image.hpp"

#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <stdexcept>
#include "constants.hpp"
#include "utildefines.hpp"

SDL_Surface *load_scaled_image(const std::string &filename)
{
	SDL_Surface *loadedImage    = nullptr;
	SDL_Surface *optimisedImage = nullptr;
	SDL_Surface *scaledImage    = nullptr;
	
	loadedImage = IMG_Load(filename.c_str());
	
	if (loadedImage != nullptr)
	{
		optimisedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
		
		scaledImage = zoomSurface(optimisedImage, GRAPHICS_SCALE, GRAPHICS_SCALE, false);
		SDL_FreeSurface(optimisedImage);
		
		return scaledImage;
	}
	else throw std::runtime_error(LINEANDFILEB + "'" + filename +
		"' cannot be loaded. Does it exist? Is it corrupted?");
}

SDL_Surface *load_scaled_image_alpha(const std::string &filename)
{
	SDL_Surface *loadedImage    = nullptr;
	SDL_Surface *optimisedImage = nullptr;
	SDL_Surface *scaledImage    = nullptr;
	
	loadedImage = IMG_Load(filename.c_str());
	
	if (loadedImage != nullptr)
	{
		optimisedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);
		
		scaledImage = zoomSurface(optimisedImage, GRAPHICS_SCALE, GRAPHICS_SCALE, false);
		SDL_FreeSurface(optimisedImage);
		
		return scaledImage;
	}
	else throw std::runtime_error(LINEANDFILEB + "'" + filename +
		"' cannot be loaded. Does it exist? Is it corrupted?");
}

SDL_Surface *load_scaled_image_color_key(const std::string &filename,
	const SDL_Colour &colour)
{
    SDL_Surface *loadedImage    = nullptr;
    SDL_Surface *optimisedImage = nullptr;
    SDL_Surface *scaledImage    = nullptr;

    loadedImage = IMG_Load(filename.c_str());

    if (loadedImage != nullptr)
    {
		optimisedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
		
		scaledImage = zoomSurface(optimisedImage, GRAPHICS_SCALE, GRAPHICS_SCALE, false);
		Uint32 colorkey = SDL_MapRGB(scaledImage->format, colour.r, colour.g, colour.b);
		SDL_SetColorKey(scaledImage, SDL_SRCCOLORKEY, colorkey);
		
		SDL_FreeSurface(optimisedImage);
		
		return scaledImage;
    }
	else throw std::runtime_error(LINEANDFILEB + "'" + filename +
		"' cannot be loaded. Does it exist? Is it corrupted?");
}
