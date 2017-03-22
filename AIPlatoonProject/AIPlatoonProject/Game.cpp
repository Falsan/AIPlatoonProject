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

//main game loop
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

//used for initialisation
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
	
	initPlatoonLoop(platoon1);
	initPlatoonLoop(platoon2);

}

void Game::initPlatoonLoop(Platoon* platoon)
{
	for (auto iter3 = 0; iter3 != platoon->platoonSections.size(); iter3++)
	{
		for (auto iter2 = 0; iter2 != platoon->platoonSections[iter3]->soldiers.size(); iter2++)
		{
			for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
			{
				if (terrainManager->terrainSquares[iter]->getSpawn())
				{
					platoon->platoonSections[iter3]->soldiers[iter2]->setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
					platoon->platoonSections[iter3]->soldiers[iter2]->shape.setPosition(terrainManager->terrainSquares[iter]->shape.getPosition());
					platoon->platoonSections[iter3]->soldiers[iter2]->calculateBraveryRating();
					terrainManager->terrainSquares[iter]->setSpawn(false);
					break;
				}
			}
		}
	}
}

//this is the main loop for processing AI decisions
void Game::tick()
{
	while (window.isOpen())
	{
		if (gameState == play)
		{
			tickPlatoonLoop(platoon1, m_SD1);
			tickPlatoonLoop(platoon2, m_SD2);


		}
		else
		{

		}

		resetBoard();
	}
	//resetBoard();
}

//the main platoon loop, put into it's own function to reduce code duplication
void Game::tickPlatoonLoop(Platoon* platoon, SoldierData _SD)
{
	for (auto iter2 = 0; iter2 != platoon->platoonSections.size(); iter2++)
	{
		if (platoon->platoonSections[iter2]->getIsLeaderDead() == false)
		{
			platoon->platoonSections[iter2]->interpretTactics();
		}
		else
		{
			platoon->platoonSections[iter2]->setCommand("");
		}

		for (auto iter = 0; iter != platoon->platoonSections[iter2]->soldiers.size(); iter++)
		{
			if (platoon->platoonSections[iter2]->soldiers[iter]->getState() != dead)
			{
				if (platoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition() !=
					terrainManager->terrainSquares[platoon->platoonSections[iter2]->soldiers[iter]->goalSquare]->shape.getPosition())
				{
					_SD.m_platoonSection = platoon->platoonSections[iter2];
					platoon->platoonSections[iter2]->soldiers[iter]->soldierThink(_SD);

					//debugList = testSoldier->commandList;
					//testPlatoon->soldiers[iter]->clearCommandList();
					sf::sleep(sf::milliseconds(1));
					if (platoon->platoonSections[iter2]->soldiers[iter]->getShooting() == true)
					{
						std::pair<Soldier*, Soldier*> pair;
						pair.first = platoon->platoonSections[iter2]->soldiers[iter];
						pair.second = platoon->platoonSections[iter2]->soldiers[iter]->getCurrentTarget();
						shots.push_back(pair);
					}

				}
				else
				{
					platoon->platoonSections[iter2]->soldiers[iter]->goalSquare = NULL;
					Toolbox::printDebugMessage("Arrived at goal");
					platoon->platoonSections[iter2]->soldiers[iter]->needsToMove = false;
					sf::sleep(sf::milliseconds(1));
					platoon->platoonSections[iter2]->soldiers[iter]->mapGenerated = false;
				}
			}
			else if (platoon->platoonSections[iter2]->soldiers[iter]->getState() == dead)
			{
				for (int iter5 = 0; iter5 < terrainManager->terrainSquares.size(); iter5++)
				{
					if (terrainManager->terrainSquares[iter5]->shape.getPosition()
						== platoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition())
					{
						terrainManager->terrainSquares[iter5]->setIsOccupied(false);
					}
				}
			}

			if (platoon->platoonSections[iter2]->soldiers[iter]->getState() == dead &&
				platoon->platoonSections[iter2]->soldiers[iter]->getLeader())
			{
				platoon->platoonSections[iter2]->setIsLeaderDead(true);
				for (auto iter4 = 0; iter4 != platoon->platoonSections[iter2]->soldiers.size(); iter4++)
				{

					platoon->platoonSections[iter2]->soldiers[iter4]->setLeaderIsDead(true);
				}
			}
		}
	}
}

