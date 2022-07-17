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
