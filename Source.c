#include <ntifs.h>
#include "messages.h"
#include "Data.h"

//======forward declarations========
void Unload(_In_ PDRIVER_OBJECT DriverObject);
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo);
NTSTATUS SetupRegKeys();
//======forward declarations========

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	// Call unref param to ignore stupid error
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status = 0;

	PrintMsg("Driver loaded...\n");
	
	PsSetLoadImageNotifyRoutine(ImageLoadCallback);

	//setup registry keys
	status = SetupRegKeys();

	if (!NT_SUCCESS(status))
		PrintMsg("Error creating RegKeys...\n");
	else
		PrintMsg("Created RegKeys successfully!\n");

	//TODO: Setup Function to continuously read RegKey ReadOrWriteKey
	mainLoop();

	// specify the driver unload function
	DriverObject->DriverUnload = Unload;
	return STATUS_SUCCESS;
}



void Unload(_In_ PDRIVER_OBJECT DriverObject)
{
	// call this or get bsod
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
	PrintMsg("Driver unloaded\n");
}

NTSTATUS SetupRegKeys()
{
	DWORD CodeReadOrWrite = 0;
	QWORD AddressForGame =  0;

	// Just picked a random registry path here
	RtlInitUnicodeString(&KeyPath, L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	RtlInitUnicodeString(&KeyName, L"Code"); // made Code the name of the Key so kernel / um can read & write Key
	RtlInitUnicodeString(&KeyPath2, L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	RtlInitUnicodeString(&Address, L"Address");

	InitializeObjectAttributes(&Obj_Attr, &KeyPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, hKey, NULL);
	InitializeObjectAttributes(&Obj_Attr2, &KeyPath2, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, hKey2, NULL);

	Status = ZwCreateKey(&hKey, KEY_ALL_ACCESS, &Obj_Attr, 0, NULL, REG_OPTION_VOLATILE, &ulResult);
	Status2 = ZwCreateKey(&hKey2, KEY_ALL_ACCESS, &Obj_Attr2, 0, NULL, REG_OPTION_VOLATILE, &ulResult2);

	if (!NT_SUCCESS(Status) && !NT_SUCCESS(Status2))
		return STATUS_UNSUCCESSFUL;
	else
		PrintMsg("Keys Created!\n");

	Status = ZwSetValueKey(hKey, &KeyName, 0, REG_DWORD, &CodeReadOrWrite, sizeof(CodeReadOrWrite));
	Status2 = ZwSetValueKey(hKey2, &Address, 0, REG_QWORD, &AddressForGame, sizeof(AddressForGame));
	
	if (NT_SUCCESS(Status) && NT_SUCCESS(Status2))
		PrintMsg("Set Value for key!\n");
	else
		return STATUS_UNSUCCESSFUL;

	return STATUS_SUCCESS;
}



// Use this function to grab processID or client.dll loaded into the system

// this function just grabs every module thats loaded after kenrnel is loaded.
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName,
	HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	if (wcsstr(FullImageName->Buffer, L"\\GarrysMod\\garrysmod\\bin\\client.dll"))
	{
		DbgPrintEx(0, 0, "Found ProcessID: %d", ProcessId);
		ProcessID = ProcessId;
	}
}