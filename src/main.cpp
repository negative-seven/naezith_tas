#include <io.h>

#include "gamemanager.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: naezith_tas <filepath> [optional arguments]";
		return 0;
	}

	const char *replayFile = argv[1];
	bool alloc = true;
	uint32_t replaySize = 0x1000;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--no-alloc") == 0)
		{
			alloc = false;
		}

		if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--replay-size") == 0)
		{
			if (i < argc - 1)
			{
				replaySize = strtol(argv[i + 1], NULL, 0);
			}
		}
	}
	
	GameManager *gameManager;
	try
	{
		gameManager = new GameManager(alloc, replayFile, replaySize);
	}
	catch (std::runtime_error)
	{
		std::cout << "Error: Could not find game window.";
		return 0;
	}
	
	gameManager->run();
	
	return 0;
}
