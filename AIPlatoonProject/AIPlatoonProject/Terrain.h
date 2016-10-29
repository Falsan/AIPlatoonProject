#pragma once

#include<SFML\Graphics.hpp>

//this class is essentiall a square of land which will have certain properties
class Terrain : sf::Transformable
{
public:
	Terrain();
	~Terrain();

	bool getIsCover();
	void setIsCover(bool);
	bool getIsOccupied();
	void setIsOccupied(bool);

	void setTexture(sf::Texture);

	void setUpTerrainSquare();

private:

	bool isOccupied;
	bool isCover;
	sf::Texture texture;
	int positionX;
	int positionY;

};
