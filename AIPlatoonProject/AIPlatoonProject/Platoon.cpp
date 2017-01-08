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
	PlatoonSection* temp = new PlatoonSection;
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->addSoldier();
	temp->chooseLeader();
	platoonSections.push_back(temp);
}