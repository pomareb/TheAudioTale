#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>

//graphics includes
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

struct ParticleElem
{
	sf::Vector2f velocity;
	sf::Time lifeTime;
};

class Particle : public sf::Drawable, public sf::Transformable
{
	sf::Color		color;
	int elemNbr;
	std::vector<ParticleElem>	Elems;
	sf::Vector2f emitter;
	sf::Time lifeTime;
	sf::VertexArray vertices;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		states.texture = NULL;
		target.draw(vertices, states);
	}
public:
	Particle(unsigned int, sf::Color);
	~Particle();
	void update(sf::Time);
	void setEmitter(sf::Vector2f);
	void resetParticle(std::size_t);
	void setColor(sf::Color);
};

