#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <thread>
#include <TlHelp32.h>

#define pToPX()  ReadProcessMemory(process, xPointer, &xPointer2, sizeof(float*), nullptr);\
				xPointer = xPointer2

#define pToPY()  ReadProcessMemory(process, yPointer, &yPointer2, sizeof(float*), nullptr);\
				yPointer = yPointer2

#define printPtr(x) std::cout << uint64_t(x) << '\n'

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

int main() {
	DWORD* processID = new DWORD;
	GetWindowThreadProcessId(FindWindowW(NULL, L"The Stanley Parable: Ultra Deluxe"), processID);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, false, *processID);
	if (!process) {
		std::cout << "Failed to create process";
		exit(1);
	}
	else {
		std::cout << "Didn't fail\n";
	}
	void* moduleBase = (void*)GetModuleBaseAddress(*processID, L"UnityPlayer.dll");
	
	char* xPointer = (char*)(moduleBase);
	char* xPointer2;

	xPointer += 0x017A5BC0;
	pToPX();
	xPointer += 0x108;
	pToPX();
	xPointer += 0x60;
	pToPX();
	xPointer += 0xB0;
	pToPX();
	xPointer += 0x68;
	pToPX();
	xPointer += 0x98;

	char* yPointer = (char*)(moduleBase);
	char* yPointer2;

	yPointer += 0x0183BFA8;
	pToPY();
	yPointer += 0x40;
	pToPY();
	yPointer += 0xAA0;
	pToPY();
	yPointer += 0x810;


	while (true) {
		/*bool ok = WriteProcessMemory(process, xPointer2, xPosition, sizeof(float), nullptr);
		std::cout << (ok ? "worked\n" : "didn't work\n");*/

		if (GetKeyState(VK_UP) & 0x8000) {
			float buffer = 0;
			std::cout << ReadProcessMemory(process, xPointer, &buffer, sizeof(float), nullptr) << '\n';
			buffer += .3f;
			std::cout << WriteProcessMemory(process, xPointer, &buffer, sizeof(float), nullptr) << '\n';
		}
		if (GetKeyState(VK_DOWN) & 0x8000) {
			float buffer = 0;
			std::cout << ReadProcessMemory(process, xPointer, &buffer, sizeof(float), nullptr) << '\n';
			buffer -= .3f;
			std::cout << WriteProcessMemory(process, xPointer, &buffer, sizeof(float), nullptr) << '\n';
		}

		if (GetKeyState(VK_RIGHT) & 0x8000) {
			float buffer = 0;
			std::cout << ReadProcessMemory(process, yPointer, &buffer, sizeof(float), nullptr) << '\n';
			buffer += .3f;
			std::cout << WriteProcessMemory(process, yPointer, &buffer, sizeof(float), nullptr) << '\n';
		}
		if (GetKeyState(VK_LEFT) & 0x8000) {
			float buffer = 0;
			std::cout << ReadProcessMemory(process, yPointer, &buffer, sizeof(float), nullptr) << '\n';
			buffer -= .3f;
			std::cout << WriteProcessMemory(process, yPointer, &buffer, sizeof(float), nullptr) << '\n';
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}