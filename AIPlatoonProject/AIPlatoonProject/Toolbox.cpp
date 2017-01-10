#include "Toolbox.h"

Toolbox::Toolbox()
{

}

Toolbox::~Toolbox()
{

}

void Toolbox::clearSpace()
{
	for (int iter = 0; iter < 100; iter++)
	{
		std::cout << std::endl;
	}
	
}

void Toolbox::printDebugMessage(std::string message)
{
	std::cout << message << std::endl;
}

void Toolbox::printDebugMessage(int message)
{
	std::cout << message << std::endl;
}

void Toolbox::printDebugMessage(sf::Vector2f message)
{
	float tempx = message.x;
	float tempy = message.y;

	std::cout << "X is " << tempx << std::endl;
	std::cout << "Y is " << tempy << std::endl;
}

int Toolbox::addAHundred(int toMultiply)
{
	toMultiply = toMultiply + 100;

	return toMultiply;
}

int Toolbox::addATwenty(int toAdd)
{
	toAdd = toAdd + 20;

	return toAdd;
}

sf::Vector2f Toolbox::findMidPoint(sf::Vector2f firstVector, sf::Vector2f secondVector)
{

	sf::Vector2f midpoint = firstVector + secondVector;

	//midpoint.x = midpoint.x / 2;
	//midpoint.y = midpoint.y / 2;

	return midpoint;
}

float Toolbox::findDistanceOfEnemies(Platoon* enemyPlatoon, Soldier* self)
{
	float distance = 20000000.0f;
	
	for (auto iter2 = 0; enemyPlatoon->platoonSections.size() > iter2; iter2++)
	{
		for (auto iter = 0; enemyPlatoon->platoonSections[iter2]->soldiers.size() > iter; iter++)
		{
			if (enemyPlatoon->platoonSections[iter2]->soldiers[iter]->getState() == aliveAndWell)
			{
				float newDistanceX = enemyPlatoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition().x - self->getPosition().x;
				float newDistanceY = enemyPlatoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition().y - self->getPosition().y;

				newDistanceX = newDistanceX * newDistanceX;
				newDistanceY = newDistanceY * newDistanceY;

				float newDistance = newDistanceX + newDistanceY;

				//newDistance = newDistance / newDistance;

				//sf::Vector2f newDistance = this->getPosition() - terrainManager->terrainSquares[iter]->shape.getPosition();
				if (newDistance < distance)
				{
					//target = enemyPlatoon->soldiers[iter];
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

	return distance;
}

std::pair<float, Soldier*> Toolbox::findDistanceOfEnemiesAndTarget(Platoon* enemyPlatoon, Soldier* self)
{
	float distance = 20000000.0f;
	Soldier* target = nullptr;
	//for(auto iter = 0; enemyPlatoon->soldiers.size() > iter; iter++)

	for (auto iter2 = 0; enemyPlatoon->platoonSections.size() > iter2; iter2++)
	{
		for (auto iter = 0; enemyPlatoon->platoonSections[iter2]->soldiers.size() > iter; iter++)
		{
			if (enemyPlatoon->platoonSections[iter2]->soldiers[iter]->getState() == aliveAndWell)
			{
				float newDistanceX = enemyPlatoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition().x - self->getPosition().x;
				float newDistanceY = enemyPlatoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition().y - self->getPosition().y;

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
	std::pair<float, Soldier*> pair;
	pair.first = distance;
	pair.second = target;
	return pair;
}

std::pair<sf::Vector2f, float> Toolbox::findGoalSquare(TerrainManager* terrainManager, Soldier* self)
{
	sf::Vector2f goalPosition = sf::Vector2f(100.0f, 100.0f);
	//sf::Vector2f distance = sf::Vector2f(100.0f, 100.0f);
	float distance = 2000000.0f;

	for (auto iter = 0; terrainManager->terrainSquares.size() > iter; iter++)
	{
		if (terrainManager->terrainSquares[iter]->getIsCover() && !terrainManager->terrainSquares[iter]->getGoal())
		{
			float newDistanceX = terrainManager->terrainSquares[iter]->shape.getPosition().x - self->getPosition().x;
			float newDistanceY = terrainManager->terrainSquares[iter]->shape.getPosition().y - self->getPosition().y;

			newDistanceX = newDistanceX * newDistanceX;
			newDistanceY = newDistanceY * newDistanceY;

			float newDistance = newDistanceX + newDistanceY;

			//newDistance = newDistance / newDistance;

			//sf::Vector2f newDistance = this->getPosition() - terrainManager->terrainSquares[iter]->shape.getPosition();
			if (newDistance < distance)
			{
				goalPosition = terrainManager->terrainSquares[iter]->shape.getPosition();
				distance = newDistance;
				terrainManager->setGoalSquare(iter);
				self->goalSquare = iter;
			}
			else
			{

			}
		}
	}

	std::pair<sf::Vector2f, float> pair;
	pair.first = goalPosition;
	pair.second = distance;
	return pair;
}