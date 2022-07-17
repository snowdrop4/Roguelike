#pragma once

#include "Timer.hpp"
#include <functional>

// http://www.reddit.com/tb/9fufw

/// Terminates the loop if 'update()' returns true.
/// 
/// Calls 'update()' 'updatesPerSec' times per second and 'draw()' as many
/// times as possible.
/// 
/// Provides the 'update()' function with the FPS as an argument.
/// Provides the 'draw()' function with an interpolation value as an argument.
void run_loop(int updatesPerSec,
              const std::function<bool(int)>                &update,
              const std::function<void(double interpolate)> &draw);
