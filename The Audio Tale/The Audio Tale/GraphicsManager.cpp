#include "GraphicsManager.h"
#include <iostream>

GraphicsManager::GraphicsManager(std::string Name, int sizeX, int sizeY)
{
	this->windowName = Name;
	this->winX = sizeX;
	this->winY = sizeY;
	
	this->mainWindow = new sf::RenderWindow(sf::VideoMode(this->winX, this->winY), this->windowName);
	std::cout << "SFML window opened" << std::endl;
	this->spriteLoader("sprite.png", wall);
	this->spriteLoader("background2.png", background);
	this->spriteLoader("background2.png", background2);
	std::cout << "All sprites added sucessfully" << std::endl;
}


GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::init()
{
	this->backgroundSpeed = -1.0f;
	this->spriteMap[background].setPosition(0, (this->winY / 2) - 250);
	this->spriteMap[background].setColor(sf::Color(0, 255, 0));
}

void GraphicsManager::game()
{
	while (this->mainWindow->isOpen())
	{
		sf::Event event;
		while (this->mainWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				this->mainWindow->close();
		}
		this->mainWindow->clear(sf::Color::Black);
		//affichage
		this->backgroundDrawing();
		
		this->mainWindow->draw(this->spriteMap[wall]);
		this->mainWindow->display();
	}
}

void GraphicsManager::backgroundDrawing()
{
	int temp;
	int diff = winX - this->spriteMap[background].getTexture()->getSize().x;
	int bg = this->spriteMap[background].getTexture()->getSize().x * -1;
	std::cout << this->spriteMap[background].getPosition().x + this->spriteMap[background].getTexture()->getSize().x << std::endl;

	this->mainWindow->draw(this->spriteMap[background]);
	if (this->spriteMap[background].getPosition().x < diff)
	{
		temp = this->spriteMap[background].getPosition().x + this->spriteMap[background].getTexture()->getSize().x;
		this->spriteMap[background2].setPosition(temp, (this->winY / 2) - 250);
		this->spriteMap[background].move(backgroundSpeed, 0);
	}
	else if (this->spriteMap[background].getPosition().x < bg)
	{
		this->spriteMap[background2].setPosition(this->spriteMap[background].getPosition().x + this->spriteMap[background].getTexture()->getSize().x, (this->winY / 2) - 250);
	}
	else
		this->spriteMap[background].move(backgroundSpeed, 0);
}

void GraphicsManager::spriteLoader(std::string filename, SpriteList name)
{
	sf::Texture texture;
	if (!texture.loadFromFile(filename))
	{
		std::cout << filename << " does not exist." << std::endl;
		return;
	}
	texture.setSmooth(true);
	this->textureList.push_back(texture);
	sf::Sprite sprite;
	sprite.setTexture(textureList.back());
	this->spriteMap[name] = sprite;
}
