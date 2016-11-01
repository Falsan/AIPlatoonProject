#pragma once

#include <SFML\Graphics.hpp>


class Soldier
{
public:
	Soldier();
	~Soldier();

	void setTexture(sf::Texture);

	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition();

	void moveUp();
	void moveLeft();
	void moveDown();
	void moveRight();

	sf::CircleShape shape;

private:

	sf::Vector2f position;
	sf::Texture texture;

};