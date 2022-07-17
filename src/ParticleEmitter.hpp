#pragma once

#include <memory>
#include <vector>
#include "Texture.hpp"

class Window;

struct Particle
{
	Particle(const Texture &texture) : 
		x(), y(), xVel(), yVel(), timeLeft(), texture(texture) { }
	
	int x, y;
	int xVel, yVel;
	int timeLeft;
	const Texture &texture;
};

class ParticleEmitter
{
	public:
		ParticleEmitter(size_t size, int x, int y);
		~ParticleEmitter();
		ParticleEmitter(const ParticleEmitter &) = delete;
		ParticleEmitter(ParticleEmitter &&) = delete;
		ParticleEmitter &operator = (const ParticleEmitter &) = delete;
		ParticleEmitter &operator = (ParticleEmitter &&) = delete;
		
		void draw(const Window &window, double interpolate);
		void update();
		
	protected:
		
	private:
		int x, y;
		
		Texture texture;
		std::vector<Particle> particles;
};
