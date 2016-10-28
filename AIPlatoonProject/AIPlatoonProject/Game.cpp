#include "Game.h"

Game::Game()
{
	isRunning = true;
	height = 800;
	width = 600;
	windowName = "PlatoonAIDemo";
}

Game::~Game()
{
}

bool Game::runGame()
{
	//defaultVideoMode(height, width);
	//gameWindow = new sf::RenderWindow(sf::VideoMode(height, width), windowName);

	sf::RenderWindow window(sf::VideoMode(800, 600), "Platoon AI Demo");
	sf::Event closeEvent;
	init();
	while (isRunning == true)
	{
		window.pollEvent(closeEvent);
		
		tick();

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

}

void Game::tick()
{
	
}