#include "gamemanager.h"

int main(int argc, char *argv[])
{
	GameManager gameManager = GameManager();

	bool alloc = true;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--no-alloc") == 0)
		{
			alloc = false;
		}
	}

	gameManager.alloc = alloc;
	gameManager.run();

	return 0;
}
