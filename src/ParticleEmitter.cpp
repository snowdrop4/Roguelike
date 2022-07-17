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
