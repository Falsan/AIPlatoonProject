#include "Toolbox.h"

Toolbox::Toolbox()
{

}

Toolbox::~Toolbox()
{

}

void Toolbox::clearSpace()
{
	for (int iter = 0; iter < 100; iter++)
	{
		std::cout << std::endl;
	}
	
}

void Toolbox::printDebugMessage(std::string message)
{
	std::cout << message << std::endl;
}

void Toolbox::printDebugMessage(int message)
{
	std::cout << message << std::endl;
}

void Toolbox::printDebugMessage(sf::Vector2f message)
{
	float tempx = message.x;
	float tempy = message.y;

	std::cout << "X is " << tempx << std::endl;
	std::cout << "Y is " << tempy << std::endl;
}

int Toolbox::addAHundred(int toMultiply)
{
	toMultiply = toMultiply + 100;

	return toMultiply;
}

int Toolbox::addATwenty(int toAdd)
{
	toAdd = toAdd + 20;

	return toAdd;
}

sf::Vector2f Toolbox::findMidPoint(sf::Vector2f firstVector, sf::Vector2f secondVector)
{
	sf::Vector2f midpoint = firstVector + secondVector;

	midpoint.x = midpoint.x / 2;
	midpoint.y = midpoint.y / 2;

	return midpoint;
}