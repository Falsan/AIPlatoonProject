#pragma once

#include "PlatoonSection.h"

class PlatoonSection;

class Platoon
{
public:
	Platoon();
	~Platoon();

	std::vector<PlatoonSection*> platoonSections;

	void createPlatoonSection();
	void createPlatoon();
	void assignTeam(int);


	int platoonTeam;
private:

	
};

