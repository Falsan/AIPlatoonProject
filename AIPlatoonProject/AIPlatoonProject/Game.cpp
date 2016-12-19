#include "Game.h"

Game::Game()
{
	isRunning = true;
	//mapGenerated = false;
	height = 800;
	width = 600;
	windowName = "PlatoonAIDemo";
	terrainManager = new TerrainManager;
	//testSoldier = new Soldier;
	gameState = play;
	testPlatoon = new PlatoonSection;
}

Game::~Game()
{
	delete testPlatoon;
	//delete testSoldier;
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

	testPlatoon->addSoldier();
	testPlatoon->addSoldier();
	testPlatoon->addSoldier();
	testPlatoon->addSoldier();
	testPlatoon->addSoldier();
	testPlatoon->addSoldier();
	testPlatoon->addSoldier();
	testPlatoon->addSoldier();
	testPlatoon->addSoldier();
	testPlatoon->chooseLeader();
	

	//testSoldier->shape.setFillColor(sf::Color::Green);
	//testSoldier->shape.setRadius(10.0f);
	
	for (auto iter2 = 0; iter2 != testPlatoon->soldiers.size(); iter2++)
	{
		for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
		{
			if (terrainManager->terrainSquares[iter]->getSpawn())
			{
				testPlatoon->soldiers[iter2]->setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
				testPlatoon->soldiers[iter2]->shape.setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
				terrainManager->terrainSquares[iter]->setSpawn(false);
				break;
			}
		}
	}

	//THIS IS THE DEBUG SETTING OF GETTING COVER
	testPlatoon->setCommand("findCoverTogether");
	testPlatoon->giveOrders();
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

		for (auto iter = 0; iter != testPlatoon->soldiers.size(); iter++)
		{
			window.draw(testPlatoon->soldiers[iter]->shape);
		}

		//window.draw(testSoldier->shape);
		
		window.display();
		window.clear();
	}
}

void Game::handleInput()
{
	while (window.isOpen())
	{
		if (gameState == play)
		{
			for (auto iter = 0; iter != testPlatoon->soldiers.size(); iter++)
			{
				if (testPlatoon->soldiers[iter]->shape.getPosition() != terrainManager->terrainSquares[testPlatoon->soldiers[iter]->goalSquare]->shape.getPosition())
				{
					if (testPlatoon->soldiers[iter]->mapGenerated == false)
					{
						testPlatoon->soldiers[iter]->generateMapToGoal(terrainManager->terrainSquares[testPlatoon->soldiers[iter]->goalSquare]->shape.getPosition(), terrainManager);
						testPlatoon->soldiers[iter]->mapGenerated = true;
					}
					if (testPlatoon->soldiers[iter]->commandList.size() > 0)
					{
						testPlatoon->soldiers[iter]->executeCommand(terrainManager, testPlatoon->soldiers[testPlatoon->getLeader()]);
					}
					else if (testPlatoon->soldiers[iter]->commandList.size() == 0)
					{
						
						testPlatoon->soldiers[iter]->pathFindToGoal(terrainManager->terrainSquares[testPlatoon->soldiers[iter]->goalSquare]->shape.getPosition(), terrainManager);
						testPlatoon->soldiers[iter]->executeCommand(terrainManager, testPlatoon->soldiers[testPlatoon->getLeader()]);
					}
					
					//debugList = testSoldier->commandList;
					testPlatoon->soldiers[iter]->clearCommandList();
					sf::sleep(sf::milliseconds(10));
				}
				else
				{
					Toolbox::printDebugMessage("Arrived at goal");
					sf::sleep(sf::milliseconds(10));
					testPlatoon->soldiers[iter]->mapGenerated = false;
				}
			}
			
			/*
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
			}*/

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				gameState = pause;
			}

			logPrinted = false;
		}

		else if(gameState == pause)
		{
			if (logPrinted == false)
			{
				Toolbox::printDebugMessage("Start of command log for test soldier");

				for (auto iter = 0; iter != debugList.size(); iter++)
				{
					Toolbox::printDebugMessage(debugList[iter]);
				}

				Toolbox::printDebugMessage("End of command log for test soldier");

				Toolbox::printDebugMessage("Positions of test soldiers is: ");
				for (auto iter = 0; iter < testPlatoon->soldiers.size(); iter++)
				{
					Toolbox::printDebugMessage(testPlatoon->soldiers[iter]->getPosition());
				}
				Toolbox::printDebugMessage("End of position: ");

				Toolbox::printDebugMessage("Goals of test soldiers is: ");
				for (auto iter = 0; iter < testPlatoon->soldiers.size(); iter++)
				{
					Toolbox::printDebugMessage(testPlatoon->soldiers[iter]->goalSquare);
				}
				Toolbox::printDebugMessage("End of goals: ");

				//Toolbox::printDebugMessage("Goal square is: ");
				//Toolbox::printDebugMessage(terrainManager->terrainSquares[testPlatoon->soldiers[iter]->goalSquare]->shape.getPosition());
				//Toolbox::printDebugMessage("End of goal square: ");

				logPrinted = true;
			}
			else
			{

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				gameState = play;
			}
		}
		else
		{

		}
		
	}
}