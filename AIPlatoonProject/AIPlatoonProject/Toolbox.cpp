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