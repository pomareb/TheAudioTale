#include "GraphicsManager.h"
#include <iostream>

GraphicsManager::GraphicsManager(std::string Name, int sizeX, int sizeY) : particles(1000)
{
	this->windowName = Name;
	this->winX = sizeX;
	this->winY = sizeY;
	
	this->mainWindow = new sf::RenderWindow(sf::VideoMode(this->winX, this->winY), this->windowName);
	std::cout << "SFML window opened" << std::endl;
	this->spriteLoader("sprite.png", wall);
	this->spriteLoader("background.png", background);
	std::cout << "All sprites added sucessfully" << std::endl;
}


GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::init()
{
	this->mainClock.restart();
	this->backgroundSpeed = -0.1f;
	this->spriteMap[background].setPosition(0, (this->winY / 2) - 250);
	this->spriteMap[background].setColor(sf::Color(0, 255, 0));
	 
	this->startLoad.loadSound("marcus_kellis_theme.mp3", "marcus_kellis_theme.mp3");
	this->startLoad.playSound();
	this->startLoad.playPauseChannels();
}

void GraphicsManager::game()
{
	while (this->mainWindow->isOpen())
	{
		startLoad.parse();
		sf::Event event;
		while (this->mainWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				this->mainWindow->close();
		}
		this->mainWindow->clear(sf::Color::Black);
		//affichage
		this->backgroundDrawing();
		
		sf::Vector2i mouse = sf::Mouse::getPosition(*(this->mainWindow));
		particles.setEmitter(this->mainWindow->mapPixelToCoords(mouse));
		sf::Time elapsed = mainClock.restart();
		particles.update(elapsed);
		this->mainWindow->draw(particles);

		//this->mainWindow->draw(this->spriteMap[wall]);
		this->mainWindow->display();
	}
}

void GraphicsManager::backgroundDrawing()
{
	int temp;
	int diff = (this->spriteMap[background].getTexture()->getSize().x / 3) - this->spriteMap[background].getTexture()->getSize().x;
	//std::cout << this->spriteMap[background].getPosition().x + this->spriteMap[background].getTexture()->getSize().x << std::endl;
	this->spriteMap[background].move(backgroundSpeed, 0);
	this->mainWindow->draw(this->spriteMap[background]);
	
	if (this->spriteMap[background].getPosition().x < diff)
	{	
		this->spriteMap[background].setPosition(0, (this->winY / 2) - 250);
		this->spriteMap[background].setColor(sf::Color(255, 0, 0));
	}
	//spritePulse(background);
	
}

void GraphicsManager::spritePulse(SpriteList sprite)
{
	sf::Color temp;
	temp = this->spriteMap[sprite].getColor();
	if (temp.r < 250)
		temp.r += 1;
	if (temp.g < 250)
		temp.g += 1;

	this->spriteMap[sprite].setColor(temp);
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

