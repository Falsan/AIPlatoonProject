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

	readInFile();

	for (int iter = 0; iter < 1200; iter++)
	{
		Terrain* temp = new Terrain;

		if (terrainValues[iter] == 0)
		{
			temp->setIsCover(false);
			temp->setIsPassable(true);
			temp->setIsOccupied(false);
			temp->setGoal(false);
			temp->setSpawn(false);
			temp->shape.setFillColor(sf::Color(0, 100, 0, 255));
		}
		else if (terrainValues[iter] == 1)
		{
			temp->setIsCover(true);
			temp->setIsPassable(true);
			temp->setIsOccupied(false);
			temp->setGoal(false);
			temp->setSpawn(false);
			temp->shape.setFillColor(sf::Color(0, 0, 100, 255));
		}
		else if (terrainValues[iter] == 2)
		{
			temp->setIsCover(false);
			temp->setIsPassable(false);
			temp->setIsOccupied(false);
			temp->setGoal(false);
			temp->setSpawn(false);
			temp->shape.setFillColor(sf::Color::Red);
		}
		else if (terrainValues[iter] == 3)
		{
			temp->setIsCover(false);
			temp->setIsPassable(true);
			temp->setIsOccupied(true);
			temp->setGoal(false);
			temp->setSpawn(true);
			temp->shape.setFillColor(sf::Color(0, 100, 0, 255));
		}
		else
		{
			Toolbox::printDebugMessage("Terrain generated wrong. Square:");
			Toolbox::printDebugMessage(iter);
			Toolbox::printDebugMessage("was responsible...");
			temp->shape.setFillColor(sf::Color::Green);
		}
		temp->shape.setSize(sf::Vector2f(20.0f, 20.0f));
		temp->shape.setPosition(x, y);
		terrainSquares.push_back(temp);

		x = Toolbox::addATwenty(x);

		if (x == 800)
		{
			x = 0;
			y = Toolbox::addATwenty(y);
		}
	}
	
}

void TerrainManager::randomGen(float& x, float& y)
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

		if (isItCover >= 90)
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

while (settingSpawnTest != 9)
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


void TerrainManager::readInFile()
{
	std::ifstream infile;

	infile.open("Gridbase.txt");

	for (int iter = 0; iter < 1200; iter++)
	{
		int temp = -5;

		infile >> temp;
			
		terrainValues.push_back(temp);
	}

	infile.close();
}

void TerrainManager::setGoalSquare(int squareToSet)
{
	//terrainSquares[squareToSet]->setGoal(false);
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