#pragma once

#include "Terrain.h"
#include "Toolbox.h"
#include <time.h>
#include <fstream>

class TerrainManager
{
public:
	TerrainManager();
	~TerrainManager();

	std::vector<int> terrainValues; //0 empty, 1 cover, 2 wall
	std::vector<Terrain*> terrainSquares;

	void setUpTerrainSquares();
	void setGoalSquare(int);
	void removeGoalFromSquare(int);
	void readInFile();
	void randomGen(float&, float&);
	
	//int getGoalSquare();

	

private:

	int goalSquare;
};

