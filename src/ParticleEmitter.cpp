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

#include "ParticleEmitter.hpp"

#include "Texture.hpp"
#include "constants.hpp"

// -----------------------------------------------------------------------------
// Public
// -----------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter(size_t size, int x, int y) :
	x(x), y(y),
	texture("../resources/particles/red.png", Colors::textureColorKey),
	particles()
{
	particles.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		particles.emplace_back(texture);
		
		particles.back().x = x;
		particles.back().y = y;
		
		particles.back().xVel = ((rand() % 2) ? 1 : -1) * (rand() % 361);
		particles.back().yVel = ((rand() % 2) ? 1 : -1) * (rand() % 361);
		
		particles.back().timeLeft = rand() % (25 * 10);
	}
}

ParticleEmitter::~ParticleEmitter() { }

void ParticleEmitter::draw(const Window &window, double interpolate)
{
	for (auto &i : particles)
		i.texture.draw(window, i.x, i.y);
}

void ParticleEmitter::update()
{
	for (auto &i : particles)
	{
		i.x += i.xVel * 0.01;
		i.y += i.yVel * 0.01;
		
		--i.timeLeft;
		
		if (i.timeLeft == 0)
		{
			i.x = x;
			i.y = y;
			
			i.xVel = ((rand() % 2) ? 1 : -1) * (rand() % 361);
			i.yVel = ((rand() % 2) ? 1 : -1) * (rand() % 361);
			
			i.timeLeft = rand() % (25 * 16);
		}
	}
}

// -----------------------------------------------------------------------------
// Private
// -----------------------------------------------------------------------------
