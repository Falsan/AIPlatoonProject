#pragma once

#include <SFML\Graphics.hpp>
#include "Toolbox.h"
#include "TerrainManager.h"
#include "SoldierStates.h"


class Soldier
{
public:
	Soldier();
	~Soldier();

	void setTexture(sf::Texture);

	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition();

	std::string currentOrder;
	void setOrder(std::string);
	std::string getOrder();

	void addCommandToList(std::string);
	void executeCommand(TerrainManager* terrainManager, Soldier* leader);
	void clearCommandList();

	void pathFindToGoal(sf::Vector2f, TerrainManager*);
	void generateMapToGoal(sf::Vector2f, TerrainManager*);

	void moveUp();
	void moveLeft();
	void moveDown();
	void moveRight();

	void setState(SoldierStates);
	int getState();
	//void resolveIfStuck(TerrainManager*, int, int, int, int);

	sf::CircleShape shape;
	std::vector<std::string> commandList;

	void findCover(TerrainManager* terrainManager);
	int goalSquare;
	void findCoverTogether(TerrainManager*, Soldier*);
	void setLeader(bool);
	bool getLeader();

	bool mapGenerated; //hacky way of doing it, REVISE

private:

	bool isLeader;
	std::vector<sf::Vector2f> pathfindPositions;
	std::vector<int> estimatedDistances;
	sf::Vector2f pathfinderPosition;
	sf::Vector2f position;
	sf::Texture texture;
	int state;
	bool pathFound;

	

	std::vector<std::pair<int, int>> map;
};