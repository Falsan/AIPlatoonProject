#pragma once

#include <SFML\Graphics.hpp>
#include "Toolbox.h"
#include "TerrainManager.h"


class Soldier
{
public:
	Soldier();
	~Soldier();

	void setTexture(sf::Texture);

	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition();

	void moveTowardsGoal(sf::Vector2f, TerrainManager*);

	void moveUp();
	void moveLeft();
	void moveDown();
	void moveRight();
	void resolveIfStuck(TerrainManager*, int, int, int, int);

	sf::CircleShape shape;

private:


	sf::Vector2f position;
	sf::Texture texture;

};