#pragma once
#include <ntifs.h>

//thanks to k1x for the read memory function!
VOID	ReadMemory(ULONG64 address, PVOID buffer, int type, PEPROCESS process)
{
    DbgPrintEx(0, 0, "Address in ReadMemory recieved: %d", address);

    KAPC_STATE  apc_state;
    KeStackAttachProcess(process, &apc_state);

    SIZE_T size = { 0 };

    switch (type)
    {
    case 1: //Int
        size = sizeof(int);
        break;
    case 2: //Float
        size = sizeof(float);
        break;
    case 3: //Double
        size = sizeof(double);
        break;
    case 4: //Char
        size = sizeof(char);
        break;
    }
    if (MmIsAddressValid((PVOID)address) && MmIsAddressValid((PVOID)(address + size)))
    {
        DbgPrintEx(0, 0, "Memory valid!");
        RtlCopyMemory(buffer, (PVOID)address, size);
    }
    KeUnstackDetachProcess(&apc_state);
}