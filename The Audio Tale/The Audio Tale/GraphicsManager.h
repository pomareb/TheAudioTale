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
#include "FMODLoader.h"

enum SpriteList
{
	background,
	player,
	wallBot,
	wallTop,
	borderTop,
	borderBot
};

struct wall
{
	sf::Vector2f	pos;
	SpriteList		type;
};

class GraphicsManager
{
	float wallSpeed;
	float backgroundSpeed;

	sf::Vector2<float> *playerPos;
	int isJumping;
	int isSquatting;

	FMODLoader startLoad;
	Particle particles;
	sf::Clock mainClock;
	std::string windowName;
	int winX;
	int winY;
	sf::Color generalColor;
	sf::RenderWindow *mainWindow;
	std::map<SpriteList, sf::Sprite>	spriteMap;
	std::list<sf::Texture>				textureList;

	void backgroundDrawing();
	void borderDrawing();
	void wallDrawing();
	void wallSpawn();
	void drawSprite(std::string);
	void spriteLoader(std::string, SpriteList);
	void spritePulse(SpriteList);
	void jump();
	void squat();
public:
	GraphicsManager(std::string, int, int);
	~GraphicsManager();
	void game();
	void init();
};

