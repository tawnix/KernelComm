#pragma once
#include <ntifs.h>

#define READ_REQUEST_CODE 1
#define WRITE_REQUEST_CODE 2

typedef __int32 DWORD, * PDWORD, * LPDWORD; //32bit
typedef __int64 QWORD; //64 bit


void PrintMsg(_In_ const char* Message)
{
	DbgPrintEx(0, 0, Message);
}







// REGISTER KEY FUNCTIONS
ULONG reg_query_value(PWSTR registry_path, PWSTR value_name, ULONG type, PVOID data, ULONG length)
{
    UNICODE_STRING valname;
    UNICODE_STRING keyname;
    OBJECT_ATTRIBUTES attribs;
    PKEY_VALUE_PARTIAL_INFORMATION pinfo;
    HANDLE handle;
    NTSTATUS rc;
    ULONG len, reslen;

    RtlInitUnicodeString(&keyname, registry_path);
    RtlInitUnicodeString(&valname, value_name);

    InitializeObjectAttributes(&attribs, &keyname, OBJ_CASE_INSENSITIVE,
        NULL, NULL);
    rc = ZwOpenKey(&handle, KEY_QUERY_VALUE, &attribs);
    if (!NT_SUCCESS(rc))
        return 0;

    len = sizeof(KEY_VALUE_PARTIAL_INFORMATION) + length;
    pinfo = ExAllocatePool(NonPagedPool, len);
    rc = ZwQueryValueKey(handle, &valname, KeyValuePartialInformation,
        pinfo, len, &reslen);
    if ((NT_SUCCESS(rc) || rc == STATUS_BUFFER_OVERFLOW) &&
        reslen >= (sizeof(KEY_VALUE_PARTIAL_INFORMATION) - 1) &&
        (!type || pinfo->Type == type))
    {
        reslen = pinfo->DataLength;
        memcpy(data, pinfo->Data, min(length, reslen));
    }
    else
        reslen = 0;
    ExFreePool(pinfo);

    ZwClose(handle);
    return reslen;
}

NTSTATUS WriteToKey(PWSTR registry_path, PWSTR value_name, ULONG type, DWORD data, ULONG length)
{

    UNICODE_STRING valname;
    UNICODE_STRING keyname;
    OBJECT_ATTRIBUTES attribs;
    HANDLE handle;
    NTSTATUS rc;
    ULONG result;

    RtlInitUnicodeString(&valname, registry_path);
    RtlInitUnicodeString(&keyname, value_name);

    InitializeObjectAttributes(&attribs, &valname, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    rc = ZwCreateKey(&handle, KEY_ALL_ACCESS, &attribs, 0, NULL, REG_OPTION_VOLATILE, &result);

    if (!NT_SUCCESS(rc))
        return STATUS_UNSUCCESSFUL;

    rc = ZwSetValueKey(handle, &keyname, 0, type, &data, length);

    if (!NT_SUCCESS(rc))
        STATUS_UNSUCCESSFUL;

    return STATUS_SUCCESS;
}