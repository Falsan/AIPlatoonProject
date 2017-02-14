#include "Soldier.h"

Soldier::Soldier()
{
	state = aliveAndWell;
	isLeader = false;
	mapGenerated = false;
	setHealth(20);
	needsToMove = false;
	leaderIsDead = false;
	brain = new TacticsCodec;

}

Soldier::~Soldier()
{
	delete brain;
	delete target;
}

void Soldier::soldierThink(SoldierData _SD)
{
	brain->think(_SD, this);
}

void Soldier::shoot(Platoon* enemyPlatoon)
{
	//pew pew

	bool hasTargetInRange = false;

	std::pair<float, Soldier*> pair = Toolbox::findDistanceOfEnemiesAndTarget(enemyPlatoon, this);

	float distance = pair.first;
	target = pair.second;

	if (distance < 100000.0f)
	{
		hasTargetInRange = true;
		inRange = true;

		//delete target;
	}
	else
	{
		hasTargetInRange = false;
		inRange = false;
	}

	equippedWeapon->shoot(enemyPlatoon, target, inRange, this, hasTargetInRange);
	
	//delete target;
}


void Soldier::executeCommand(TerrainManager* terrainManager, Soldier* leader, Platoon* enemyPlatoon)
{
	
	bool excecuting = true;
	if (mapGenerated == false)
	{
		generateMapToGoal(terrainManager->terrainSquares[goalSquare]->shape.getPosition(), terrainManager);
	}
	if (commandList.size() > 0)
	{
		while (excecuting == true)
		{
			if (commandList.size() <= 0)
			{
				excecuting = false;
			}
			else if (commandList.front() == "moveUp")
			{
				moveUp(terrainManager);
				setShooting(false);
				excecuting = false;
			}
			else if (commandList.front() == "moveDown")
			{
				moveDown(terrainManager);
				setShooting(false);
				excecuting = false;
			}
			else if (commandList.front() == "moveLeft")
			{
				moveLeft(terrainManager);
				setShooting(false);
				excecuting = false;
			}
			else if (commandList.front() == "moveRight")
			{
				moveRight(terrainManager);
				setShooting(false);
				excecuting = false;
			}
			else if (commandList.front() == "findCover")
			{
				//commandList.erase(commandList.begin());
				clearCommandList();
				findCover(terrainManager); //might be something wrong with this function
				generateMapToGoal(terrainManager->terrainSquares[goalSquare]->shape.getPosition(), terrainManager);
				pathFindToGoal(terrainManager->terrainSquares[goalSquare]->shape.getPosition(), terrainManager);
				setShooting(false);
				//excecuting = false;
			}
			else if (commandList.front() == "findCoverTogether")
			{
				setShooting(false);
				//findCoverTogether(terrainManager, leader);
				//clearCommandList();
				//generateMapToGoal(terrainManager->terrainSquares[goalSquare]->shape.getPosition(), terrainManager);
				//pathFindToGoal(terrainManager->terrainSquares[goalSquare]->shape.getPosition(), terrainManager);
			}
			else if (commandList.front() == "fire")
			{
				shoot(enemyPlatoon);
				setShooting(true);
				excecuting = false;
			}
			else if (commandList.front() == "hunkerDown")
			{
				hunkerDown();
				setShooting(false);
				excecuting = false;
			}
			else if (commandList.front() == "advance")
			{
				brain->advance(enemyPlatoon, terrainManager, leader, this);
				
				setShooting(false);
				excecuting = false;
			}
			
		}
		
	}
	else
	{

	}
	
}

void Soldier::addCommandToList(std::string command)
{
	commandList.push_back(command);
}

void Soldier::clearCommandList()
{
	if (commandList.size() > 0)
	{
		prevCommandList.clear();
		commandList.erase(commandList.begin());
		prevCommandList = commandList;
		commandList.clear();
	}
}

