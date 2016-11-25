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
	void setGoalSquare(int);
	void removeGoalFromSquare(int);
	
	//int getGoalSquare();

	

private:

	int goalSquare;
};

