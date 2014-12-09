#include "GraphicsManager.h"
#include <iostream>

GraphicsManager::GraphicsManager(std::string Name, int sizeX, int sizeY) : particles(4000)
{
	this->isJumping = 0;
	this->windowName = Name;
	this->winX = sizeX;
	this->winY = sizeY;

	this->playerPos = new sf::Vector2f(this->winX / 4, this->winY / 2);
	
	this->mainWindow = new sf::RenderWindow(sf::VideoMode(this->winX, this->winY), this->windowName);
	std::cout << "SFML window opened" << std::endl;
	this->spriteLoader("bot_walls_400.png", wallBot);
	this->spriteLoader("top_walls_400.png", wallTop);
	this->spriteLoader("background.png", background);
	this->spriteLoader("top.png", borderTop);
	this->spriteLoader("bot.png", borderBot);
	std::cout << "All sprites added sucessfully" << std::endl;
	this->isColling = false;
	this->lastIsColling = false;
	this->score = 0;
	fnt.loadFromFile("gilsanub.ttf");
	this->txt.setFont(fnt);
	this->txt.setString("Score : ");
	this->txt.setCharacterSize(24);
	this->txt.setColor(sf::Color::White);
	this->txt.setStyle(sf::Text::Bold | sf::Text::Underlined);
	this->txt.setPosition(25,100);
}


GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::init()
{
	this->mainClock.restart();
	this->backgroundSpeed = -0.5f;
	this->wallSpeed = 3 * this->backgroundSpeed;
	this->spriteMap[background].setPosition(0, (this->winY / 2) - 250);
	this->spriteMap[background].setColor(sf::Color(220, 145, 0));
	this->spriteMap[borderTop].setPosition(0, (this->winY / 2) - 280);
	this->spriteMap[borderBot].setPosition(0, (this->winY / 2) + 215);
//	this->spriteMap[wall].;
//	this->spriteMap[wallTop].setColor(sf::Color(40, 40, 40));
	this->spriteMap[wallTop].setPosition(-400, (this->winY / 2) - 250);
	this->spriteMap[wallBot].setPosition(-400, (this->winY / 2));

	// init top sprite pool
	this->wallsTop[1].center.x = -400;
	this->wallsTop[1].center.y = (this->winY / 2) - 250;
	this->wallsTop[1].pos.x = -400;
	this->wallsTop[1].pos.y = (this->winY / 2) - 250;
	this->wallsTop[1].type = wallTop;
	this->wallsTop[2].center.x = -400;
	this->wallsTop[2].center.y = (this->winY / 2) - 250;
	this->wallsTop[2].pos.x = -400;
	this->wallsTop[2].pos.y = (this->winY / 2) - 250;
	this->wallsTop[2].type = wallTop;
	this->wallsTop[3].pos.x = -400;
	this->wallsTop[3].center.x = -400;
	this->wallsTop[3].center.y = (this->winY / 2) - 250;
	this->wallsTop[3].pos.y = (this->winY / 2) - 250;
	this->wallsTop[3].type = wallTop;
	this->wallsTop[4].center.x = -400;
	this->wallsTop[4].center.y = (this->winY / 2) - 250;
	this->wallsTop[4].pos.x = -400;
	this->wallsTop[4].pos.y = (this->winY / 2) - 250;
	this->wallsTop[4].type = wallTop;

	// init bot sprite pool
	this->wallsBot[1].center.x = -400;
	this->wallsBot[1].center.y = (this->winY / 2) + 250;
	this->wallsBot[1].pos.x = -400;
	this->wallsBot[1].pos.y = (this->winY / 2);
	this->wallsBot[1].type = wallBot;
	this->wallsBot[2].center.x = -400;
	this->wallsBot[2].center.y = (this->winY / 2) + 250;
	this->wallsBot[2].pos.x = -400;
	this->wallsBot[2].pos.y = (this->winY / 2);
	this->wallsBot[2].type = wallBot;
	this->wallsBot[3].center.x = -400;
	this->wallsBot[3].center.y = (this->winY / 2) + 250;
	this->wallsBot[3].pos.x = -400;
	this->wallsBot[3].pos.y = (this->winY / 2);
	this->wallsBot[3].type = wallBot;
	this->wallsBot[4].center.x = -400;
	this->wallsBot[4].center.y = (this->winY / 2) + 250;
	this->wallsBot[4].pos.x = -400;
	this->wallsBot[4].pos.y = (this->winY / 2);
	this->wallsBot[4].type = wallBot;
	
	// init du son
	this->startLoad.loadSound("01.mp3", "01.mp3");
	this->startLoad.playSound();
	this->startLoad.playPauseChannel();
}

