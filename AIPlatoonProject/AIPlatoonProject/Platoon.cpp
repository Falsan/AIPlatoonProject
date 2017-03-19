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
	temp->soldiers[0]->getWeapon()->setWeaponType(scopedRifle);
	temp->soldiers[1]->getWeapon()->setWeaponType(lightMachineGun);
	temp->soldiers[2]->getWeapon()->setWeaponType(subMachineGun);
	temp->soldiers[3]->getWeapon()->setWeaponType(rifle);
	temp->soldiers[4]->getWeapon()->setWeaponType(rifle);
	temp->soldiers[5]->getWeapon()->setWeaponType(rifle);
	temp->soldiers[0]->getWeapon()->setupAmmoCapacity();
	temp->soldiers[1]->getWeapon()->setupAmmoCapacity();
	temp->soldiers[2]->getWeapon()->setupAmmoCapacity();
	temp->soldiers[3]->getWeapon()->setupAmmoCapacity();
	temp->soldiers[4]->getWeapon()->setupAmmoCapacity();
	temp->soldiers[5]->getWeapon()->setupAmmoCapacity();
	platoonSections.push_back(temp);
}

void Platoon::assignTeam(int team)
{
	platoonTeam = team;
}