//this thread handles drawing and rendering
void Game::draw()
{
	while (window.isOpen())
	{
		for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++)
		{
			window.draw(terrainManager->terrainSquares[iter]->shape);
		}
		
		//dead platoon members drawn first so that alive members do not seemingly vanish when moving over them
		drawDeadPlatoonLoop(platoon1);
		drawDeadPlatoonLoop(platoon2);

		drawPlatoonLoop(platoon1);
		drawPlatoonLoop(platoon2);

		//draws the bullets for that pass
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

void Game::drawPlatoonLoop(Platoon* platoon)
{
	for (auto iter3 = 0; iter3 != platoon->platoonSections.size(); iter3++)
	{
		for (auto iter2 = 0; iter2 != platoon->platoonSections[iter3]->soldiers.size(); iter2++)
		{
			if (platoon->platoonSections[iter3]->soldiers[iter2]->getState() != dead)
			{
				window.draw(platoon->platoonSections[iter3]->soldiers[iter2]->shape);
			}
		}
	}
}

void Game::drawDeadPlatoonLoop(Platoon* platoon)
{
	for (auto iter3 = 0; iter3 != platoon->platoonSections.size(); iter3++)
	{
		for (auto iter2 = 0; iter2 != platoon->platoonSections[iter3]->soldiers.size(); iter2++)
		{
			if (platoon->platoonSections[iter3]->soldiers[iter2]->getState() == dead)
			{
				window.draw(platoon->platoonSections[iter3]->soldiers[iter2]->shape);
			}
		}
	}
}

//mostly this thread is used for debug purposes to allow the display
//of debug in the console window
void Game::handleInput()
{
	while (window.isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			gameState = pause;
			logPrinted = false;
		}

		
		else if (gameState == pause)
		{
			if (logPrinted == false)
			{
				Toolbox::printDebugMessage("Positions of Platoon1 soldiers is: ");
				for (auto iter2 = 0; iter2 != platoon1->platoonSections.size(); iter2++)
				{
					
					for (auto iter = 0; iter != platoon1->platoonSections[iter2]->soldiers.size(); iter++)
					{
						Toolbox::printDebugMessage(platoon1->platoonSections[iter2]->soldiers[iter]->getPosition());
					}

				}
				Toolbox::printDebugMessage("End of positions: ");

				Toolbox::printDebugMessage("Goals of Platoon1 soldiers is: ");
				for (auto iter2 = 0; iter2 != platoon1->platoonSections.size(); iter2++)
				{

					for (auto iter = 0; iter != platoon1->platoonSections[iter2]->soldiers.size(); iter++)
					{
						Toolbox::printDebugMessage(platoon1->platoonSections[iter2]->soldiers[iter]->goalSquare);
					}

				}
				Toolbox::printDebugMessage("End of goals: ");

				Toolbox::printDebugMessage("Positions of Platoon2 soldiers is: ");
				for (auto iter2 = 0; iter2 != platoon2->platoonSections.size(); iter2++)
				{

					for (auto iter = 0; iter != platoon2->platoonSections[iter2]->soldiers.size(); iter++)
					{
						Toolbox::printDebugMessage(platoon2->platoonSections[iter2]->soldiers[iter]->getPosition());
					}

				}
				Toolbox::printDebugMessage("End of positions: ");

				Toolbox::printDebugMessage("Goals of Platoon2 soldiers is: ");
				for (auto iter2 = 0; iter2 != platoon2->platoonSections.size(); iter2++)
				{

					for (auto iter = 0; iter != platoon2->platoonSections[iter2]->soldiers.size(); iter++)
					{
						Toolbox::printDebugMessage(platoon2->platoonSections[iter2]->soldiers[iter]->goalSquare);
					}

				}
				Toolbox::printDebugMessage("End of goals: ");



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
	}
	
}

void Game::resetBoard()
{
	for (auto iter = 0; terrainManager->terrainSquares.size() > iter; iter++)
	{
		terrainManager->terrainSquares[iter]->setGoal(false);
	}
}