void Soldier::findCover(TerrainManager* terrainManager)
{
	Toolbox::printDebugMessage("cover");
	sf::Vector2f goalPosition = sf::Vector2f(100.0f, 100.0f);
	//sf::Vector2f distance = sf::Vector2f(100.0f, 100.0f);
	float distance = 2000000.0f;
	std::pair<sf::Vector2f, float> pair = Toolbox::findGoalSquare(terrainManager, this); //MAY BE AN ISSUE WITH THIS "THIS"

	goalPosition = pair.first;
	distance = pair.second;
}

void Soldier::setLeader(bool toSet)
{
	isLeader = toSet;

	if (isLeader == true)
	{
		shape.setFillColor(sf::Color::Yellow);
	}
	else if(isLeader == false)
	{
		shape.setFillColor(sf::Color::Green);
	}
}

bool Soldier::getLeader()
{
	return isLeader;
}

void Soldier::findCoverTogether(TerrainManager* terrainManager, Soldier* leader)
{
	if (this->getLeader())
	{
		findCover(terrainManager);
	}
	else
	{
		sf::Vector2f goalPosition = sf::Vector2f(100.0f, 100.0f);
		//sf::Vector2f distance = sf::Vector2f(100.0f, 100.0f);
		float distance = 2000000.0f;

		std::pair<sf::Vector2f, float> pair = Toolbox::findGoalSquare(terrainManager, this); //MAY BE AN ISSUE WITH THIS "THIS"
		
		goalPosition = pair.first;
		distance = pair.second;
	}
}

void Soldier::generateMapToGoal(sf::Vector2f goalPos, TerrainManager* terrainManager)
{

	map.resize(terrainManager->terrainSquares.size());

	for (auto iter = 0; terrainManager->terrainSquares.size() > iter; iter++)
	{
		long newDistanceX = terrainManager->terrainSquares[iter]->shape.getPosition().x - goalPos.x;
		long newDistanceY = terrainManager->terrainSquares[iter]->shape.getPosition().y - goalPos.y;

		newDistanceX = newDistanceX * newDistanceX;
		newDistanceY = newDistanceY * newDistanceY;

		long newDistance = newDistanceX + newDistanceY;
		newDistance = newDistance / 10;

		//if (terrainManager->terrainSquares[iter]->getIsPassable() == false || terrainManager->terrainSquares[iter]->getIsOccupied() == true)
		//{
		//	newDistance = LONG_MAX;
		//}
		//if (newDistance > LONG_MAX)
		//{
		//	newDistance = newDistance - 1;
		//}

		map[iter].first = iter;
		map[iter].second = newDistance;
	}

	mapGenerated = true;
}

void Soldier::pathFindToGoal(sf::Vector2f goalPos, TerrainManager* terrainManager)
{
	if (goalPos.x == NULL && goalPos.y == NULL)
	{

	}
	else
	{
		int currentPosition;
		int currentUp;
		int currentDown;
		int currentLeft;
		int currentRight;

		pathfinderPosition = shape.getPosition();

		for (auto iter = 0; terrainManager->terrainSquares.size() > iter; iter++)
		{
			if (terrainManager->terrainSquares[iter]->shape.getPosition() == pathfinderPosition)
			{
				currentPosition = iter; //40 down 1 accross
				currentRight = iter + 1;
				currentLeft = iter - 1;
				currentDown = iter + 40;
				currentUp = iter - 40;

				//fudge for the issue with reading off grid
				if (currentUp < 0)
				{
					currentUp = 0;
				}

				if (currentDown > 1199)
				{
					currentDown = 1199;
				}

				if (currentRight > 1199)
				{
					currentRight = 1199;
				}

				if (currentLeft < 0)
				{
					currentLeft = 0;
				}
			}
		}

			//while (pathfinderPosition != goalPos)
			//{
			int leastDistance;

			int tempArray[4] = { map[currentUp].second, map[currentDown].second,
				map[currentLeft].second, map[currentRight].second };
			int temp = 2147483647;

			for (int iter = 0; iter <= 3; iter++)
			{
				if (tempArray[iter] < temp)
				{
					temp = tempArray[iter];
				}
			}

			if (temp == map[currentDown].second)
			{
				addCommandToList("moveDown");
				pathfinderPosition.y = pathfinderPosition.y + 20.0f;
				//map[currentDown].second = map[currentDown].second + 100;
			}
			else if (temp == map[currentLeft].second)
			{
				addCommandToList("moveLeft");
				pathfinderPosition.x = pathfinderPosition.x - 20.0f;
				//map[currentLeft].second = map[currentLeft].second + 100;
			}
			else if (temp == map[currentRight].second)
			{
				addCommandToList("moveRight");
				pathfinderPosition.x = pathfinderPosition.x + 20.0f;
				//map[currentRight].second = map[currentRight].second + 100;
			}
			else if (temp == map[currentUp].second)
			{
				addCommandToList("moveUp");
				pathfinderPosition.y = pathfinderPosition.y - 20.0f;
				//map[currentUp].second = map[currentUp].second + 100;
			}
			else
			{
				Toolbox::printDebugMessage("SomethingWentWrong");
			}
			//}
		}
	mapGenerated = false;
	//map.clear();
}

