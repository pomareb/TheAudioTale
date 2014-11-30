#pragma once

#include <list>
#include <string>
#include <map>

//graphics includes
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include "Particle.h"
enum SpriteList
{
	background,
	player,
	wall,
	wall2
};

class GraphicsManager
{
	Particle particles;
	sf::Clock mainClock;
	std::string windowName;
	int winX;
	int winY;
	sf::Color generalColor;
	float backgroundSpeed;
	sf::RenderWindow *mainWindow;
	std::map<SpriteList, sf::Sprite>	spriteMap;
	std::list<sf::Texture>				textureList;

	void backgroundDrawing();
	void drawSprite(std::string);
	void spriteLoader(std::string, SpriteList);
	void spritePulse(SpriteList);
public:
	GraphicsManager(std::string, int, int);
	~GraphicsManager();
	void game();
	void init();
};

