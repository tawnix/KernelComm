#include "RegistryKeyData.h"
#include "GameInfo.h"
/*
short int	2	-32,768 to 32,767

unsigned short int	2	0 to 65,535

unsigned int	4	0 to 4,294,967,295

int	4	-2,147,483,648 to 2,147,483,647
*/

// MAKE SURE TO RUN EXE AS ADMIN
//spent like 2 days debugging and it turned out to be admin rights to change HKEY_LOCAL_MACHINE

int main()
{


	//Setup game information.
	std::cout << "[+]Setting up information for driver...\n\n";

	DWORD ProcessID = get_process_id("hl2.exe");
	GetClientDllBase((char*)"client.dll", ProcessID);


	//send address to registry key
	WriteRegKey(ClientBase + LOCAL_PLAYER, "Address");
	std::cout << "[+]Sending Read code in 10 seconds...\n\n";
	Sleep(10000);


	std::cout << "[+]Telling driver to read memory now\n\n";
	WriteRegKey(READ, "Code");

	Sleep(100);

	std::cout << "[+] Grabbing local player\n";
	DWORD LocalPlayer = ReadRegKey("Address");
	std::cout << "[+]LocalPlayer addy: " << LocalPlayer << "\n\n";



	std::cout << "[+] Giving kernel LocalPlayer + 0x90\n";
	// now try to read player health
	WriteRegKey(LocalPlayer + 0x90, "Address");
	WriteRegKey(READ, "Code");
	std::cout << "[+] Set key to :" << LocalPlayer + 0x90 << "\n\n";


	Sleep(100);

	std::cout << "[+] Grabbing health now\n";
	// now grab the address key again it should be set to health
	int Health = ReadRegKey("Address");

	std::cout << "[+] Health = " << Health << "\n\n";

	while (true)
	{
		Sleep(500);
	}	

	return 0;
}