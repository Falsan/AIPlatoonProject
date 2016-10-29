#include "Terrain.h"

Terrain::Terrain()
{

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

