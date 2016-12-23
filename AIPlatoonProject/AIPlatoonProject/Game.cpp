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
	platoon1 = new PlatoonSection;
	platoon2 = new PlatoonSection;

	m_SD1.m_terrainManager = terrainManager;
	m_SD2.m_terrainManager = terrainManager;
}

Game::~Game()
{
	delete platoon1;
	delete platoon2;
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

	platoon1->addSoldier();
	platoon1->addSoldier();
	platoon1->addSoldier();

	platoon2->addSoldier();
	platoon2->addSoldier();
	platoon2->addSoldier();

	platoon1->chooseLeader();
	platoon2->chooseLeader();

	m_SD1.m_leader = platoon1->soldiers[platoon1->getLeader()];
	m_SD2.m_leader = platoon2->soldiers[platoon2->getLeader()];

	m_SD1.enemyPlatoon = platoon2;
	m_SD2.enemyPlatoon = platoon1;
	

	//testSoldier->shape.setFillColor(sf::Color::Green);
	//testSoldier->shape.setRadius(10.0f);
	
	for (auto iter2 = 0; iter2 != platoon1->soldiers.size(); iter2++)
	{
		for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
		{
			if (terrainManager->terrainSquares[iter]->getSpawn())
			{
				platoon1->soldiers[iter2]->setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
				platoon1->soldiers[iter2]->shape.setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
				platoon1->soldiers[iter2]->calculateBraveryRating();
				terrainManager->terrainSquares[iter]->setSpawn(false);
				break;
			}
		}
	}

	for (auto iter2 = 0; iter2 != platoon2->soldiers.size(); iter2++)
	{
		for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
		{
			if (terrainManager->terrainSquares[iter]->getSpawn())
			{
				platoon2->soldiers[iter2]->setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
				platoon2->soldiers[iter2]->shape.setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
				platoon2->soldiers[iter2]->calculateBraveryRating();
				terrainManager->terrainSquares[iter]->setSpawn(false);
				break;
			}
		}
	}

	//THIS IS THE DEBUG SETTING OF GETTING COVER
	//platoon1->setCommand("findCoverTogether");
	//testPlatoon->giveOrders();
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

		for (auto iter = 0; iter != platoon1->soldiers.size(); iter++)
		{
			window.draw(platoon1->soldiers[iter]->shape);
		}

		for (auto iter = 0; iter != platoon2->soldiers.size(); iter++)
		{
			window.draw(platoon2->soldiers[iter]->shape);
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
			for (auto iter = 0; iter != platoon1->soldiers.size(); iter++)
			{
				if (platoon1->soldiers[iter]->shape.getPosition() != terrainManager->terrainSquares[platoon1->soldiers[iter]->goalSquare]->shape.getPosition())
				{
					
					platoon1->soldiers[iter]->soldierThink(terrainManager, platoon1->soldiers[platoon1->getLeader()]);
					
					//debugList = testSoldier->commandList;
					//testPlatoon->soldiers[iter]->clearCommandList();
					sf::sleep(sf::milliseconds(10));
				}
				else
				{
					Toolbox::printDebugMessage("Arrived at goal");
					sf::sleep(sf::milliseconds(10));
					platoon1->soldiers[iter]->mapGenerated = false;
				}
			}
			
			for (auto iter = 0; iter != platoon2->soldiers.size(); iter++)
			{
				if (platoon2->soldiers[iter]->shape.getPosition() != terrainManager->terrainSquares[platoon2->soldiers[iter]->goalSquare]->shape.getPosition())
				{

					platoon2->soldiers[iter]->soldierThink(terrainManager, platoon1->soldiers[platoon2->getLeader()]);

					//debugList = testSoldier->commandList;
					//testPlatoon->soldiers[iter]->clearCommandList();
					sf::sleep(sf::milliseconds(10));
				}
				else
				{
					Toolbox::printDebugMessage("Arrived at goal");
					sf::sleep(sf::milliseconds(10));
					platoon2->soldiers[iter]->mapGenerated = false;
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
				for (auto iter = 0; iter < platoon1->soldiers.size(); iter++)
				{
					Toolbox::printDebugMessage(platoon1->soldiers[iter]->getPosition());
				}
				Toolbox::printDebugMessage("End of position: ");

				Toolbox::printDebugMessage("Goals of test soldiers is: ");
				for (auto iter = 0; iter < platoon1->soldiers.size(); iter++)
				{
					Toolbox::printDebugMessage(platoon1->soldiers[iter]->goalSquare);
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