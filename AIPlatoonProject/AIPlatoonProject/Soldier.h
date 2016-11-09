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

	void addCommandToList(std::string);
	void executeCommand(std::string);
	void clearCommandList();

	void pathFindToGoal(sf::Vector2f, TerrainManager*);

	void moveUp();
	void moveLeft();
	void moveDown();
	void moveRight();
	void resolveIfStuck(TerrainManager*, int, int, int, int);

	sf::CircleShape shape;
	std::vector<std::string> commandList;

private:

	sf::Vector2f pathfinderPosition;
	sf::Vector2f position;
	sf::Texture texture;

};