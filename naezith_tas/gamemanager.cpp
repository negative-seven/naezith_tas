#include "gamemanager.h"

const char *GameManager::windowName = "Remnants of Naezith";

GameManager::GameManager()
{
	alloc = true;

	memory = new WindowsMemory::MemoryHandler(NULL, windowName);
}

void GameManager::run()
{
	if (alloc)
	{
		injectCode();
	}
	else
	{
		injectCodeNoAlloc();
	}

	while (true)
	{
		writeReplay();
		Sleep(500);
	}
}

void GameManager::injectCode()
{
	HWND hWindow = FindWindow(NULL, windowName);
	DWORD pId;
	GetWindowThreadProcessId(hWindow, &pId);
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pId);

	LPVOID alloc = VirtualAllocEx(pHandle, NULL, replaySize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	std::cout << "Allocated " << replaySize << " bytes of memory at 0x" << std::hex << (uint32_t)alloc << std::dec << '\n';

	{
		uint8_t bytes[] = {
			'\x48', '\xBA', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',	// mov rdx, (qword)alloc
			'\x4C', '\x8D', '\x82', '\x00', '\x00', '\x00', '\x00',							// lea r8, [rdx+MAX_SIZE]
			'\xC3'																			// ret
		};
		uint64_t addr = (uint32_t)alloc;
		memcpy(bytes + 0x2, &addr, sizeof(addr));
		uint32_t addr2 = replaySize;
		memcpy(bytes + 0xD, &addr2, sizeof(addr2));

		memory->writeBytes((uint32_t)alloc, bytes, sizeof(bytes));
	}

	{
		uint8_t bytes[] = {
			'\xE8', '\x00', '\x00', '\x00', '\x00',	// call alloc
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90',									// nop
			'\x90'									// nop
		};
		uint32_t addr = (uint32_t)alloc - (memory->getBaseAddress("naezith.exe") + 0xEFEC4);
		memcpy(bytes + 0x1, &addr, sizeof(addr));

		memory->writeBytes(memory->getBaseAddress("naezith.exe") + 0xEFEBF, bytes, sizeof(bytes));
	}
}

void GameManager::injectCodeNoAlloc()
{
	uint8_t bytes[] = {
		'\x4C', '\x8D', '\x82', '\x00', '\x00', '\x00', '\x00',	// lea r8, [rdx+MAX_SIZE]
		'\x90',													// nop
		'\x90',													// nop
		'\x90'													// nop
	};
	uint32_t addr = replaySize;
	memcpy(bytes + 0x3, &addr, sizeof(addr));

	memory->writeBytes(memory->getBaseAddress("naezith.exe") + 0xEFEC6, bytes, sizeof(bytes));
}

void GameManager::writeReplay()
{
	std::string str = readFile(replayFile);
	uint8_t *temp = new uint8_t[str.size() + 1];
	strncpy_s(reinterpret_cast<char *>(temp), str.size() + 1, str.c_str(), str.size());

	uint32_t replayStringStartPtr = memory->readUint32(memory->getBaseAddress("naezith.exe") + 0x1F3520);
	uint32_t replayStringEndPtr = memory->readUint32(memory->getBaseAddress("naezith.exe") + 0x1F3528);;

	unsigned int size = min(replayStringEndPtr - replayStringStartPtr, str.size() + 1);
	if (size > 0)
	{
		memory->writeBytes(replayStringStartPtr, temp, size);
	}
}
