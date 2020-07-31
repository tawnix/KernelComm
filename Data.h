#pragma once
#include "messages.h"
#include "memory.h"

// Have these here so I can use RPM on 64bit and 32bit games
typedef __int32 DWORD, * PDWORD, * LPDWORD; //32bit
typedef __int64 QWORD; //64 bit


// shitty way of creating bool probably wont use this at all
typedef enum { false, true } bool;

//========Registry Key Info========
HANDLE      hKey = 0, hKey2 = 0;
ULONG       ulResult, ulResult2;
NTSTATUS    Status, Status2;
OBJECT_ATTRIBUTES Obj_Attr, Obj_Attr2;
UNICODE_STRING KeyName;
UNICODE_STRING Address;
UNICODE_STRING KeyPath, KeyPath2;
//========Registry Key Info========


//========ProcessInfo========
ULONG ProcessID = 0;


void ReadData2(DWORD Address, PVOID output, int type)
{
    PEPROCESS Process;
    if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)ProcessID, &Process)))
    {
        ReadMemory(Address, &output, type, Process);
    }
}

void mainLoop()
{
    DWORD SetBackCode = 0;
    DWORD ReadCode = 0;
    ULONG ReadAddress = 0;
    ULONG OutPut = 0;

    while (ReadCode != 666)
    {

        // read reg key value and check what the message is from um
        reg_query_value(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"Code", REG_DWORD, &ReadCode, sizeof(DWORD));

        switch (ReadCode)
        {
        case(0):// do nothing
            break;
        case (1)://read memory
            PrintMsg("Calling read key now!");
            //Read address from memory store it in ReadAddress
            reg_query_value(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"Address", REG_DWORD, &ReadAddress, sizeof(DWORD));
            DbgPrintEx(0, 0, "Data read: %d", ReadAddress);

            DbgPrintEx(0, 0, "Calling RPM With params: Address [+] %d", ReadAddress);

            ReadData2(ReadAddress, &OutPut, 1);

            DbgPrintEx(0, 0, "Output from RPM: %d", OutPut);



            // write the output from the RPM to the key and set the CODE back to 0 so the driver doesnt keep reading the same address
            WriteToKey(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"Address", REG_DWORD, &OutPut, sizeof(DWORD));
            WriteToKey(L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"Code", REG_DWORD, &SetBackCode, sizeof(DWORD));
            
            break;
        case (2): //write mem
            break;
        default:
            break;
        }
    }
}