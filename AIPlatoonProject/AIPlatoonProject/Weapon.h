#pragma once
#include "WeaponTypes.h"
#include "Platoon.h"
#include "Soldier.h"

class Soldier;


//much of this class is INCOMPLETE as only shooting is required for this project
//further development would see this class completed
class Weapon
{
public:

	void shoot(Platoon*, Soldier*, bool, Soldier*, bool);
	void reload();

	void setWeaponType(WeaponTypes);
	WeaponTypes getWeaponType();

	void setAmmo(int);
	int getAmmoCount();

	void setAmmoCapacity(int);
	int getAmmoCapacity();

	void setupAmmoCapacity();

private:

	int ammoCapacity;
	int ammo;
	WeaponTypes weaponType;

};