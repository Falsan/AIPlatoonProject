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

	
	window.create(sf::VideoMode(800, 600), "Platoon AI Demo");
	
	sf::Event closeEvent;
	init();
	while (isRunning == true)
	{
		window.pollEvent(closeEvent);
		
		tick();
		draw();

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
	float x = 0;
	float y = 0;
	for (int iter = 0; iter < 100; iter++)
	{
		Terrain* temp = new Terrain;
		
		temp->shape.setSize(sf::Vector2f(100.0f, 100.0f));
		temp->shape.setPosition(x, y);
		temp->shape.setFillColor(sf::Color(150, 50, 250));
		terrainSquares.push_back(temp);
		x++;
		
		if (x == 10)
		{
			x = 0;
			y++;
		}

		//delete temp;
	}
}

void Game::tick()
{
	
}

void Game::draw()
{
	window.clear();
	//sf::RectangleShape rectangle(sf::Vector2f(120, 50));
	//rectangle.setPosition(10, 50);
	//rectangle.setFillColor(sf::Color::White);

	for (auto iter = 0; iter != terrainSquares.size(); iter++)
	{
		window.draw(terrainSquares[iter]->shape);
	}

	//window.draw(rectangle);
	window.display();
}