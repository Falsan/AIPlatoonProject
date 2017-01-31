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
#include "SoldierData.h"
#include "Platoon.h"

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

	void resetBoard();

	TerrainManager* terrainManager;

	Platoon* platoon1;
	Platoon* platoon2;

	//Soldier* testSoldier;

	sf::RenderWindow window;
private:

	SoldierData m_SD1;
	SoldierData m_SD2;

	bool logPrinted;
	std::vector<std::string> debugList;

	int gameState;

	int height;
	int width;
	
	sf::RenderWindow* gameWindow;
	//sf::VideoMode defaultVideoMode(int, int);
	std::string windowName;
	std::vector<std::pair<Soldier*, Soldier*>> shots; //first is shooter, second is target
	
};

