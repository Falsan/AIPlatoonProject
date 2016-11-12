#pragma once

#include <iostream>;
#include <string>
#include <SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include <SFML\System.hpp>
#include"Toolbox.h"
#include "TerrainManager.h"
#include "Soldier.h"
#include "GameState.h"
#include "PlatoonSection.h"

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

	void handleInput();

	TerrainManager* terrainManager;

	PlatoonSection* testPlatoon;

	//Soldier* testSoldier;

	sf::RenderWindow window;
private:

	bool logPrinted;
	std::vector<std::string> debugList;

	int gameState;

	int height;
	int width;
	
	sf::RenderWindow* gameWindow;
	//sf::VideoMode defaultVideoMode(int, int);
	std::string windowName;
};