void Soldier::calculateBraveryRating() //need to run this at setup to assign bravery ratings
{
	braveryRating = rand() % 6 + 6;

	if (isLeader)
	{
		braveryRating = braveryRating + 2;
	}
	
	actualBraveryRating = braveryRating;
}

void Soldier::setPosition(sf::Vector2f positionToSet)
{
	position = positionToSet;
}

sf::Vector2f Soldier::getPosition()
{
	return position;
}

void Soldier::moveUp(TerrainManager* terrainManager)
{
	if (position.y == 0.0f)
	{
	}
	else
	{
		position.y = position.y - 20.0f;
		shape.setPosition(sf::Vector2f(position.x, position.y));

		for (int iter = 0; iter < terrainManager->terrainSquares.size(); iter++) //changes the occupied tag on the terrain
		{
			if (terrainManager->terrainSquares[iter]->shape.getPosition() == shape.getPosition())
			{
				//terrainManager->terrainSquares[iter]->setIsOccupied(true);
				//terrainManager->terrainSquares[iter]->shape.setFillColor(sf::Color::Magenta);

				if (terrainManager->terrainSquares[iter]->getIsCover())
				{
					setIsInCover(true);
				}
				else
				{
					setIsInCover(false);
				}
			}

			if (terrainManager->terrainSquares[iter]->shape.getPosition().y == shape.getPosition().y - 20.0f)
			{
				terrainManager->terrainSquares[iter]->setIsOccupied(false);
				//terrainManager->terrainSquares[iter]->shape.setFillColor(sf::Color::Blue);
			}
		}
	}
}

void Soldier::moveDown(TerrainManager* terrainManager)
{
	if (position.y == 580.0f)
	{

	}
	else
	{
		position.y = position.y + 20.0f;
		shape.setPosition(sf::Vector2f(position.x, position.y));


		for (int iter = 0; iter < terrainManager->terrainSquares.size(); iter++)
		{
			if (terrainManager->terrainSquares[iter]->shape.getPosition() == shape.getPosition())
			{
				//terrainManager->terrainSquares[iter]->setIsOccupied(true);
				//terrainManager->terrainSquares[iter]->shape.setFillColor(sf::Color::Magenta);

				if (terrainManager->terrainSquares[iter]->getIsCover())
				{
					setIsInCover(true);
				}
				else
				{
					setIsInCover(false);
				}
			}

			if (terrainManager->terrainSquares[iter]->shape.getPosition().y == shape.getPosition().y + 20.0f)
			{
				terrainManager->terrainSquares[iter]->setIsOccupied(false);
				//terrainManager->terrainSquares[iter]->shape.setFillColor(sf::Color::Blue);
			}
		}
	}
}

void Soldier::moveLeft(TerrainManager* terrainManager)
{
	if (position.x == 0.0f)
	{

	}
	else
	{
		position.x = position.x - 20.0f;
		shape.setPosition(sf::Vector2f(position.x, position.y));

		for (int iter = 0; iter < terrainManager->terrainSquares.size(); iter++)
		{
			if (terrainManager->terrainSquares[iter]->shape.getPosition() == shape.getPosition())
			{
				//terrainManager->terrainSquares[iter]->setIsOccupied(true);
				//terrainManager->terrainSquares[iter]->shape.setFillColor(sf::Color::Magenta);

				if (terrainManager->terrainSquares[iter]->getIsCover())
				{
					setIsInCover(true);
				}
				else
				{
					setIsInCover(false);
				}
			}

			if (terrainManager->terrainSquares[iter]->shape.getPosition().x == shape.getPosition().x - 20.0f)
			{
				terrainManager->terrainSquares[iter]->setIsOccupied(false);
				//terrainManager->terrainSquares[iter]->shape.setFillColor(sf::Color::Blue);
			}
		}
	}
}

