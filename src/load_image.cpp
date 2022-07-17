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
