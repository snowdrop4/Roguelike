#include "Texture.hpp"

#include <SDL/SDL.h>
#include "Window.hpp"
#include "load_image.hpp"
#include "apply_surface.hpp"

Texture::Texture() :
	texture(nullptr),
	texturePath()
{
	
}

Texture::Texture(const std::string &path, bool alpha) :
	texture(nullptr),
	texturePath()
{
	if (alpha) load_alpha(path);
	else load(path);
}

Texture::Texture(const std::string &path, const SDL_Color &col) :
	texture(nullptr),
	texturePath()
{
	load_color_key(path, col);
}

Texture::Texture(const std::string &path, Uint8 r, Uint8 g, Uint8 b) :
	texture(nullptr),
	texturePath()
{
	load_color_key(path, { r, g, b });
}

Texture::Texture(Texture &&other) :
	texture(other.texture),
	texturePath(std::move(other.texturePath))
{
	other.texture = nullptr;
}

Texture &Texture::operator = (Texture &&other)
{
	texture = other.texture;
	other.texture = nullptr;
	texturePath = std::move(other.texturePath);
	return *this;
}

Texture::~Texture()
{
	SDL_FreeSurface(texture);
}

std::string Texture::get_path() const
{ return texturePath; }

int Texture::get_w() const
{ return texture->w; }

int Texture::get_h() const
{ return texture->h; }

void Texture::draw(const Window &window, int x, int y) const
{
	apply_surface(x, y, window.get_camera_x(), window.get_camera_y(),
		texture, window.get_screen());
}

void Texture::draw_literal(const Window &window, int x, int y) const
{
	apply_surface_literal(x, y, texture, window.get_screen());
}

void Texture::load(const std::string &path)
{
	if (texture != nullptr) SDL_FreeSurface(texture);
	
	texturePath = path;
	texture = load_scaled_image(path);
}

void Texture::load_alpha(const std::string &path)
{
	if (texture != nullptr) SDL_FreeSurface(texture);
	
	texturePath = path;
	texture = load_scaled_image_alpha(path);
}

void Texture::load_color_key(const std::string &path, const SDL_Color &Color)
{
	if (texture != nullptr) SDL_FreeSurface(texture);
	
	texturePath = path;
	texture = load_scaled_image_color_key(path, Color);
}

void Texture::load_color_key(const std::string &path, Uint8 r, Uint8 g, Uint8 b)
{
	load_color_key(path, { r, g, b });
}

void Texture::set_alpha(Uint8 alpha)
{
	SDL_SetAlpha(texture, SDL_SRCALPHA, alpha);
}