void Soldier::moveRight(TerrainManager* terrainManager)
{
	if (position.x == 780.0f)
	{

	}
	else
	{
		position.x = position.x + 20.0f;
		shape.setPosition(sf::Vector2f(position.x, position.y));

		for (int iter = 0; iter < terrainManager->terrainSquares.size(); iter++)
		{
			if (terrainManager->terrainSquares[iter]->shape.getPosition() == shape.getPosition())
			{
				//terrainManager->terrainSquares[iter]->setIsOccupied(true);
				//terrainManager->terrainSquares[iter]->shape.setFillColor(sf::Color::Magenta);

				if (terrainManager->terrainSquares[iter]->getIsCover())
				{
					setIsInCover(true);
				}
				else
				{
					setIsInCover(false);
				}
			}

			if (terrainManager->terrainSquares[iter]->shape.getPosition().x == shape.getPosition().x + 20.0f)
			{
				terrainManager->terrainSquares[iter]->setIsOccupied(false);
				//terrainManager->terrainSquares[iter]->shape.setFillColor(sf::Color::Blue);
			}
		}
	}
}

void Soldier::setIsInCover(bool toSet)
{
	inCover = toSet;
}

bool Soldier::getIsInCover()
{
	return inCover;
}

std::string Soldier::getOrder()
{
	return currentOrder;
}

void Soldier::setOrder(std::string orderToSet)
{
	commandList.insert(commandList.begin(), orderToSet);
}

int Soldier::getState()
{
	return state;
}

void Soldier::setState(SoldierStates stateToSet)
{
	state = stateToSet;
}

void Soldier::setHealth(int healthToSet)
{
	health = healthToSet;
}

int Soldier::getHealth()
{
	return health;
}

void Soldier::reduceHealth()
{
	health = health - 1;
}

void Soldier::hunkerDown()
{
	Toolbox::printDebugMessage("HunkerDown");
}

bool Soldier::getGettingShotAt()
{
	return gettingShotAt;
}

void Soldier::setGettingShotAt(bool isGettingShotAt)
{
	gettingShotAt = isGettingShotAt;
}

bool Soldier::getShooting()
{
	return shooting;
}

void Soldier::setShooting(bool isShooting)
{
	shooting = isShooting;
}

bool Soldier::getFleeing()
{
	return fleeing;
}

void Soldier::setFleeing(bool isFleeing)
{
	fleeing = isFleeing;
}

bool Soldier::getLeaderIsDead()
{
	return leaderIsDead;
}

void Soldier::setLeaderIsDead(bool isLeaderDead)
{
	leaderIsDead = isLeaderDead;
}

void Soldier::setCurrentTarget(Soldier* targetToSet)
{
	target = targetToSet;
}

Soldier* Soldier::getCurrentTarget()
{
	return target;
}

void Soldier::setActualBraveryRating(int braveryToSet)
{
	actualBraveryRating = braveryToSet;
}

int Soldier::getActualBraveryRating()
{
	return actualBraveryRating;
}

void Soldier::setBraveryRating(int braveryToSet)
{
	braveryRating = braveryToSet;
}

int Soldier::getBraveryRating()
{
	return braveryRating;
}

Weapon* Soldier::getWeapon()
{
	return equippedWeapon;
}

void Soldier::setUpWeapon(WeaponTypes loadout)
{
	equippedWeapon->setWeaponType(loadout);
	equippedWeapon->setupAmmoCapacity();
}

void Soldier::setInRange(bool toSet)
{
	inRange = toSet;
}

bool Soldier::getInRange()
{
	return inRange;
}