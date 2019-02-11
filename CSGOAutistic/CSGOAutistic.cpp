// main.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "Utilities/Scanner.h"
#include "Utilities/VTHook.h"
#include "Features/esp.h"
#include "CSGOAutistic.h"

#include "hooks.h"

#include <iostream>
#include <cstdio>

typedef DWORD(__stdcall *hkCreateMove)(DWORD a1, DWORD a2);
hkCreateMove oCreateMove;

DWORD __stdcall myCreateMove(DWORD a1, DWORD a2)
{
	std::cout << "ClsUserCmd: 0x" << std::hex << a2 << std::endl;

	auto fCmdClass = reinterpret_cast<DWORD *>(a2);

	std::cout << std::hex << (fCmdClass + 12) << std::endl;
	std::cout << std::hex << *(fCmdClass + 12) << std::endl;
	DWORD *state = (fCmdClass + 12);

	if (!(*state & 0x2) && (GetAsyncKeyState(VK_HOME) & 1))
	{
		*state |= 0x2;
	}

	return oCreateMove(a1, a2);
}

DWORD start()
{
	//std::ios_base::sync_with_stdio(false);

	FILE *fpConsole;
	AllocConsole();

	freopen_s(&fpConsole, "CONOUT$", "wb", stdout);
	freopen_s(&fpConsole, "CONOUT$", "wb", stderr);

	features::esp->init();

	hooks::init();
	hooks::loop(true);
	hooks::clean_up();

	//LPVOID vftpClientModeSharedCsNormal = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0xEC);

	//auto lpClientPanoramaModuleBase = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client_panorama.dll"));
	//auto clsClientModeCSNormal = reinterpret_cast<LPVOID*>(lpClientPanoramaModuleBase + offsetClientModeClass);

	//std::cout << "clsClientModeCSNormal: 0x" << std::hex << clsClientModeCSNormal << std::endl;
	//std::cout << "*clsClientModeCSNormal: 0x" << std::hex << *clsClientModeCSNormal << std::endl;
	//std::cout << "**clsClientModeCSNormal: 0x" << std::hex << *reinterpret_cast<void **>(*clsClientModeCSNormal) << std::endl;

	//std::cout << "Allocated bytes for vtable at 0x" << std::hex << vftpClientModeSharedCsNormal << std::endl;

	////copy magic
	//auto original_clientmode_vtable = reinterpret_cast<LPVOID*>(*clsClientModeCSNormal);
	//auto original_vtable_baseaddr = reinterpret_cast<std::uintptr_t>(*original_clientmode_vtable);
	//std::memcpy(vftpClientModeSharedCsNormal, *original_clientmode_vtable, 0xEC);

	//std::cout << "ORIGINAL: 0x" << std::hex << original_vtable_baseaddr << std::endl;

	//MEMORY_BASIC_INFORMATION memoryInfo = { 0 };

	//VirtualQuery(clsClientModeCSNormal, &memoryInfo, sizeof(MEMORY_BASIC_INFORMATION));

	//std::cout << "Protect: 0x" << std::hex << memoryInfo.Protect << std::endl;
	//// replace vft pointer
	//auto lppvClientModeCSNormalVTable = reinterpret_cast<void ***>(*clsClientModeCSNormal);
	//*lppvClientModeCSNormalVTable = reinterpret_cast<void **>(vftpClientModeSharedCsNormal);

	//std::cout << "VTABLE: 0x" << std::hex << (*lppvClientModeCSNormalVTable) + 24 << std::endl;
	//std::cout << "VTABLE: 0x" << std::hex << (*lppvClientModeCSNormalVTable)[24] << std::endl;

	//DWORD oldFuncAddress = reinterpret_cast<DWORD>((*lppvClientModeCSNormalVTable)[24]);
	//std::cout << std::hex << oldFuncAddress << std::endl;

	//oCreateMove = reinterpret_cast<hkCreateMove>(oldFuncAddress);
	//std::cout << "oCreateMove stored in: 0x" << std::hex << oCreateMove << std::endl;
	//std::cout << "oCreateMove stored in: 0x" << std::hex << &oCreateMove << std::endl;

	//std::cout << "MyCreateMove: 0x" << std::hex << myCreateMove << std::endl;
	//std::cout << "MyCreateMove: 0x" << std::hex << *reinterpret_cast<void **>(myCreateMove) << std::endl;
	//std::cout << "MyCreateMove: 0x" << std::hex << reinterpret_cast<void **>(myCreateMove) << std::endl;

	//*((*lppvClientModeCSNormalVTable) + 24) = myCreateMove;

	//// Unhook function
	////*((*lppvClientModeCSNormalVTable) + 24) = reinterpret_cast<hkCreateMove *>(oldFuncAddress); // why bother tho

	//// Unhook table
	//*lppvClientModeCSNormalVTable = reinterpret_cast<void **>(original_vtable_baseaddr);

	//HeapFree(GetProcessHeap(), 0, vftpClientModeSharedCsNormal);

	fclose(fpConsole);
	FreeConsole();

	return DWORD(0);
}

