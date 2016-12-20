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

	void moveUp(TerrainManager*);
	void moveLeft(TerrainManager*);
	void moveDown(TerrainManager*);
	void moveRight(TerrainManager*);

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

	void soldierThink(TerrainManager* terrainManager, Soldier* leader);

	bool mapGenerated; //hacky way of doing it, REVISE

	void calculateBraveryRating();

private:

	bool isLeader;
	std::vector<sf::Vector2f> pathfindPositions;
	std::vector<int> estimatedDistances;
	sf::Vector2f pathfinderPosition;
	sf::Vector2f position;
	sf::Texture texture;
	int state;
	bool pathFound;

	//these bools control the general status of the soldier
	bool gettingShotAt;
	bool inCover;
	bool shooting;
	bool leaderIsDead;
	bool fleeing;

	int braveryRating; //this integer controls how likely the soldier is to obey orders under prime conditions
	int actualBraveryRating; //this integer will be a modifyable value which controls if the soldier actually does something under stress
	

	std::vector<std::pair<int, int>> map;
};