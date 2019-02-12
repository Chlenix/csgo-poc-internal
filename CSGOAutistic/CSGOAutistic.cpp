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

	fclose(fpConsole);
	FreeConsole();

	return DWORD(0);
}

