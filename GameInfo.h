#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

#define LOCAL_PLAYER 0x6883F4

DWORD ClientBase{ 0 };

void GetClientDllBase(char* moduleName, DWORD ProcessID)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (strcmp(modEntry.szModule, moduleName) == 0)
                {
                    ClientBase = (DWORD)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
}



DWORD get_process_id(LPCSTR lpExeName)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapShot == INVALID_HANDLE_VALUE)
        return NULL;

    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);

    for (BOOL success = Process32First(hSnapShot, &pe); success == TRUE; success = Process32Next(hSnapShot, &pe))
    {
        if (strcmp(lpExeName, pe.szExeFile) == 0)
        {
            std::cout << "Got proc id!\n\n\n";
            CloseHandle(hSnapShot);
            return pe.th32ProcessID;
        }
    }

    CloseHandle(hSnapShot);
    return NULL;
}