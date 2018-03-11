#include "gamemanager.h"

int main(int argc, char *argv[])
{
	GameManager gameManager = GameManager();

	bool alloc = true;
	const char *replayFile = "inputs.txt";
	uint32_t replaySize = 0x1000;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--no-alloc") == 0)
		{
			alloc = false;
		}

		if (strcmp(argv[i], "--replay-file") == 0)
		{
			if (i < argc - 1)
			{
				replayFile = argv[i + 1];
			}
		}

		if (strcmp(argv[i], "--replay-size") == 0)
		{
			if (i < argc - 1)
			{
				replaySize = strtol(argv[i + 1], NULL, 0);
			}
		}
	}

	gameManager.alloc = alloc;
	gameManager.replayFile = replayFile;
	gameManager.replaySize = replaySize;
	gameManager.run();

	return 0;
}
