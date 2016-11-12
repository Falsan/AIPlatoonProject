#include "PlatoonSection.h"

PlatoonSection::PlatoonSection()
{
	
}

PlatoonSection::~PlatoonSection()
{
	
	soldiers.clear();
	
}

void PlatoonSection::addSoldier()
{
	soldiers.push_back(new Soldier);

	soldiers.back()->shape.setFillColor(sf::Color::Green);
	soldiers.back()->shape.setRadius(10.0f);
}

void PlatoonSection::interpretTactics()
{
	if (command == "")
	{

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