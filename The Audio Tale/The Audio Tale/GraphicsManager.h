#pragma once

#include <list>
#include <string>
#include <map>
#include <iostream>
#include <sstream>

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
	TATmenu,
	background,
	player,
	wallBot,
	wallTop,
	borderTop,
	borderBot
};

struct wall
{
	sf::Vector2<float>	pos;
	sf::Vector2<float>	center;
	SpriteList		type;
};

class GraphicsManager
{
	bool isMenu;
	sf::Font fnt;
	sf::Text txt;
	int score;

	float wallSpeed;
	float backgroundSpeed;

	sf::Vector2<float> *playerPos;
	int isJumping;
	int isSquatting;
	bool isColliding;
	bool lastIsCollididng;

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
	std::map<int, wall>					wallsTop;
	std::map<int, wall>					wallsBot;

	void backgroundDrawing(float);
	void borderDrawing();
	void wallDrawing();
	bool wallCollider();
	void wallSpawn();
	void drawSprite(std::string);
	void spriteLoader(std::string, SpriteList);
	void spritePulse(SpriteList);
	void jump();
	void squat();

	void launch();
	void game();
	void menu();
public:
	GraphicsManager(std::string, int, int);
	~GraphicsManager();
	void loop();
	void init();
};

