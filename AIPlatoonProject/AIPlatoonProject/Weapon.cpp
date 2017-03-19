#include "Weapon.h"

Weapon::Weapon()
{
	weaponType = rifle;
}

Weapon::~Weapon()
{

}

//this is the main function that deals with shooting
void Weapon::shoot(Platoon* enemyPlatoon, Soldier* target, bool inRange, Soldier* self, bool hasTargetInRange)
{

	if (hasTargetInRange == true)
	{
		if (weaponType == rifle || weaponType == scopedRifle)
		{
			takeShot(target, self);
		}
		else if (weaponType == lightMachineGun)
		{
			takeShot(target, self);
			takeShot(target, self);
			takeShot(target, self);
			takeShot(target, self);
			takeShot(target, self);
		}
		else if (weaponType == subMachineGun)
		{
			takeShot(target, self);
			takeShot(target, self);
		}

		hasTargetInRange = false;
	}
	else if (hasTargetInRange == false)
	{
		inRange = false;
	}
	else
	{
		Toolbox::printDebugMessage("Hunkerdown");
	}
}

void Weapon::takeShot(Soldier* target, Soldier* self)
{
	int hit = rand() % 10 + 1;
	int missChance;

	if (self->getActualBraveryRating() > self->getBraveryRating())
	{
		missChance = self->getActualBraveryRating() - self->getBraveryRating();
	}



	Toolbox::printDebugMessage("Pew");
	target->setGettingShotAt(true);
	self->setShooting(true);
	if (hit >= 5)//hit
	{
		Toolbox::printDebugMessage("Hit");
		//target->gettingShotAt = true;
		target->reduceHealth();

		if (target->getHealth() <= 0)
		{
			target->setState(dead);
			target->shape.setFillColor(sf::Color::Black);
		}
	}
	else if (hit < 5)//miss
	{
		Toolbox::printDebugMessage("Miss");
	}
}

void Weapon::reload()
{
	setAmmo(getAmmoCapacity());
}

void Weapon::setupAmmoCapacity()
{
	if (getWeaponType() == WeaponTypes::rifle)
	{
		setAmmoCapacity(5);
		setAmmo(5);
	}
	else if (getWeaponType() == WeaponTypes::scopedRifle)
	{
		setAmmoCapacity(5);
		setAmmo(5);
	}
	else if (getWeaponType() == WeaponTypes::subMachineGun)
	{
		setAmmoCapacity(30);
		setAmmo(30);
	}
	else if (getWeaponType() == WeaponTypes::assaultRifle)
	{
		setAmmoCapacity(30);
		setAmmo(30);
	}
	else if (getWeaponType() == WeaponTypes::lightMachineGun)
	{
		setAmmoCapacity(100);
		setAmmo(100);
	}
}

void Weapon::setAmmo(int ammoToSet)
{
	ammo = ammoToSet;
}

int Weapon::getAmmoCount()
{
	return ammo;
}

void Weapon::setWeaponType(WeaponTypes weaponTypeToSet)
{
	weaponType = weaponTypeToSet;
}

WeaponTypes Weapon::getWeaponType()
{
	return weaponType;
}

void Weapon::setAmmoCapacity(int ammoToSet)
{
	ammoCapacity = ammoToSet;
}

int Weapon::getAmmoCapacity()
{
	return ammoCapacity;
}