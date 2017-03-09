#include "TacticsCodec.h"

TacticsCodec::TacticsCodec()
{
	currentOrder = "";
}

TacticsCodec::~TacticsCodec()
{

}

void TacticsCodec::think(SoldierData _SD, Soldier* self)
{
	if (self->getState() == panicked)
	{
		self->setFleeing(true);
		int random = rand() % 3 + 1;
		if (random == 1)
		{
			flee(_SD, self);
		}
		else
		{
			self->setState(aliveAndWell);
			self->setActualBraveryRating(self->getActualBraveryRating() + 1);
		}

		self->setActualBraveryRating(self->getActualBraveryRating() + 1);
	}
	else
	{
		if (self->getGettingShotAt() == true)
		{
			self->setActualBraveryRating(self->getActualBraveryRating() - 2);
		}
		if (self->getShooting() == true)
		{
			self->setActualBraveryRating(self->getActualBraveryRating() - 1);
		}
		if (self->getLeaderIsDead() == true)
		{
			self->setActualBraveryRating(self->getActualBraveryRating() - 1);
		}

		if (self->getActualBraveryRating() < 10)
		{

		}
		else
		{
			self->setActualBraveryRating(self->getActualBraveryRating() + 1);
		}

		//if all of these are false no sensible soldier would not obey a direct order
		if (self->getGettingShotAt() == false && self->getShooting() == false && self->getFleeing() == false && self->getLeaderIsDead() == false)
		{
			act(_SD, self);

		}
		else
		{
			if (self->getActualBraveryRating() < 4)
			{
				panicCheck(_SD, self);
			}
			else
			{
				act(_SD, self);
			}
		}
	}
	self->clearCommandList();
	self->setGettingShotAt(false);
}

void TacticsCodec::panicCheck(SoldierData _SD, Soldier* self)
{
	int random = rand() % 100 + 1;

	if (random > 75)
	{
		soldierPanic(_SD, self);
	}
	else
	{
		self->setActualBraveryRating(self->getActualBraveryRating() + 1);
		act(_SD, self);
	}
}

void TacticsCodec::act(SoldierData _SD, Soldier* self)
{
	Toolbox::getDistanceOfOfficer(_SD.m_platoonSection, self, _SD.m_terrainManager);
	//if (Toolbox::getDistanceOfOfficer(_SD.m_platoonSection, self, _SD.m_terrainManager))
	//{
		//Toolbox::printDebugMessage("hit");
	//}
	//else
	//{
		if (currentOrder == "")
		{
			interpretOrders(_SD.enemyPlatoon, self);
		}
		else if (currentOrder == "attack")
		{
			checkRange(_SD.enemyPlatoon, self);
			if (self->getInRange() == true)
			{
				self->commandList.push_back("fire");
			}
			else
			{
				self->commandList.push_back("advance");
			}

		}
		else if (currentOrder == "defend")
		{
			if (self->getIsInCover() == false)
			{

			}
		}

		if (self->commandList.size() > 0)
		{
			self->pathFindToGoal(_SD.m_terrainManager->terrainSquares[self->goalSquare]->shape.getPosition(), _SD.m_terrainManager);
			self->executeCommand(_SD.m_terrainManager, _SD.m_leader, _SD.enemyPlatoon);
		}
		else if (self->commandList.size() == 0)
		{
			if (self->mapGenerated == false)
			{
				self->generateMapToGoal(_SD.m_terrainManager->terrainSquares[self->goalSquare]->shape.getPosition(), _SD.m_terrainManager);
			}
			self->pathFindToGoal(_SD.m_terrainManager->terrainSquares[self->goalSquare]->shape.getPosition(), _SD.m_terrainManager);
			self->executeCommand(_SD.m_terrainManager, _SD.m_leader, _SD.enemyPlatoon);
		}
	//}
}

