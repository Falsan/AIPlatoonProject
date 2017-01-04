#pragma once

#include "TerrainManager.h"
#include "Soldier.h"
#include "PlatoonSection.h"

class Soldier;
class PlatoonSection;
class TerrainManager;

struct SoldierData
{
	TerrainManager* m_terrainManager;
	Soldier* m_leader;
	PlatoonSection* enemyPlatoon;
};