#include "Platoon.h"

Platoon::Platoon()
{
}

Platoon::~Platoon()
{
}

void Platoon::createPlatoon()
{
	createPlatoonSection();
	createPlatoonSection();
	createPlatoonSection();
	createPlatoonSection();
}

void Platoon::createPlatoonSection()
{
	sf::Color teamColour;
	sf::Color leaderColour;
	if (platoonTeam == 1)
	{
		teamColour = sf::Color::Green;
		leaderColour = sf::Color::Yellow;
	}
	else if (platoonTeam == 2)
	{
		teamColour = sf::Color::Red;
		leaderColour = sf::Color::Cyan;
	}
	PlatoonSection* temp = new PlatoonSection;
	temp->teamLeaderColour = leaderColour;
	temp->teamColour = teamColour;
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	//temp->addSoldier();
	//temp->addSoldier();
	temp->chooseLeader();
	platoonSections.push_back(temp);
}

void Platoon::assignTeam(int team)
{
	platoonTeam = team;
}