#include "PlatoonSection.h"

PlatoonSection::PlatoonSection()
{
	leader = NULL;
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
	if (command == "")
	{
		chooseLeader();
	}
	else
	{
		giveOrders();
	}
}

void PlatoonSection::giveOrders()
{
	for (auto iter = 0; iter < soldiers.size(); iter++)
	{
		soldiers[iter]->setOrder(command);
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

int PlatoonSection::getLeader()
{
	return leader;
}