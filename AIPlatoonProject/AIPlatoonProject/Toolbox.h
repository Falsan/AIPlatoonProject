#pragma once
#include <iostream>
#include <string>
#include <SFML\Graphics.hpp>

class Toolbox
{
public:
	Toolbox();
	~Toolbox();

	static void clearSpace();
	static void printDebugMessage(std::string message);
	static void printDebugMessage(sf::Vector2f);
	static void printDebugMessage(int);
	static int addAHundred(int);
	static int addATwenty(int);
	static sf::Vector2f findMidPoint(sf::Vector2f, sf::Vector2f);

private:

};
