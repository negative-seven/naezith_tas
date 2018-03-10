#pragma once

#include <iostream>
#include <windows.h>

#include "utils.h"
#include "winmemory.h"

class GameManager
{
public:
	bool alloc;
private:
	static const char *windowName;

	WindowsMemory::MemoryHandler *memory;

public:
	GameManager();
	void run();
private:
	void injectCode();
	void injectCodeNoAlloc();
	void writeReplay();
};
