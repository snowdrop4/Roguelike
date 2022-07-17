#pragma once

#include "UtilDefines.hpp"

class Window;

class Animation
{
	public:
		Animation();
		COPY_CTOR(Animation) = delete;
		MOVE_CTOR(Animation) = default;
		COPY_ASSIGN(Animation) = delete;
		MOVE_ASSIGN(Animation) = default;
		virtual ~Animation();
	
	virtual bool finished() const = 0;
	virtual void draw(const Window &window, double interpolate) const = 0;
	
	protected:
	
	private:
		bool isFinished;
};
