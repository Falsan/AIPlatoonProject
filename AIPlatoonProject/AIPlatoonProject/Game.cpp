#include "Game.h"

Game::Game()
{
	isRunning = true;
	height = 800;
	width = 600;
	windowName = "PlatoonAIDemo";
	terrainManager = new TerrainManager;
	testSoldier = new Soldier;
}

Game::~Game()
{
	delete testSoldier;
	delete terrainManager;
}

bool Game::runGame()
{
	//defaultVideoMode(height, width);
	//gameWindow = new sf::RenderWindow(sf::VideoMode(height, width), windowName);

	sf::Thread drawThread(&Game::draw, this);
	sf::Thread inputThread(&Game::handleInput, this);
	
	
	sf::Event event;
	init();

	window.setActive(false);
	drawThread.launch();
	inputThread.launch();

	while (isRunning == true)
	{
		
		tick();
		
	
		window.pollEvent(event);

		if (event.type == sf::Event::Closed)
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

	testSoldier->shape.setFillColor(sf::Color::Green);
	testSoldier->shape.setRadius(10.0f);
	testSoldier->shape.setPosition(sf::Vector2f(0.0f, 0.0f));
	testSoldier->setPosition(sf::Vector2f(0.0f, 0.0f));
}

void Game::tick()
{

}

void Game::draw()
{
	while (window.isOpen())
	{
		for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
		{
			window.draw(terrainManager->terrainSquares[iter]->shape);
		}

		window.draw(testSoldier->shape);
		
		window.display();
		window.clear();
	}
}

void Game::handleInput()
{
	while (window.isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		{
			if (testSoldier->shape.getPosition() == terrainManager->terrainSquares[terrainManager->goalSquare]->shape.getPosition())
			{

			}
			else
			{
				testSoldier->moveTowardsGoal(terrainManager->terrainSquares[terrainManager->goalSquare]->shape.getPosition(), terrainManager);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			testSoldier->moveUp();
			sf::sleep(sf::milliseconds(100));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			testSoldier->moveDown();
			sf::sleep(sf::milliseconds(100));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			testSoldier->moveLeft();
			sf::sleep(sf::milliseconds(100));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			testSoldier->moveRight();
			sf::sleep(sf::milliseconds(100));
		}
	}
}