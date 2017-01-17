#include "Game.h"


int main()
{
	Game* thisGame = new Game;

	srand(time(NULL));

	while (thisGame->isRunning == true)
	{
		thisGame->isRunning = thisGame->runGame();
	}

	delete thisGame;

	return 0;
}