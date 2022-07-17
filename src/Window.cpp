#include "Window.hpp"

#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_events.h>
#include <algorithm>

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
Window::Window(int w, int h, bool fullscreen, const std::string &name) :
	fullscreen(fullscreen),
	screen(),
	cameraX(),
	cameraY(),
	windowedW(w),
	windowedH(h)
{
	reset_window(w, h);
	set_title(name);
}

Window::~Window()
{ SDL_FreeSurface(screen); }

int Window::get_w() const { return screen->w; }
int Window::get_h() const { return screen->h; }
int Window::get_camera_x() const { return cameraX; }
int Window::get_camera_y() const { return cameraY; }
		
SDL_Surface *Window::get_screen() const
{ return screen; }

void Window::set_title(const std::string &s)
{ SDL_WM_SetCaption(s.c_str(), nullptr); }

void Window::set_camera_x(int x) { cameraX = x; }
void Window::set_camera_y(int y) { cameraY = y; }

void Window::clear(const SDL_Color &c)
{ clear(c.r, c.g, c.b); }

void Window::clear(Uint8 r, Uint8 g, Uint8 b)
{ SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, r, g, b)); }

void Window::refresh() const
{ SDL_Flip(screen); }

void Window::update(const std::unique_ptr<SDL_Event> &event)
{
	if (event == nullptr) return;
	
	if (event->type == SDL_VIDEORESIZE && fullscreen == false)
	{		
		windowedW = event->resize.w - event->resize.w % TILE_WIDTH;
		windowedH = event->resize.h - event->resize.h % TILE_HEIGHT;
		
		reset_window(windowedW, windowedH);
	}
	else if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_F11)
		{
			fullscreen = !fullscreen;
			
			if (fullscreen == false)
				reset_window(windowedW, windowedH);
			else
			{
				// find the largest resolution supported
				auto modes = SDL_ListModes(NULL, SDL_FULLSCREEN);
				std::vector<std::pair<int, int>> sortedModes;
				
				for (int i = 0; modes[i]; ++i)
					sortedModes.emplace_back(modes[i]->w, modes[i]->h);
					
				std::sort(sortedModes.begin(), sortedModes.end());
				
				reset_window(sortedModes.back().first, sortedModes.back().second);
			}
		}
		else if (event->key.keysym.sym == SDLK_TAB && SDL_GetModState() == KMOD_LALT)
			SDL_WM_IconifyWindow();
	}
}

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------
void Window::reset_window(int w_A, int h_A)
{
	screen = SDL_SetVideoMode(w_A, h_A, 32,
		(fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE) | SDL_HWSURFACE |
		 SDL_ASYNCBLIT);
}
