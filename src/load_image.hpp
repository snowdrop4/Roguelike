#pragma once

#include <string>

struct SDL_Color;
struct SDL_Surface;

SDL_Surface *load_scaled_image          (const std::string &filename);
SDL_Surface *load_scaled_image_alpha    (const std::string &filename);
SDL_Surface *load_scaled_image_color_key(const std::string &filename, const SDL_Color &);
