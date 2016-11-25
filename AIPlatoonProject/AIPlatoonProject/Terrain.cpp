#include "Terrain.h"

Terrain::Terrain()
{
	isgoal = false;
}
Terrain::~Terrain()
{

}

void Terrain::setIsCover(bool cover)
{
	isCover = cover;
}

bool Terrain::getIsCover()
{
	return isCover;
}

void Terrain::setIsOccupied(bool occupied)
{
	isOccupied = occupied;
}

bool Terrain::getIsOccupied()
{
	return isOccupied;
}

void Terrain::setTexture(sf::Texture textureToSet)
{
	texture = textureToSet;
}

bool Terrain::getIsPassable()
{
	return isPassable;
}

void Terrain::setIsPassable(bool passToSet)
{
	isPassable = passToSet;
}

void Terrain::setGoal(bool goal)
{
	isgoal = goal;
}

bool Terrain::getGoal()
{
	return isgoal;
}

void Terrain::setSpawn(bool toSet)
{
	isSpawn = toSet;
}

bool Terrain::getSpawn()
{
	return isSpawn;
}
