#pragma once

#include "Terrain.h"
#include "Toolbox.h"
#include <time.h>

class TerrainManager
{
public:
	TerrainManager();
	~TerrainManager();

	std::vector<Terrain*> terrainSquares;

	void setUpTerrainSquares();

	int goalSquare;

private:

};

