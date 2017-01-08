#pragma once
#include <SFML\Graphics.hpp>
#include "Toolbox.h"
#include "Soldier.h"

class Soldier;

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
	void chooseLeader();
	int getLeader();
	void addSoldier();
	sf::Color teamColour;
	sf::Color teamLeaderColour;
	std::vector<Soldier*> soldiers;

private:
	int leader;
		//+ waypoint vector2
		//+ waypointFlank vector2
};