#include "Game.h"


int main()
{
	Game* thisGame = new Game;

	while (thisGame->isRunning == true)
	{
		thisGame->isRunning = thisGame->runGame();
	}
	return 0;
}