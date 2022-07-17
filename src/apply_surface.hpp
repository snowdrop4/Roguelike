#pragma once

struct SDL_Surface;
struct SDL_Rect;

/// Takes into account the position of the camera
void apply_surface(int x, int y, int xOffset, int yOffset, SDL_Surface *source,
                   SDL_Surface *destination, SDL_Rect *clip = nullptr);

/// Doesn't take into account the position of the camera
void apply_surface_literal(int x, int y, SDL_Surface *source,
                           SDL_Surface *destination,
                           SDL_Rect *clip = nullptr);
