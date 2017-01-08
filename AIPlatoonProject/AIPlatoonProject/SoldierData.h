#pragma once

#include "TerrainManager.h"
#include "Soldier.h"
#include "PlatoonSection.h"
#include "Platoon.h"

class Soldier;
class PlatoonSection;
class TerrainManager;
class Platoon;

struct SoldierData
{
	TerrainManager* m_terrainManager;
	Soldier* m_leader;
	PlatoonSection* m_platoonSection;
	Platoon* enemyPlatoon;
};