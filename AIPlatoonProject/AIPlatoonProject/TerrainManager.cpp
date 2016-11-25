#include "TerrainManager.h"

TerrainManager::TerrainManager()
{
}

TerrainManager::~TerrainManager()
{
	terrainSquares.clear();
}

void TerrainManager::setUpTerrainSquares()
{
	srand(time(NULL));
	float x = 0;
	float y = 0;


	for (int iter = 0; iter < 1200; iter++)
	{
		Terrain* temp = new Terrain;
		temp->setSpawn(false);
		int isItPassable = rand() % 100 + 1;

		if (isItPassable >= 95)
		{
			temp->setIsPassable(false);
			temp->setIsCover(false);
			temp->shape.setFillColor(sf::Color::Red);
		}
		else
		{
			temp->setIsPassable(true);


			int isItCover = rand() % 100 + 1;

			if (isItCover >= 99)
			{
				temp->setIsCover(true);
				temp->shape.setFillColor(sf::Color::Blue);
			}
			else
			{
				temp->setIsCover(false);
				temp->shape.setFillColor(sf::Color::Magenta);
			}
		}
		temp->setIsOccupied(false);

		temp->shape.setSize(sf::Vector2f(20.0f, 20.0f));
		temp->shape.setPosition(x, y);
		
		//temp->shape.setOutlineColor(sf::Color::Blue);
		terrainSquares.push_back(temp);
		x = Toolbox::addATwenty(x);

		if (x == 800)
		{
			x = 0;
			y = Toolbox::addATwenty(y);
		}

		//delete temp;
	}

	/*bool settingGoal = true;

	while (settingGoal == true)
	{
		goalSquare = rand() % 1199;

		if (terrainSquares[goalSquare]->getIsPassable() == true)
		{
			terrainSquares[goalSquare]->setGoal(true);
			settingGoal = false;
		}
		else
		{

		}
	}*/

	int settingSpawnTest = 0;

	while (settingSpawnTest != 2)
	{
		int randomSpawn = rand() % 1199;

		if (terrainSquares[randomSpawn]->getIsPassable() == true && terrainSquares[randomSpawn]->getIsCover() == false
			&& terrainSquares[randomSpawn]->getSpawn() == false)
		{
			terrainSquares[randomSpawn]->setSpawn(true);
			settingSpawnTest++;
		}
		else
		{

		}
	}
	
}

void TerrainManager::setGoalSquare(int squareToSet)
{
	terrainSquares[squareToSet]->setGoal(false);
	//goalSquare = squareToSet;
	terrainSquares[squareToSet]->setGoal(true);
}

void TerrainManager::removeGoalFromSquare(int squareToSet)
{
	terrainSquares[squareToSet]->setGoal(false);
}


/*int TerrainManager::getGoalSquare()
{
	return goalSquare;
}*/