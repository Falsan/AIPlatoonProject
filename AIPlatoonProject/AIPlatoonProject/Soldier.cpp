#include "Soldier.h"

Soldier::Soldier()
{
	state = aliveAndWell;
	isLeader = false;
	mapGenerated = false;
	setHealth(20);
	needsToMove = false;
	leaderIsDead = false;

}

Soldier::~Soldier()
{
	delete target;
}

void Soldier::soldierThink(SoldierData _SD)
{
	if (state == panicked)
	{
		setFleeing(true);
		int random = rand() % 3 + 1;
		if (random == 1)
		{
			flee(_SD);
		}
		else
		{
			state = aliveAndWell;
			actualBraveryRating++;
		}
		
		actualBraveryRating++;
	}
	else
	{
		if (gettingShotAt == true)
		{
			actualBraveryRating--;
			actualBraveryRating--;
		}
		if (shooting == true)
		{
			actualBraveryRating--;
		}
		if (leaderIsDead == true)
		{
			actualBraveryRating--;
		}

		if (actualBraveryRating < 10)
		{

		}
		else
		{
			actualBraveryRating++;
		}

		//if all of these are false no sensible soldier would not obey a direct order
		if (gettingShotAt == false && shooting == false && fleeing == false && leaderIsDead == false)
		{
			soldierAct(_SD);

		}
		else
		{
			if (actualBraveryRating < 4)
			{
				panicCheck(_SD);
			}
			else
			{
				soldierAct(_SD);
			}
		}
	}
	clearCommandList();
	setGettingShotAt(false);
}

void Soldier::panicCheck(SoldierData _SD)
{
	int random = rand() % 100 + 1;

	if (random > 75)
	{
		soldierPanic(_SD);
	}
	else
	{
		actualBraveryRating++;
		soldierAct(_SD);
	}
}

void Soldier::soldierAct(SoldierData _SD)
{
	if (currentOrder == "")
	{
		interpretOrders(_SD.enemyPlatoon);
		//currentOrder = "findCover";
		//commandList.push_back("findCover");
	}

	if (commandList.size() > 0)
	{
		pathFindToGoal(_SD.m_terrainManager->terrainSquares[goalSquare]->shape.getPosition(), _SD.m_terrainManager);
		executeCommand(_SD.m_terrainManager, _SD.m_leader, _SD.enemyPlatoon);
	}
	else if (commandList.size() == 0)
	{
		if (mapGenerated == false)
		{
			generateMapToGoal(_SD.m_terrainManager->terrainSquares[goalSquare]->shape.getPosition(), _SD.m_terrainManager);
		}
		pathFindToGoal(_SD.m_terrainManager->terrainSquares[goalSquare]->shape.getPosition(), _SD.m_terrainManager);
		executeCommand(_SD.m_terrainManager, _SD.m_leader, _SD.enemyPlatoon);
	}
}

void Soldier::interpretOrders(Platoon* enemyPlatoon)
{
	if (inCover == false)
	{
		if (needsToMove == false)
		{
			commandList.push_back("findCover");
			needsToMove = true;
		}
		else
		{
			if (prevCommandList.size() > 0)
			{
				commandList.push_back(prevCommandList.front());
			}
			needsToMove = false;
		}
		//currentOrder = "findCover";
	}
	else
	{
		needsToMove = false;
		bool validTarget = false;
		//check to see if there's anything to shoot at
		for (auto iter2 = 0; iter2 != enemyPlatoon->platoonSections.size(); iter2++) 
		{
			for (auto iter = 0; iter < enemyPlatoon->platoonSections[iter2]->soldiers.size(); iter++)
			{
				if (enemyPlatoon->platoonSections[iter2]->soldiers[iter]->getState() == aliveAndWell)
				{
					validTarget = true;
					break;
				}
			}
		}
		if (validTarget == true)
		{
			checkRange(enemyPlatoon);
			if (inRange == true)
			{
				commandList.push_back("fire");
			}
			else
			{
				commandList.push_back("advance");
			}
		}
		else
		{
			commandList.push_back("hunkerDown");
		}
	}
}

void Soldier::flank(SoldierData _SD)
{
	std::pair<float, Soldier*> pair = Toolbox::findDistanceOfEnemiesAndTarget(_SD.enemyPlatoon ,this);

	float distance = pair.first; //need to get the x and y values of distance for moving around the enemy
	target = pair.second;
}

void Soldier::soldierPanic(SoldierData _SD)
{
	Toolbox::printDebugMessage("Panic");
	state = panicked;
}

void Soldier::flee(SoldierData _SD)
{
	Toolbox::printDebugMessage("Flee");
	
	addCommandToList("hunkerDown");

	soldierAct(_SD);
}

void Soldier::advance(Platoon* enemyPlatoon, TerrainManager* terrainManager, Soldier* leader)
{
	Toolbox::printDebugMessage("Forward");
	//target = nullptr;
	float distance = 20000000.0f;
	for (auto iter2 = 0; iter2 != enemyPlatoon->platoonSections.size(); iter2++)
	{
		for (auto iter = 0; enemyPlatoon->platoonSections[iter2]->soldiers.size() > iter; iter++)
		{
			if (enemyPlatoon->platoonSections[iter2]->soldiers[iter]->getState() == aliveAndWell)
			{
				float newDistanceX = enemyPlatoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition().x - this->getPosition().x;
				float newDistanceY = enemyPlatoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition().y - this->getPosition().y;

				newDistanceX = newDistanceX * newDistanceX;
				newDistanceY = newDistanceY * newDistanceY;

				float newDistance = newDistanceX + newDistanceY;

				//newDistance = newDistance / newDistance;

				//sf::Vector2f newDistance = this->getPosition() - terrainManager->terrainSquares[iter]->shape.getPosition();
				if (newDistance < distance)
				{
					target = enemyPlatoon->platoonSections[iter2]->soldiers[iter];
					distance = newDistance;
					//terrainManager->setGoalSquare(iter);
					//goalSquare = iter;
				}
				else
				{

				}
			}
			else
			{

			}
		}
	}

	sf::Vector2f midpoint = Toolbox::findMidPoint(this->getPosition(), target->getPosition());

	clearCommandList();
	generateMapToGoal(midpoint, terrainManager);
	pathFindToGoal(midpoint, terrainManager);
	executeCommand(terrainManager, leader, enemyPlatoon);
	//clearCommandList();
}

void Soldier::checkRange(Platoon* enemyPlatoon)
{
	float distance = Toolbox::findDistanceOfEnemies(enemyPlatoon, this);

	if (distance < 100000.0f)
	{
		inRange = true;

		//delete target;
	}
	else
	{
		inRange = false;
	}
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
				advance(enemyPlatoon, terrainManager, leader);
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

		if (terrainManager->terrainSquares[iter]->getIsPassable() == false || terrainManager->terrainSquares[iter]->getIsOccupied() == true)
		{
			newDistance = 2147483647;
		}

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
				terrainManager->terrainSquares[iter]->setIsOccupied(true);

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
				terrainManager->terrainSquares[iter]->setIsOccupied(true);

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
				terrainManager->terrainSquares[iter]->setIsOccupied(true);

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
				terrainManager->terrainSquares[iter]->setIsOccupied(true);

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