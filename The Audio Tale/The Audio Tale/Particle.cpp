#include "Particle.h"

Particle::Particle(unsigned int maxParticles) : vertices(sf::Points, maxParticles), emitter(0, 0), Elems(maxParticles)
{
	this->elemNbr = maxParticles;
	this->lifeTime = sf::seconds(3);
}

void Particle::setEmitter(sf::Vector2f pos)
{
	this->emitter = pos;
}
void Particle::update(sf::Time elapsed)
{
	for (std::size_t i = 0; i < Elems.size(); ++i)
	{
		ParticleElem& p = Elems[i];
		p.lifeTime -= elapsed;
		if (p.lifeTime <= sf::Time::Zero)
			resetParticle(i);
		vertices[i].position += p.velocity * elapsed.asSeconds();
		float ratio = p.lifeTime.asSeconds() / lifeTime.asSeconds();
		vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
	}
}

void Particle::resetParticle(std::size_t index)
{
	float angle = (180 - 25/2 + (std::rand() % 25)) * 3.14f / 180.f;
	float speed = (std::rand() % 50) + 400.f;
	Elems[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
	Elems[index].lifeTime = sf::milliseconds((std::rand() % 1000) + 500);
	vertices[index].position = emitter;
}
Particle::~Particle()
{
}