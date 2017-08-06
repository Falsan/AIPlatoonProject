#include "Game.h"
#include <utility>
#include <memory>

int main()
{
	std::unique_ptr<Game> thisGame(new Game);

	srand(time(NULL));

	while (thisGame->isRunning == true)
	{
		thisGame->isRunning = thisGame->runGame();
	}

	return 0;
}