#pragma once
#include <Windows.h>
#include <iostream>


#define READ 1
#define WRITE 2
#define DO_NOTHING 0

DWORD ReadRegKey(LPCSTR KeyValueName)
{
	DWORD data;
	DWORD BufferSize = sizeof(DWORD);
	RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", KeyValueName, RRF_RT_ANY, NULL, (PVOID)&data, &BufferSize);
	std::cout << "Data read from ReadRegKey: " << data << "\n";
	return data;
}

void WriteRegKey(DWORD data, LPCSTR valueName)
{
	std::cout << "Attempting to WriteRegKey data recieved: " << data << "\n";

	HKEY handle;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &handle)  == ERROR_SUCCESS)
		RegSetValueExA(handle, valueName, NULL, REG_DWORD, (PBYTE)&data, sizeof(DWORD));
	else
		std::cout << "couldn't open key\n";
}

