#pragma once
#include <SFML\Graphics.hpp>
#include "Toolbox.h"
#include "Soldier.h"


class PlatoonSection
{
public:
	PlatoonSection();
	~PlatoonSection();

	void interpretTactics();
	void giveOrders();

	std::string command;

	std::string getCommand();
	void setCommand(std::string);

	void addSoldier();

	std::vector<Soldier*> soldiers;

private:
		//+ waypoint vector2
		//+ waypointFlank vector2
};