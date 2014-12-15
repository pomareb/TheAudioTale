#include "GraphicsManager.h"
#include <iostream>

GraphicsManager::GraphicsManager(std::string Name, int sizeX, int sizeY) : particles(4000, sf::Color(255,255,255))
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
	this->spriteLoader("Menu.png", TATmenu);
	std::cout << "All sprites added sucessfully" << std::endl;
	this->isColliding = false;
	this->lastIsCollididng = false;
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
	this->isMenu = true;
	this->mainClock.restart();
	this->backgroundSpeed = -0.5f;
	this->wallSpeed = 3 * this->backgroundSpeed;
	std::cout << "BG speed is " << this->backgroundSpeed << std::endl;
	std::cout << "Walls speed is " << this->wallSpeed << std::endl;
	this->spriteMap[background].setPosition(0, (this->winY / 2) - 250);
	this->spriteMap[background].setColor(sf::Color(220, 145, 0));
	this->spriteMap[borderTop].setPosition(0, (this->winY / 2) - 280);
	this->spriteMap[borderBot].setPosition(0, (this->winY / 2) + 215);
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
}


void GraphicsManager::loop()
{
	while (this->mainWindow->isOpen())
	{
		if (this->isMenu == true)
			this->menu();
		else
			this->game();
	}
}

void GraphicsManager::menu()
{
	sf::Event event;
	while (this->mainWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			this->mainWindow->close();
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		int x = sf::Mouse::getPosition(*(this->mainWindow)).x;
		int y = sf::Mouse::getPosition(*(this->mainWindow)).y;
		// launch game
		if (x >= ((this->winX / 2) - (1920 / 2) + 650) && x <= (this->winX / 2) - (1920 / 2) + 850)
		{
			// play
			if (y > ((this->winY / 2) - (1080 / 2) + 420) && y < ((this->winY / 2) - (1080 / 2) + 520))
			{
				this->isMenu = false;
				this->launch();
			}
			
			if (y >((this->winY / 2) - (1080 / 2) + 580) && y < ((this->winY / 2) - (1080 / 2) + 670))
			{
				this->mainWindow->close();
			}
		}
	}
	this->spriteMap[TATmenu].setPosition((this->winX / 2) - (1920 / 2), (this->winY / 2) - (1080 / 2));
	this->mainWindow->draw(this->spriteMap[TATmenu]);
	this->mainWindow->display();

}

void GraphicsManager::launch()
{
	// sound init
	this->startLoad.loadSound("trauma.mp3", "trauma.mp3");
	this->startLoad.playSound();
	this->startLoad.playPauseChannel();
	this->startLoad.delayedStartChannels(250.0);
}

void GraphicsManager::game()
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
		

	this->lastIsCollididng = this->isColliding;
	this->isColliding = false;
	if (this->wallCollider() == true)
		this->isColliding = true;
	if (this->isColliding == false && this->lastIsCollididng == true)
		this->score = this->score - (this->score / 15);
	this->score += (abs(this->playerPos->y - (this->winY / 2)) / 100);
	std::stringstream sstm;
	sstm << "Score : " << score;
	this->txt.setString(sstm.str());

	this->mainWindow->clear(sf::Color::Black);
	//affichage
	this->backgroundDrawing(startLoad.getBpmEstimate());
	if (this->startLoad.isBeatNow())
		this->wallSpawn();
	this->wallDrawing();
	particles.setEmitter(*(this->playerPos));
	sf::Time elapsed = mainClock.restart();
	if (this->isColliding == true)
		this->particles.setColor(sf::Color(255,64,48));
	else
		this->particles.setColor(sf::Color(128,128,128));
		
	particles.update(elapsed);
	this->mainWindow->draw(particles);
	this->borderDrawing();

		

	this->mainWindow->draw(this->txt);
	this->mainWindow->display();
}


void GraphicsManager::wallSpawn()
{
	int i = 1;
	while (i <= 4)
	{
		if (this->wallsTop[i].pos.x == -400)
		{
			this->wallsTop[i].pos.x = this->winX + 150;

			return;
		}
		else if (this->wallsBot[i].pos.x  == -400)
		{
			this->wallsBot[i].pos.x = this->winX + 150;
			return;
		}
		i++;
	}

}

