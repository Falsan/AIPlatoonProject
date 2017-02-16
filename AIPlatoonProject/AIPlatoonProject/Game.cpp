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
	platoon1 = new Platoon;
	platoon2 = new Platoon;

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
	platoon1->assignTeam(1);
	platoon2->assignTeam(2);
	platoon1->createPlatoon();
	platoon2->createPlatoon();

	//m_SD1.m_leader = platoon1->soldiers[platoon1->getLeader()];
	//m_SD2.m_leader = platoon2->soldiers[platoon2->getLeader()];

	m_SD1.enemyPlatoon = platoon2;
	m_SD2.enemyPlatoon = platoon1;
	

	//testSoldier->shape.setFillColor(sf::Color::Green);
	//testSoldier->shape.setRadius(10.0f);
	for (auto iter3 = 0; iter3 != platoon1->platoonSections.size(); iter3++)
	{
		for (auto iter2 = 0; iter2 != platoon1->platoonSections[iter3]->soldiers.size(); iter2++)
		{
			for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
			{
				if (terrainManager->terrainSquares[iter]->getSpawn())
				{
					platoon1->platoonSections[iter3]->soldiers[iter2]->setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
					platoon1->platoonSections[iter3]->soldiers[iter2]->shape.setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
					platoon1->platoonSections[iter3]->soldiers[iter2]->calculateBraveryRating();
					terrainManager->terrainSquares[iter]->setSpawn(false);
					break;
				}
			}
		}
	}

	for (auto iter3 = 0; iter3 != platoon2->platoonSections.size(); iter3++)
	{
		for (auto iter2 = 0; iter2 != platoon2->platoonSections[iter3]->soldiers.size(); iter2++)
		{
			for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
			{
				if (terrainManager->terrainSquares[iter]->getSpawn())
				{
					platoon2->platoonSections[iter3]->soldiers[iter2]->setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
					platoon2->platoonSections[iter3]->soldiers[iter2]->shape.setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
					platoon2->platoonSections[iter3]->soldiers[iter2]->calculateBraveryRating();
					terrainManager->terrainSquares[iter]->setSpawn(false);
					break;
				}
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
		
		for (auto iter3 = 0; iter3 != platoon1->platoonSections.size(); iter3++)
		{
			for (auto iter2 = 0; iter2 != platoon1->platoonSections[iter3]->soldiers.size(); iter2++)
			{
				window.draw(platoon1->platoonSections[iter3]->soldiers[iter2]->shape);
			}
		}

		for (auto iter3 = 0; iter3 != platoon2->platoonSections.size(); iter3++)
		{
			for (auto iter2 = 0; iter2 != platoon2->platoonSections[iter3]->soldiers.size(); iter2++)
			{
				window.draw(platoon2->platoonSections[iter3]->soldiers[iter2]->shape);
			}
		}

		for (auto iter = 0; iter != shots.size(); iter++)
		{
			sf::Vertex bullet[2];

			bullet[0].position = shots[iter].first->shape.getPosition();
			bullet[0].color = sf::Color::White;
			bullet[1].position = shots[iter].second->shape.getPosition();
			bullet[1].color = sf::Color::White;
			

			window.draw(bullet, 2, sf::Lines);
		}

		//window.draw(testSoldier->shape);
		shots.clear();
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
			for (auto iter2 = 0; iter2 != platoon1->platoonSections.size(); iter2++)
			{
				platoon1->platoonSections[iter2]->interpretTactics();

				for (auto iter = 0; iter != platoon1->platoonSections[iter2]->soldiers.size(); iter++)
				{
					if (platoon1->platoonSections[iter2]->soldiers[iter]->getState() != dead)
					{
						if (platoon1->platoonSections[iter2]->soldiers[iter]->shape.getPosition() !=
							terrainManager->terrainSquares[platoon1->platoonSections[iter2]->soldiers[iter]->goalSquare]->shape.getPosition())
						{
							m_SD1.m_platoonSection = platoon1->platoonSections[iter2];
							platoon1->platoonSections[iter2]->soldiers[iter]->soldierThink(m_SD1);

							//debugList = testSoldier->commandList;
							//testPlatoon->soldiers[iter]->clearCommandList();
							sf::sleep(sf::milliseconds(1));
							if (platoon1->platoonSections[iter2]->soldiers[iter]->getShooting() == true)
							{
								std::pair<Soldier*, Soldier*> pair;
								pair.first = platoon1->platoonSections[iter2]->soldiers[iter];
								pair.second = platoon1->platoonSections[iter2]->soldiers[iter]->getCurrentTarget();
								shots.push_back(pair);
							}
							
						}
						else
						{
							platoon1->platoonSections[iter2]->soldiers[iter]->goalSquare = NULL;
							Toolbox::printDebugMessage("Arrived at goal");
							platoon1->platoonSections[iter2]->soldiers[iter]->needsToMove = false;
							sf::sleep(sf::milliseconds(1));
							platoon1->platoonSections[iter2]->soldiers[iter]->mapGenerated = false;
						}
					}

					if (platoon1->platoonSections[iter2]->soldiers[iter]->getState() == dead &&
						platoon1->platoonSections[iter2]->soldiers[iter]->getLeader())
					{
						for (auto iter4 = 0; iter4 != platoon1->platoonSections[iter2]->soldiers.size(); iter4++)
						{
							platoon1->platoonSections[iter2]->soldiers[iter4]->setLeaderIsDead(true);
						}
					}
				}
			}
			
			for (auto iter2 = 0; iter2 != platoon2->platoonSections.size(); iter2++)
			{
				platoon2->platoonSections[iter2]->interpretTactics();
				for (auto iter = 0; iter != platoon2->platoonSections[iter2]->soldiers.size(); iter++)
				{
					if (platoon2->platoonSections[iter2]->soldiers[iter]->getState() != dead)
					{
						if (platoon2->platoonSections[iter2]->soldiers[iter]->shape.getPosition() !=
							terrainManager->terrainSquares[platoon2->platoonSections[iter2]->soldiers[iter]->goalSquare]->shape.getPosition())
						{
							m_SD2.m_platoonSection = platoon2->platoonSections[iter2];
							platoon2->platoonSections[iter2]->soldiers[iter]->soldierThink(m_SD2);

							//debugList = testSoldier->commandList;
							//testPlatoon->soldiers[iter]->clearCommandList();
							sf::sleep(sf::milliseconds(1));
							if (platoon2->platoonSections[iter2]->soldiers[iter]->getShooting() == true)
							{
								std::pair<Soldier*, Soldier*> pair;
								pair.first = platoon2->platoonSections[iter2]->soldiers[iter];
								pair.second = platoon2->platoonSections[iter2]->soldiers[iter]->getCurrentTarget();
								shots.push_back(pair);
							}
						}
						else
						{
							terrainManager->terrainSquares[platoon2->platoonSections[iter2]->soldiers[iter]->goalSquare]->setGoal(false);
							platoon2->platoonSections[iter2]->soldiers[iter]->goalSquare = NULL;
							
							Toolbox::printDebugMessage("Arrived at goal");
							platoon2->platoonSections[iter2]->soldiers[iter]->needsToMove = false;
							sf::sleep(sf::milliseconds(1));
							platoon2->platoonSections[iter2]->soldiers[iter]->mapGenerated = false;
						}
					}

					if (platoon2->platoonSections[iter2]->soldiers[iter]->getState() == dead &&
						platoon2->platoonSections[iter2]->soldiers[iter]->getLeader())
					{
						for (auto iter4 = 0; iter4 != platoon2->platoonSections[iter2]->soldiers.size(); iter4++)
						{
							platoon2->platoonSections[iter2]->soldiers[iter4]->setLeaderIsDead(true);
						}
					}
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
				/*Toolbox::printDebugMessage("Start of command log for test soldier");

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
				//Toolbox::printDebugMessage("End of goal square: ");*/

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

	resetBoard();
}

void Game::resetBoard()
{
	for (auto iter = 0; terrainManager->terrainSquares.size() > iter; iter++)
	{
		terrainManager->terrainSquares[iter]->setGoal(false);
	}
}