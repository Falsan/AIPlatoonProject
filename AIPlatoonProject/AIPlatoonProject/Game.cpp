#include "Game.h"

Game::Game()
{
	isRunning = true;
	height = 800;
	width = 600;
	windowName = "PlatoonAIDemo";
	terrainManager = new TerrainManager;
}

Game::~Game()
{
	delete terrainManager;
}

bool Game::runGame()
{
	//defaultVideoMode(height, width);
	//gameWindow = new sf::RenderWindow(sf::VideoMode(height, width), windowName);

	sf::Thread drawThread(&Game::draw, this);
	
	
	sf::Event closeEvent;
	init();

	window.setActive(false);
	drawThread.launch();

	while (isRunning == true)
	{
		
		tick();
		
	
		window.pollEvent(closeEvent);

		if (closeEvent.type == sf::Event::Closed)
		{
			isRunning = false;
			window.close();
		}
	}
	
	if (isRunning == false)
	{
		return false;
	}

	std::cout << "Issue has occured, reopening game";
	return true;
}

void Game::init()
{
	window.create(sf::VideoMode(800, 600), "Platoon AI Demo");
	terrainManager->setUpTerrainSquares();
}

void Game::tick()
{
	
}

void Game::draw()
{
	//sf::RectangleShape rectangle(sf::Vector2f(120, 50));
	//rectangle.setPosition(10, 50);
	//rectangle.setFillColor(sf::Color::White);
	
	for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
	{
		window.draw(terrainManager->terrainSquares[iter]->shape);
	}
	
	//window.draw(rectangle);
	window.display();
	window.clear();
}