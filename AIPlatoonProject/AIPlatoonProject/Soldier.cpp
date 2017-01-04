#include "Soldier.h"

Soldier::Soldier()
{
	state = aliveAndWell;
	isLeader = false;
	mapGenerated = false;
	setHealth(20);
	needsToMove = false;
}

Soldier::~Soldier()
{
	delete target;
}

void Soldier::soldierThink(SoldierData _SD)
{
	//if all of these are false no sensible soldier would not obey a direct order
	if (gettingShotAt == false && shooting == false && fleeing == false && leaderIsDead == false) 
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
		//executeCommand(terrainManager, leader);
	}


	clearCommandList();
}

void Soldier::interpretOrders(PlatoonSection* enemyPlatoon)
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

		}
		//currentOrder = "findCover";
	}
	else
	{
		bool validTarget = false;
		//check to see if there's anything to shoot at
		for (auto iter = 0; iter < enemyPlatoon->soldiers.size(); iter++)
		{
			if (enemyPlatoon->soldiers[iter]->getState() == aliveAndWell)
			{
				validTarget = true;
				break;
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

void Soldier::advance(PlatoonSection* enemyPlatoon, TerrainManager* terrainManager, Soldier* leader)
{
	Toolbox::printDebugMessage("Forward");
	target = nullptr;
	float distance = 20000000.0f;
	for (auto iter = 0; enemyPlatoon->soldiers.size() > iter; iter++)
	{
		if (enemyPlatoon->soldiers[iter]->getState() == aliveAndWell)
		{
			float newDistanceX = enemyPlatoon->soldiers[iter]->shape.getPosition().x - this->getPosition().x;
			float newDistanceY = enemyPlatoon->soldiers[iter]->shape.getPosition().y - this->getPosition().y;

			newDistanceX = newDistanceX * newDistanceX;
			newDistanceY = newDistanceY * newDistanceY;

			float newDistance = newDistanceX + newDistanceY;

			//newDistance = newDistance / newDistance;

			//sf::Vector2f newDistance = this->getPosition() - terrainManager->terrainSquares[iter]->shape.getPosition();
			if (newDistance < distance)
			{
				target = enemyPlatoon->soldiers[iter];
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

	sf::Vector2f midpoint = Toolbox::findMidPoint(this->getPosition(), target->getPosition());

	clearCommandList();
	generateMapToGoal(midpoint, terrainManager);
	pathFindToGoal(midpoint, terrainManager);
	executeCommand(terrainManager, leader, enemyPlatoon);
	//clearCommandList();
}

void Soldier::checkRange(PlatoonSection* enemyPlatoon)
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

void Soldier::shoot(PlatoonSection* enemyPlatoon)
{
	//pew pew
	bool hasTargetInRange = false;

	std::pair<float, Soldier*> pair = Toolbox::findDistanceOfEnemiesAndTarget(enemyPlatoon, this);

	float distance = pair.first;
	Soldier* target = pair.second;

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
	

	if (hasTargetInRange == true)
	{
		int hit = rand() % 2 + 1;
		Toolbox::printDebugMessage("Pew");
		if (hit == 1)//hit
		{
			Toolbox::printDebugMessage("Hit");
			//target->gettingShotAt = true;
			target->reduceHealth();

			if (target->getHealth() <= 0)
			{
				target->setState(dead);
				target->shape.setFillColor(sf::Color::Black);
			}
		}
		else if (hit == 2)//miss
		{
			Toolbox::printDebugMessage("Miss");
		}

		hasTargetInRange = false;
	}
	else if(hasTargetInRange == false)
	{
		inRange = false;
	}
	else
	{
		Toolbox::printDebugMessage("Hunkerdown");
	}
	//delete target;
}


void Soldier::executeCommand(TerrainManager* terrainManager, Soldier* leader, PlatoonSection* enemyPlatoon)
{
	if (mapGenerated == false)
	{
		generateMapToGoal(terrainManager->terrainSquares[goalSquare]->shape.getPosition(), terrainManager);
	}
	if (commandList.size() > 0)
	{

		if (commandList.front() == "moveUp")
		{
			moveUp(terrainManager);
		}
		else if (commandList.front() == "moveDown")
		{
			moveDown(terrainManager);
		}
		else if (commandList.front() == "moveLeft")
		{
			moveLeft(terrainManager);
		}
		else if (commandList.front() == "moveRight")
		{
			moveRight(terrainManager);
		}
		else if (commandList.front() == "findCover")
		{
			findCover(terrainManager);
		}
		else if (commandList.front() == "findCoverTogether")
		{
			findCoverTogether(terrainManager, leader);
		}
		else if (commandList.front() == "fire")
		{
			shoot(enemyPlatoon);
		}
		else if (commandList.front() == "hunkerDown")
		{
			hunkerDown();
		}
		else if (commandList.front() == "advance")
		{
			advance(enemyPlatoon, terrainManager, leader);
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
	commandList.clear();
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

//OLD PATHFINDING STUFF

/*xOrY = rand() % 2 + 1;
for (auto iter = 0; iter != terrainManager->terrainSquares.size(); iter++) //this gets the current position in the array
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

//xOrY = rand() % 2 + 1;
resolveIfStuck(terrainManager, currentUp, currentDown, currentLeft, currentRight);

if (pathfinderPosition.y == goalPos.y)
{
xOrY = 2;
}
else if (pathfinderPosition.x == goalPos.x)
{
xOrY = 1;
}
else
{

}

if (xOrY == 1)
{
//move up or down
if ((pathfinderPosition.y >= goalPos.y) && terrainManager->terrainSquares[currentUp]->getIsPassable() == true)
{
addCommandToList("moveUp");
pathfinderPosition.y = pathfinderPosition.y - 20.0f;

}
else if ((pathfinderPosition.y <= goalPos.y) && terrainManager->terrainSquares[currentDown]->getIsPassable() == true)
{
addCommandToList("moveDown");
pathfinderPosition.y = pathfinderPosition.y + 20.0f;

}
else
{
Toolbox::printDebugMessage("MoveY was called when Y for goal and position are the same");
}
}
else if (xOrY == 2)
{
//move right or left
if ((pathfinderPosition.x >= goalPos.x) && terrainManager->terrainSquares[currentLeft]->getIsPassable() == true)
{
addCommandToList("moveLeft");
pathfinderPosition.x = pathfinderPosition.x - 20.0f;
}
else if (pathfinderPosition.x <= goalPos.x && terrainManager->terrainSquares[currentRight]->getIsPassable() == true)
{
addCommandToList("moveRight");
pathfinderPosition.x = pathfinderPosition.x + 20.0f;
}
else
{
Toolbox::printDebugMessage("MoveX was called when X for goal and position are the same");
}
}
else
{
Toolbox::printDebugMessage("Decision to move on X or Y failed");
}*/