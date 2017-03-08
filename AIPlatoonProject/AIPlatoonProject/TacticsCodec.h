#pragma once

#include "SoldierData.h"
#include "Soldier.h"

class TerrainManager;
class Soldier;
class Platoon;

class TacticsCodec
{
public:
	TacticsCodec();
	~TacticsCodec();
	void checkForCover(Soldier*);
	void think(SoldierData, Soldier*);
	void panicCheck(SoldierData, Soldier*);
	void act(SoldierData, Soldier*);
	void interpretOrders(Platoon* enemyPlatoon, Soldier*);
	void flank(SoldierData _SD, Soldier*);
	void soldierPanic(SoldierData _SD, Soldier*);
	void flee(SoldierData _SD, Soldier*);
	void advance(Platoon* enemyPlatoon, TerrainManager* terrainManager, Soldier* leader, Soldier* self);
	void checkRange(Platoon* enemyPlatoon, Soldier*);
	void setOrder(std::string);

private:

	std::string currentOrder = "";
};