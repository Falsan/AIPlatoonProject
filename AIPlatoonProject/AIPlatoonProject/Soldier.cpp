#include "Soldier.h"

Soldier::Soldier()
{
	
}

Soldier::~Soldier()
{

}

void Soldier::executeCommand(std::string command)
{
	if (command == "moveUp")
	{
		moveUp();
	}
	else if (command == "moveDown")
	{
		moveDown();
	}
	else if (command == "moveLeft")
	{
		moveLeft();
	}
	else if (command == "moveRight")
	{
		moveRight();
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

void Soldier::pathFindToGoal(sf::Vector2f goalPos, TerrainManager* terrainManager)
{
	int currentPosition;
	int currentUp;
	int currentDown;
	int currentLeft;
	int currentRight;

	int xOrY;

	pathfinderPosition = shape.getPosition();

	while (pathfinderPosition != goalPos)
	{
		xOrY = rand() % 2 + 1;
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

		if (/*shape.getPosition().y != goalPos.y && */xOrY == 1)
		{
			//move up or down
			if ((pathfinderPosition.y > goalPos.y) && terrainManager->terrainSquares[currentUp]->getIsPassable() == true)
			{
				addCommandToList("moveUp");
				pathfinderPosition.y = pathfinderPosition.y - 20.0f;
				
			}
			else if ((pathfinderPosition.y < goalPos.y) && terrainManager->terrainSquares[currentDown]->getIsPassable() == true)
			{
				addCommandToList("moveDown");
				pathfinderPosition.y = pathfinderPosition.y + 20.0f;
				
			}
			else
			{
				Toolbox::printDebugMessage("MoveY was called when Y for goal and position are the same");
			}
		}
		else if (/*shape.getPosition().x != goalPos.x && */xOrY == 2)
		{
			//move right or left
			if ((pathfinderPosition.x > goalPos.x) && terrainManager->terrainSquares[currentLeft]->getIsPassable() == true)
			{
				addCommandToList("moveLeft");
				pathfinderPosition.x = pathfinderPosition.x - 20.0f;
			}
			else if (pathfinderPosition.x < goalPos.x && terrainManager->terrainSquares[currentRight]->getIsPassable() == true)
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
		}
	}
	
}

void Soldier::resolveIfStuck(TerrainManager* terrainManager, int currentUp, int currentDown, int currentLeft, int currentRight)
{
	int randomMove = rand() % 4 + 1;

	if (terrainManager->terrainSquares[currentUp]->getIsPassable() == false && terrainManager->terrainSquares[currentRight]->getIsPassable() == false)
	{
		if (randomMove == 1)
		{
			addCommandToList("moveDown");
			pathfinderPosition.y = pathfinderPosition.y + 20.0f;
		}
		else if (randomMove == 2)
		{
			addCommandToList("moveLeft");
			pathfinderPosition.x = pathfinderPosition.x - 20.0f;
		}
		else if(randomMove == 3)
		{
			addCommandToList("moveLeft");
			pathfinderPosition.x = pathfinderPosition.x - 20.0f;
		}
		else if(randomMove == 4)
		{
			addCommandToList("moveDown");
			pathfinderPosition.y = pathfinderPosition.y + 20.0f;
		}
		else
		{
			Toolbox::printDebugMessage("Failed to initialise random move correctly");
		}
	}

	if (terrainManager->terrainSquares[currentUp]->getIsPassable() == false && terrainManager->terrainSquares[currentLeft]->getIsPassable() == false)
	{
		if (randomMove == 1)
		{
			addCommandToList("moveDown");
			pathfinderPosition.y = pathfinderPosition.y + 20.0f;
		}
		else if (randomMove == 2)
		{
			addCommandToList("moveRight");
			pathfinderPosition.x = pathfinderPosition.x + 20.0f;
		}
		else if (randomMove == 3)
		{
			addCommandToList("moveRight");
			pathfinderPosition.x = pathfinderPosition.x + 20.0f;
		}
		else if (randomMove == 4)
		{
			addCommandToList("moveDown");
			pathfinderPosition.y = pathfinderPosition.y + 20.0f;
		}
		else
		{
			Toolbox::printDebugMessage("Failed to initialise random move correctly");
		}
	}

	if (terrainManager->terrainSquares[currentDown]->getIsPassable() == false && terrainManager->terrainSquares[currentRight]->getIsPassable() == false)
	{
		if (randomMove == 1)
		{
			addCommandToList("moveUp");
			pathfinderPosition.y = pathfinderPosition.y - 20.0f;
		}
		else if (randomMove == 2)
		{
			addCommandToList("moveLeft");
			pathfinderPosition.x = pathfinderPosition.x - 20.0f;
		}
		else if (randomMove == 3)
		{
			addCommandToList("moveLeft");
			pathfinderPosition.x = pathfinderPosition.x - 20.0f;
		}
		else if (randomMove == 4)
		{
			addCommandToList("moveUp");
			pathfinderPosition.y = pathfinderPosition.y - 20.0f;
		}
		else
		{
			Toolbox::printDebugMessage("Failed to initialise random move correctly");
		}
	}

	if (terrainManager->terrainSquares[currentDown]->getIsPassable() == false && terrainManager->terrainSquares[currentLeft]->getIsPassable() == false)
	{
		if (randomMove == 1)
		{
			addCommandToList("moveUp");
			pathfinderPosition.y = pathfinderPosition.y - 20.0f;
		}
		else if (randomMove == 2)
		{
			addCommandToList("moveRight");
			pathfinderPosition.x = pathfinderPosition.x + 20.0f;
		}
		else if (randomMove == 3)
		{
			addCommandToList("moveRight");
			pathfinderPosition.x = pathfinderPosition.x + 20.0f;
		}
		else if (randomMove == 4)
		{
			addCommandToList("moveUp");
			pathfinderPosition.y = pathfinderPosition.y - 20.0f;
		}
		else
		{
			Toolbox::printDebugMessage("Failed to initialise random move correctly");
		}
	}

	if (terrainManager->terrainSquares[currentUp]->getIsPassable() == false)
	{
		if (randomMove == 1)
		{
			addCommandToList("moveDown");
			pathfinderPosition.y = pathfinderPosition.y + 20.0f;
		}
		else if (randomMove == 2)
		{
			addCommandToList("moveRight");
			pathfinderPosition.x = pathfinderPosition.x + 20.0f;
		}
		else if (randomMove == 3)
		{
			addCommandToList("moveLeft");
			pathfinderPosition.x = pathfinderPosition.x - 20.0f;
		}
		else if (randomMove == 4)
		{
			addCommandToList("moveDown");
			pathfinderPosition.y = pathfinderPosition.y + 20.0f;
		}
		else
		{
			Toolbox::printDebugMessage("Failed to initialise random move correctly");
		}
	}

	if (terrainManager->terrainSquares[currentDown]->getIsPassable() == false)
	{
		if (randomMove == 1)
		{
			addCommandToList("moveUp");
			pathfinderPosition.y = pathfinderPosition.y - 20.0f;
		}
		else if (randomMove == 2)
		{
			addCommandToList("moveRight");
			pathfinderPosition.x = pathfinderPosition.x + 20.0f;
		}
		else if (randomMove == 3)
		{
			addCommandToList("moveLeft");
			pathfinderPosition.x = pathfinderPosition.x - 20.0f;
		}
		else if (randomMove == 4)
		{
			addCommandToList("moveUp");
			pathfinderPosition.y = pathfinderPosition.y - 20.0f;
		}
		else
		{
			Toolbox::printDebugMessage("Failed to initialise random move correctly");
		}
	}

	if (terrainManager->terrainSquares[currentRight]->getIsPassable() == false)
	{
		if (randomMove == 1)
		{
			addCommandToList("moveUp");
			pathfinderPosition.y = pathfinderPosition.y - 20.0f;
		}
		else if (randomMove == 2)
		{
			addCommandToList("moveLeft");
			pathfinderPosition.x = pathfinderPosition.x - 20.0f;
		}
		else if (randomMove == 3)
		{
			addCommandToList("moveLeft");
			pathfinderPosition.x = pathfinderPosition.x - 20.0f;
		}
		else if (randomMove == 4)
		{
			addCommandToList("moveDown");
			pathfinderPosition.y = pathfinderPosition.y + 20.0f;
		}
		else
		{
			Toolbox::printDebugMessage("Failed to initialise random move correctly");
		}
	}

	if (terrainManager->terrainSquares[currentLeft]->getIsPassable() == false)
	{
		if (randomMove == 1)
		{
			addCommandToList("moveUp");
			pathfinderPosition.y = pathfinderPosition.y - 20.0f;
		}
		else if (randomMove == 2)
		{
			addCommandToList("moveRight");
			pathfinderPosition.x = pathfinderPosition.x + 20.0f;
		}
		else if (randomMove == 3)
		{
			addCommandToList("moveRight");
			pathfinderPosition.x = pathfinderPosition.x + 20.0f;
		}
		else if (randomMove == 4)
		{
			addCommandToList("moveDown");
			pathfinderPosition.y = pathfinderPosition.y + 20.0f;
		}
		else
		{
			Toolbox::printDebugMessage("Failed to initialise random move correctly");
		}
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

void Soldier::moveUp()
{
	if (position.y == 0.0f)
	{
	}
	else
	{
		position.y = position.y - 20.0f;
		shape.setPosition(sf::Vector2f(position.x, position.y));
	}
}

void Soldier::moveDown()
{
	if (position.y == 580.0f)
	{

	}
	else
	{
		position.y = position.y + 20.0f;
		shape.setPosition(sf::Vector2f(position.x, position.y));
	}
}

void Soldier::moveLeft()
{
	if (position.x == 0.0f)
	{

	}
	else
	{
		position.x = position.x - 20.0f;
		shape.setPosition(sf::Vector2f(position.x, position.y));
	}
}

void Soldier::moveRight()
{
	if (position.x == 780.0f)
	{

	}
	else
	{
		position.x = position.x + 20.0f;
		shape.setPosition(sf::Vector2f(position.x, position.y));
	}
}