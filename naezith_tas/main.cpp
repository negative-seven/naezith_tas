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

int main()
{
	WindowsMemory::MemoryHandler memory = WindowsMemory::MemoryHandler(FindWindow(NULL, "Remnants of Naezith"));

	HWND hWindow = FindWindow(NULL, "Remnants of Naezith");
	DWORD pId;
	GetWindowThreadProcessId(hWindow, &pId);
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pId);

	LPVOID alloc = VirtualAllocEx(pHandle, NULL, MAX_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	std::cout << "Allocated " << MAX_SIZE << " bytes of memory at 0x" << std::hex << (uint32_t)alloc << std::dec << '\n';

	uint32_t pointer;
	uint32_t pointer2;
	try
	{
		pointer = memory.getBaseAddress("naezith.exe") + 0x1EC468;
		pointer2 = memory.getBaseAddress("naezith.exe") + 0x1EC470;
	}
	catch (std::runtime_error e)
	{
		std::cout << GetLastError() << '\n';
		while (true);
	}

	while (true)
	{
		std::string str = readFile("inputs.txt");
		uint8_t *temp = new uint8_t[str.size() + 1];
		strncpy_s(reinterpret_cast<char *>(temp), str.size() + 1, str.c_str(), str.size());

		memory.writeBytes((uint32_t)alloc, temp, str.size());
		memory.writeUint32(pointer, (uint32_t)alloc);
		memory.writeUint32(pointer2, (uint32_t)alloc + MAX_SIZE);
		Sleep(500);
	}

	return 0;
}
