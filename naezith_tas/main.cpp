#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

#include "winmemory.h"

#define MAX_SIZE 0x1000

std::string readFile(std::string filepath)
{
	std::ifstream file;
	std::ostringstream buffer;

	file.open(filepath);
	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}

void run()
{
	WindowsMemory::MemoryHandler memory = WindowsMemory::MemoryHandler(FindWindow(NULL, "Remnants of Naezith"));

	HWND hWindow = FindWindow(NULL, "Remnants of Naezith");
	DWORD pId;
	GetWindowThreadProcessId(hWindow, &pId);
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pId);

	LPVOID alloc = VirtualAllocEx(pHandle, NULL, MAX_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	std::cout << "Allocated " << MAX_SIZE << " bytes of memory at 0x" << std::hex << (uint32_t)alloc << std::dec << '\n';

	{
		uint8_t bytes[] = "\x48\xBA\x00\x00\x00\x00\x00\x00\x00\x00\x4C\x8D\x82\x00\x00\x00\x00\xC3";
		uint64_t addr = (uint32_t)alloc;
		uint32_t addr2 = MAX_SIZE;
		memcpy(bytes + 0x2, &addr, sizeof(addr));
		memcpy(bytes + 0xD, &addr2, sizeof(addr2));
		memory.writeBytes((uint32_t)alloc, bytes, 18);
	}

	{
		uint8_t bytes[] = "\xE8\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
		uint32_t addr = (uint32_t)alloc - (memory.getBaseAddress("naezith.exe") + 0xE92C4);
		memcpy(bytes + 0x1, &addr, sizeof(addr));
		memory.writeBytes(memory.getBaseAddress("naezith.exe") + 0xE92BF, bytes, 17);
	}

	while (true)
	{
		std::string str = readFile("inputs.txt");
		uint8_t *temp = new uint8_t[str.size() + 1];
		strncpy_s(reinterpret_cast<char *>(temp), str.size() + 1, str.c_str(), str.size());

		uint32_t replayStringStartPtr = memory.readUint32(memory.getBaseAddress("naezith.exe") + 0x1EC468);
		uint32_t replayStringEndPtr = memory.readUint32(memory.getBaseAddress("naezith.exe") + 0x1EC470);;

		unsigned int size = min(replayStringEndPtr - replayStringStartPtr, str.size() + 1);
		if (size > 0)
		{
			memory.writeBytes(replayStringStartPtr, temp, size);
		}

		Sleep(500);
	}
}

void runNoAlloc()
{
	WindowsMemory::MemoryHandler memory = WindowsMemory::MemoryHandler(FindWindow(NULL, "Remnants of Naezith"));

	{
		uint8_t bytes[] = "\x4C\x8D\x82\x00\x10\x00\x00\x90\x90\x90";
		memory.writeBytes(memory.getBaseAddress("naezith.exe") + 0xE92C6, bytes, 10);
	}

	while (true)
	{
		std::string str = readFile("inputs.txt");
		uint8_t *temp = new uint8_t[str.size() + 1];
		strncpy_s(reinterpret_cast<char *>(temp), str.size() + 1, str.c_str(), str.size());

		uint32_t replayStringStartPtr = memory.readUint32(memory.getBaseAddress("naezith.exe") + 0x1EC468);
		uint32_t replayStringEndPtr = memory.readUint32(memory.getBaseAddress("naezith.exe") + 0x1EC470);;

		unsigned int size = min(replayStringEndPtr - replayStringStartPtr, str.size() + 1);
		if (size > 0)
		{
			memory.writeBytes(replayStringStartPtr, temp, size);
		}

		Sleep(500);
	}
}

int main(int argc, char *argv[])
{
	bool alloc = true;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--noalloc") == 0)
		{
			alloc = false;
		}
	}

	if (alloc)
	{
		run();
	}
	else
	{
		runNoAlloc();
	}

	return 0;
}
