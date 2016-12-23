#pragma once

#include "TerrainManager.h"
#include "Soldier.h"
#include "PlatoonSection.h"

struct SoldierData
{
	TerrainManager* m_terrainManager;
	Soldier* m_leader;
	PlatoonSection* enemyPlatoon;
};