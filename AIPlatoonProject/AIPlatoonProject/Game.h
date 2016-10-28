#pragma once

#include <iostream>;
//#include <string.h>
#include <SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include"Toolbox.h"

class Game
{
public:
	Game();
	~Game();

	bool runGame();

	void tick();

	bool isRunning;

	void init();


private:

	int height;
	int width;

	sf::RenderWindow* gameWindow;
	//sf::VideoMode defaultVideoMode(int, int);
	std::string windowName;
};

