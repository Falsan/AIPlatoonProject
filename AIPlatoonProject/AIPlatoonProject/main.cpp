#include "Game.h"


int main()
{
	Game* thisGame = new Game;

	while (thisGame->isRunning == true)
	{
		thisGame->isRunning = thisGame->runGame();
	}

	delete thisGame;

	return 0;
}