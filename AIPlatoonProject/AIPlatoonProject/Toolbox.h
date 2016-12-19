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

private:

};