void GraphicsManager::game()
{
	while (this->mainWindow->isOpen())
	{
		startLoad.parse();
		
		bool beatNow = startLoad.isBeatNow();
		float frequency = startLoad.getFrequency();
		float volume = startLoad.getVolume();
		float bpmEstimate = startLoad.getBpmEstimate();
		float maxVol = startLoad.getMaxVol();
		
		sf::Event event;
		while (this->mainWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				this->mainWindow->close();
		}
		this->jump();
		this->squat();
		
		this->mainWindow->clear(sf::Color::Black);
		//affichage
		this->backgroundDrawing(startLoad.getBpmEstimate());
		if (this->startLoad.isBeatNow())
			this->wallSpawn();
		this->wallDrawing();
		//sf::Vector2i mouse = sf::Mouse::getPosition(*(this->mainWindow));
		particles.setEmitter(*(this->playerPos));
		sf::Time elapsed = mainClock.restart();
		particles.update(elapsed);
		this->mainWindow->draw(particles);
		this->borderDrawing();

		this->lastIsColling = this->isColling;
		if (this->wallCollider() == true)
			this->isColling == true;
		if (this->isColling == false && this->lastIsColling == true)
			this->score = this->score - (this->score / 15);
		this->score += (pow(sqrt(this->playerPos->y - (this->winY / 2)), 2) / 100);
		std::stringstream sstm;
		sstm << "Score : " << score;
		this->txt.setString(sstm.str());// +this->score);
		this->mainWindow->draw(this->txt);
		this->mainWindow->display();
	}
}


void GraphicsManager::wallSpawn()
{
	int i = 1;
	std::cout << "wanna create wall" << std::endl;
	while (i <= 4)
	{
		if (this->wallsTop[i].pos.x == -400)
		{
			std::cout << "spawned Top" << std::endl;
			this->wallsTop[i].pos.x = this->winX + 150;
			//this->wallsTop[i].pos.y = (this->winY / 2) - 250;
			return;
		}
		else if (this->wallsBot[i].pos.x  == -400)
		{
			std::cout << "spawned Bot" << std::endl;
			this->wallsBot[i].pos.x = this->winX + 150;
			//this->wallsBot[i].pos.y = this->winY / 2;
			return;
		}
		i++;
	}
	/*
	if (this->spriteMap[wallBot].getPosition().x == -400)
	{
		this->spriteMap[wallBot].setPosition(this->winX + 150, (this->winY / 2));
	}
	else if (this->spriteMap[wallTop].getPosition().x == -400)
	{
		this->spriteMap[wallTop].setPosition(this->winX + 150, (this->winY / 2) - 250);
	}*/
}

void GraphicsManager::wallDrawing()
{
	int i = 1;

	while (i <= 4)
	{
		if (this->wallsTop[i].pos.x > -400)
		{
			this->wallsTop[i].pos.x -= 1;// wallSpeed;
			this->wallsTop[i].center.x = this->wallsTop[i].pos.x + 200;
			this->spriteMap[wallTop].setPosition(this->wallsTop[i].pos);
			this->mainWindow->draw(this->spriteMap[wallTop]);
		}
		else
			this->wallsTop[i].pos.x = -400.0;
		if (this->wallsBot[i].pos.x > -400)
		{
			this->wallsBot[i].pos.x -= 1;// wallSpeed;
			this->wallsBot[i].center.x = this->wallsBot[i].pos.x + 200;
			this->spriteMap[wallBot].setPosition(this->wallsBot[i].pos);
			this->mainWindow->draw(this->spriteMap[wallBot]);
		}
		else
			this->wallsBot[i].pos.x = -400.0;
		i++;
	}
	/*int temp;
	int diff;

	if (this->spriteMap[wallBot].getPosition().x != -400)
	{
		diff = this->spriteMap[wallBot].getTexture()->getSize().x * (-1);
		//std::cout << this->spriteMap[wall].getPosition().x + this->spriteMap[wall].getTexture()->getSize().x << std::endl;
		this->spriteMap[wallBot].move(wallSpeed, 0);
		this->mainWindow->draw(this->spriteMap[wallBot]);
		this->spriteMap[wallBot].move(200, 0);
		if (this->spriteMap[wallBot].getPosition().x < diff)
		{
			this->spriteMap[wallBot].setPosition(-400, (this->winY / 2));
		}
	}
	if (this->spriteMap[wallTop].getPosition().x != -400)
	{
		diff = this->spriteMap[wallTop].getTexture()->getSize().x * (-1);
		//std::cout << this->spriteMap[wall].getPosition().x + this->spriteMap[wall].getTexture()->getSize().x << std::endl;
		this->spriteMap[wallTop].move(wallSpeed, 0);
		this->mainWindow->draw(this->spriteMap[wallTop]);

		if (this->spriteMap[wallTop].getPosition().x < diff)
		{
			this->spriteMap[wallTop].setPosition(-400, (this->winY / 2) - 250);
		}
	}*/
}

