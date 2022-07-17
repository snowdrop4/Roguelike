#pragma once

#include <memory>
#include "UtilDefines.hpp"
#include "constants.hpp"

/// This object must be destroyed before 'SDL_Quit()' is called in 'main()'.
/// 
/// There can only be one instance of this class.
/// 
/// This class must be instantiated before the function 'SDL_DisplayFormat()' 
/// can be used (this is used in image loading code) [that's probably not the
/// only dependant function, so just instantiate this first thing, okay?].

union SDL_Event;

class Window
{
	public:
		/// If a resize event occurs, pass the (w,h) to the callback function
		/// and use the (w,h) returned as the size to resize to (useful for
		/// placing upper or lower limits or enforcing other rules on the window
		/// size).
		Window(int w, int h, bool fullscreen, const std::string &name);
		COPY_CTOR(Window) = delete;
		MOVE_CTOR(Window) = delete;
		COPY_ASSIGN(Window) = delete;
		MOVE_ASSIGN(Window) = delete;
		~Window();
		
		int get_w() const;
		int get_h() const;
		
		int get_camera_x() const;
		int get_camera_y() const;
		
		SDL_Surface *get_screen() const;
		
		/// Processes all pending 'SDL_BlitSurface()'s.
		/// 
		/// Call after drawing something to the screen (ends the frame).
		void refresh() const;
		
		void set_title(const std::string &title);
		
		void set_camera_x(int x);
		void set_camera_y(int y);
		
		void clear(const SDL_Color &c = Colors::black);
		void clear(Uint8 r, Uint8 g, Uint8 b);
		
		void update(const std::unique_ptr<SDL_Event> &event);
		
	private:
		void reset_window(int w, int h);
		
		bool fullscreen;
		
		SDL_Surface *screen;
		int cameraX;
		int cameraY;
		
		int windowedW;
		int windowedH;
};
