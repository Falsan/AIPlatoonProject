#include "PlatoonSection.h"

PlatoonSection::PlatoonSection()
{
	leader = NULL;
	isLeaderDead = false;
}

PlatoonSection::~PlatoonSection()
{
	
	soldiers.clear();
	
}

void PlatoonSection::chooseLeader()
{
	leader = rand() % soldiers.size();
	soldiers[leader]->setLeader(true);
	soldiers[leader]->shape.setFillColor(teamLeaderColour);
}

void PlatoonSection::addSoldier()
{
	soldiers.push_back(new Soldier);

	soldiers.back()->shape.setFillColor(teamColour);
	soldiers.back()->shape.setRadius(10.0f);
}

void PlatoonSection::interpretTactics()
{
	int half = soldiers.size() / 2;
	int numberOfDeadSoldiers = 0;
	int numberOfFleeingSoldiers = 0;

	for (auto iter = 0; iter < soldiers.size(); iter++)
	{
		if (soldiers[iter]->getState() == dead)
		{
			numberOfDeadSoldiers++;
		}
		if (soldiers[iter]->getState() == panicked)
		{
			numberOfFleeingSoldiers++;
		}
	}

	if (numberOfDeadSoldiers >= half)
	{
		command = "defend";
		Toolbox::printDebugMessage("Defend");
	}
	if (numberOfFleeingSoldiers > half)
	{
		command = "rally";
		Toolbox::printDebugMessage("Rally");
	}
	if (numberOfDeadSoldiers < half)
	{
		command = "attack";
		Toolbox::printDebugMessage("Attack");
	}

	giveOrders();
}

void PlatoonSection::giveOrders()
{
	for (auto iter = 0; iter < soldiers.size(); iter++)
	{
		soldiers[iter]->getTactics()->setOrder(command);
	}

	setCommand("");
}

void PlatoonSection::setCommand(std::string commandToSet)
{
	command = commandToSet;
}

std::string PlatoonSection::getCommand()
{
	return command;
}

void PlatoonSection::setIsLeaderDead(bool toSet)
{
	isLeaderDead = toSet;
}

bool PlatoonSection::getIsLeaderDead()
{
	return isLeaderDead;
}

int PlatoonSection::getLeader()
{
	return leader;
}
