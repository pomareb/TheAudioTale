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


enum SpriteList
{
	background,
	background2,
	wall,
	wall2
};

class GraphicsManager
{
	std::string windowName;
	int winX;
	int winY;
	int RedId, GreenId, BlueId;
	float backgroundSpeed;
	sf::RenderWindow *mainWindow;
	std::map<SpriteList, sf::Sprite>	spriteMap;
	std::list<sf::Texture>				textureList;
	
public:
	GraphicsManager(std::string, int, int);
	~GraphicsManager();
	void game();
	void spriteLoader(std::string, SpriteList);
	void drawSprite(std::string);
	void init();
	void backgroundDrawing();
};