void TacticsCodec::interpretOrders(Platoon* enemyPlatoon, Soldier* self)
{
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
		checkRange(enemyPlatoon, self);
		if (self->getInRange() == true)
		{
			self->commandList.push_back("fire");
		}
		else
		{
			int random = rand() % 2 + 1;
			if (random == 1)
			{
				self->commandList.push_back("advance");
			}
			else if (random == 2)
			{
				checkForCover(self);
			}
		}
	}

}

void TacticsCodec::checkForCover(Soldier* self)
{
	if (self->getIsInCover() == false)
	{
		if (self->needsToMove == false)
		{
			self->commandList.push_back("findCover");
			self->needsToMove = true;
		}
		else
		{
			if (self->prevCommandList.size() > 0)
			{
				self->commandList.push_back(self->prevCommandList.front());
			}
			self->needsToMove = false;
		}
		//currentOrder = "findCover";
	}
	else
	{
		self->commandList.push_back("hunkerDown");
		self->needsToMove = false;
	}
}

void TacticsCodec::flank(SoldierData _SD, Soldier* self)
{
	std::pair<float, Soldier*> pair = Toolbox::findDistanceOfEnemiesAndTarget(_SD.enemyPlatoon, self);

	float distance = pair.first; //need to get the x and y values of distance for moving around the enemy
	self->setCurrentTarget(pair.second);
}

void TacticsCodec::soldierPanic(SoldierData _SD, Soldier* self)
{
	Toolbox::printDebugMessage("Panic");
	self->setState(panicked);
}

void TacticsCodec::flee(SoldierData _SD, Soldier* self)
{
	Toolbox::printDebugMessage("Flee");

	self->addCommandToList("hunkerDown");

	act(_SD, self);
}

void TacticsCodec::advance(Platoon* enemyPlatoon, TerrainManager* terrainManager, Soldier* leader, Soldier* self)
{
	Toolbox::printDebugMessage("Forward");
	self->setCurrentTarget(nullptr);
	float distance = 20000000.0f;
	for (auto iter2 = 0; iter2 != enemyPlatoon->platoonSections.size(); iter2++)
	{
		for (auto iter = 0; enemyPlatoon->platoonSections[iter2]->soldiers.size() > iter; iter++)
		{
			if (enemyPlatoon->platoonSections[iter2]->soldiers[iter]->getState() == aliveAndWell)
			{
				float newDistanceX = enemyPlatoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition().x - self->shape.getPosition().x;
				float newDistanceY = enemyPlatoon->platoonSections[iter2]->soldiers[iter]->shape.getPosition().y - self->shape.getPosition().y;

				newDistanceX = newDistanceX * newDistanceX;
				newDistanceY = newDistanceY * newDistanceY;

				float newDistance = newDistanceX + newDistanceY;

				//newDistance = newDistance / newDistance;

				//sf::Vector2f newDistance = this->getPosition() - terrainManager->terrainSquares[iter]->shape.getPosition();
				if (newDistance < distance)
				{
					self->setCurrentTarget(enemyPlatoon->platoonSections[iter2]->soldiers[iter]);
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

	//sf::Vector2f midpoint = Toolbox::findMidPoint(this->getPosition(), target->getPosition());
	if (self->getCurrentTarget() == nullptr)
	{

	}
	else
	{
		self->clearCommandList();
		self->generateMapToGoal(self->getCurrentTarget()->shape.getPosition(), terrainManager);
		self->pathFindToGoal(self->getCurrentTarget()->shape.getPosition(), terrainManager);
		self->executeCommand(terrainManager, leader, enemyPlatoon);
		//clearCommandList();
	}
}

void TacticsCodec::checkRange(Platoon* enemyPlatoon, Soldier* self)
{
	float distance = Toolbox::findDistanceOfEnemies(enemyPlatoon, self);

	if (distance < 100000.0f)
	{
		self->setInRange(true);

		//delete target;
	}
	else
	{
		self->setInRange(false);
	}
}

void TacticsCodec::setOrder(std::string toSet)
{
	currentOrder = toSet;
}