void GraphicsManager::wallDrawing()
{
	int i = 1;

	if (this->isColliding == true)
	{
		this->spriteMap[wallTop].setColor(sf::Color(255, 128, 128));
		this->spriteMap[wallBot].setColor(sf::Color(255, 128, 128));
	}
	else
	{
		this->spriteMap[wallTop].setColor(sf::Color(255, 255, 255));
		this->spriteMap[wallBot].setColor(sf::Color(255, 255, 255));
	}
	while (i <= 4)
	{
		if (this->wallsTop[i].pos.x > -400)
		{
			if (this->wallsTop[i].pos.x < 400)
				this->spriteMap[wallTop].setColor(sf::Color(0, 255, 0));
			else
				this->spriteMap[wallTop].setColor(sf::Color(255, 255, 255));

			this->wallsTop[i].pos.x -= 2;
			this->wallsTop[i].center.x = this->wallsTop[i].pos.x + 200;
			this->spriteMap[wallTop].setPosition(this->wallsTop[i].pos);
			this->mainWindow->draw(this->spriteMap[wallTop]);
		}
		else
			this->wallsTop[i].pos.x = -400.0;
		if (this->wallsBot[i].pos.x > -400)
		{
			if (this->wallsBot[i].pos.x < 400)
				this->spriteMap[wallBot].setColor(sf::Color(0, 255, 0));
			else
				this->spriteMap[wallBot].setColor(sf::Color(255, 255, 255));

			this->wallsBot[i].pos.x -= 2;
			this->wallsBot[i].center.x = this->wallsBot[i].pos.x + 200;
			this->spriteMap[wallBot].setPosition(this->wallsBot[i].pos);
			this->mainWindow->draw(this->spriteMap[wallBot]);

		}
		else
			this->wallsBot[i].pos.x = -400.0;
		i++;
	}
}

bool GraphicsManager::wallCollider()
{
	int i = 1;

	while (i <= 4)
	{

		if (sqrt(pow(this->wallsTop[i].center.x - playerPos->x, 2) + pow(this->wallsTop[i].center.y - playerPos->y, 2)) < 185.0)
		{
			return true;
		}
		if (sqrt(pow(this->wallsBot[i].center.x - playerPos->x, 2) + pow(this->wallsBot[i].center.y - playerPos->y, 2)) < 215.0)
		{
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
	this->spriteMap[background].move(backgroundSpeed, 0);
		this->spriteMap[background].setColor(sf::Color(196, 80 + (bpm / 2), 0));
	this->mainWindow->draw(this->spriteMap[background]);
	if (this->spriteMap[background].getPosition().x < diff)
	{	
		this->spriteMap[background].setPosition(0, (this->winY / 2) - 250);
	}
	
}

void GraphicsManager::borderDrawing()
{
	int temp;
	int diff = (this->spriteMap[borderTop].getTexture()->getSize().x / 3) - this->spriteMap[borderTop].getTexture()->getSize().x;
	this->spriteMap[borderTop].move(wallSpeed, 0);
	this->spriteMap[borderBot].move(wallSpeed, 0);
	this->mainWindow->draw(this->spriteMap[borderTop]);
	this->mainWindow->draw(this->spriteMap[borderBot]);
	if (this->spriteMap[borderTop].getPosition().x < diff)
	{
		this->spriteMap[borderTop].setPosition(0, (this->winY / 2) - 280);
		this->spriteMap[borderBot].setPosition(0, (this->winY / 2) + 215);
	}
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
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) && this->isSquatting == 0)
	{
		this->isJumping = 1;
	}
	if (this->isJumping == 1)
	{
		if (this->playerPos->y <= (this->winY / 2) - 150.0f)
			this->isJumping = 2;
		else
			this->playerPos->y -= 0.5;

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
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) && this->isJumping == 0)
	{
		this->isSquatting = 1;
	}
	if (this->isSquatting == 1)
	{
		if (this->playerPos->y >= (this->winY / 2) + 150.0f)
			this->isSquatting = 2;
		else
			this->playerPos->y += 0.5;

	}
	else if (this->isSquatting== 2)
	{
		this->playerPos->y -= 0.5;
		if (this->playerPos->y <= (this->winY / 2))
			this->isSquatting = 0;
	}
}
