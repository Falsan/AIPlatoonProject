#pragma once
#include <iostream>
#include <string>

class Toolbox
{
public:
	Toolbox();
	~Toolbox();

	static void clearSpace();
	static void printDebugMessage(std::string message);
	static int addAHundred(int);
	static int addATwenty(int);

private:

};
