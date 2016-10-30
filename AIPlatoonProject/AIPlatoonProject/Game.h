#pragma once

#include <iostream>;
#include <string>
#include <SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include"Toolbox.h"
#include "Terrain.h"

class Game
{
public:
	Game();
	~Game();

	bool runGame();

	void tick();

	void draw();

	bool isRunning;

	void init();

	std::vector<Terrain*> terrainSquares;
	sf::RenderWindow window;
private:

	int height;
	int width;

	
	sf::RenderWindow* gameWindow;
	//sf::VideoMode defaultVideoMode(int, int);
	std::string windowName;
};