bool GraphicsManager::wallCollider()
{
	int i = 1;

	while (i <= 4)
	{
		//std::cout << "TOP COLLIDER TRIGGERED / " << i << " - dist : " << sqrt(pow(this->wallsTop[i].center.x - playerPos->x, 2) * pow(this->wallsTop[i].center.y - playerPos->y, 2)) << "center :" << this->wallsTop[i].center.x << " - " << this->wallsTop[i].center.y << std::endl;
		if (sqrt(pow(this->wallsTop[i].center.x - playerPos->x, 2) + pow(this->wallsTop[i].center.y - playerPos->y, 2)) < 185.0)
		{
//			std::cout << "TOP COLLIDER TRIGGERED / " << i << " - dist : " << sqrt(pow(this->wallsTop[i].center.x - playerPos->x, 2) * pow(this->wallsTop[i].center.y - playerPos->y, 2)) << "center :" << this->wallsTop[i].center.x << " - " << this->wallsTop[i].center.y<< std::endl;
			return true;
		}
		if (sqrt(pow(this->wallsBot[i].center.x - playerPos->x, 2) + pow(this->wallsBot[i].center.y - playerPos->y, 2)) < 215.0)
		{
//			std::cout << "BOT COLLIDER TRIGGERED / " << i << " - dist : " << sqrt(pow(this->wallsTop[i].center.x - playerPos->x, 2) * pow(this->wallsTop[i].center.y - playerPos->y, 2)) << "center :" << this->wallsTop[i].center.x << " - " << this->wallsTop[i].center.y << std::endl;
			return true;
		}
		i++;
	}
	return false;

}

void GraphicsManager::backgroundDrawing(float bpm)
{
	int temp;
	int diff = (this->spriteMap[background].getTexture()->getSize().x / 3) - this->spriteMap[background].getTexture()->getSize().x;
	//std::cout << this->spriteMap[background].getPosition().x + this->spriteMap[background].getTexture()->getSize().x << std::endl;
	this->spriteMap[background].move(backgroundSpeed, 0);
	this->mainWindow->draw(this->spriteMap[background]);
	if (this->spriteMap[background].getPosition().x < diff)
	{	
		this->spriteMap[background].setPosition(0, (this->winY / 2) - 250);
		this->spriteMap[background].setColor(sf::Color(196, 80 + (bpm / 2), 0));
	}
	//spritePulse(background);
	
}

void GraphicsManager::borderDrawing()
{
	int temp;
	int diff = (this->spriteMap[borderTop].getTexture()->getSize().x / 3) - this->spriteMap[borderTop].getTexture()->getSize().x;
	//std::cout << this->spriteMap[background].getPosition().x + this->spriteMap[background].getTexture()->getSize().x << std::endl;
	this->spriteMap[borderTop].move(wallSpeed, 0);
	this->spriteMap[borderBot].move(wallSpeed, 0);
	this->mainWindow->draw(this->spriteMap[borderTop]);
	this->mainWindow->draw(this->spriteMap[borderBot]);
	if (this->spriteMap[borderTop].getPosition().x < diff)
	{
		this->spriteMap[borderTop].setPosition(0, (this->winY / 2) - 280);
		this->spriteMap[borderBot].setPosition(0, (this->winY / 2) + 215);
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

void GraphicsManager::jump()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && this->isJumping == 0 && this->isSquatting == 0)
	{
		this->isJumping = 1;
	}
	if (this->isJumping == 1)
	{
		this->playerPos->y -= 0.5;
		if (this->playerPos->y <= (this->winY / 2) - 150.0f)
			this->isJumping = 2;
	}
	else if (this->isJumping == 2)
	{
		this->playerPos->y += 0.5;
		if (this->playerPos->y >= (this->winY / 2))
			this->isJumping = 0;
	}
}

void GraphicsManager::squat()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->isSquatting == 0 && this->isJumping == 0)
	{
		this->isSquatting = 1;
	}
	if (this->isSquatting == 1)
	{
		this->playerPos->y += 0.5;
		if (this->playerPos->y >= (this->winY / 2) + 150.0f)
			this->isSquatting = 2;
	}
	else if (this->isSquatting== 2)
	{
		this->playerPos->y -= 0.5;
		if (this->playerPos->y <= (this->winY / 2))
			this->isSquatting = 0;
	}
}
