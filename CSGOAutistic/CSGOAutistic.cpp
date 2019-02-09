// main.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Utilities/Scanner.h"
#include "Utilities/VTHook.h"
#include "hooks.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include "index.hpp"

static HANDLE self;

DWORD __stdcall MAIN();

constexpr static std::uint32_t screen_width = 1600;
constexpr static std::uint32_t screen_height = 900;

constexpr static std::uint32_t bar_width = 300;
constexpr static std::uint32_t bar_height = 50;

typedef DWORD(__stdcall *hkCreateMove)(DWORD a1, DWORD a2);
hkCreateMove oCreateMove;

typedef DWORD(__thiscall *hkPaintTraverse)(void *Panel, unsigned int vPanel, bool, bool);
hkPaintTraverse oPaintTraverse;

typedef const char * (__thiscall * hkGetName)(void *, unsigned int);
hkGetName oGetName;

typedef void(__thiscall *fnSetTextPosition)(PVOID _this, int x, int y);
typedef void(__thiscall *fnSetTextRGBA)(PVOID _this, int r, int g, int b, int a);
typedef void(__thiscall *fnSetRGBA)(PVOID _this, int r, int g, int b, int a);
typedef void(__thiscall *fnDrawPrintText)(PVOID _this, wchar_t* text, int len, int wtf);
typedef void(__thiscall *fnDrawFilledRect)(PVOID _this, int x, int y, int w, int h);

fnSetRGBA oSetRGBA;
fnDrawFilledRect oDrawFilledRect;
fnSetTextPosition oSetTextPosition;
fnSetTextRGBA oSetTextRGBA;
fnDrawPrintText oDrawPrintText;

void ***GlobalSurfaceObject;

//constexpr std::ptrdiff_t offsetClientModeClass = 0x051712A4;

//void __fastcall myPaintTraverse(void *Panel, int edx, unsigned int vPanel, bool forceRepaint, bool allowForce)
//{
//	oPaintTraverse(Panel, vPanel, forceRepaint, allowForce);
//
//	static auto FocusOverlayPanel = 0;
//	static bool FoundPanel = false;
//
//	if (!FoundPanel)
//	{
//		const auto panelName = oGetName(Panel, vPanel);
//
//		if (!strcmp(panelName, "FocusOverlayPanel")) // or MatSystemTopPanel
//		{
//			FocusOverlayPanel = vPanel;
//			FoundPanel = true;
//		}
//		else
//			return;
//	}
//
//	if (FocusOverlayPanel != vPanel)
//	{
//		return;
//	}
//
//	std::size_t const maxSize = 100;
//	wchar_t aw[maxSize] = L"PRINT ME";
//	std::size_t h = wcsnlen_s(aw, maxSize);
//
//	//oSetTextRGBA(GlobalSurfaceObject, 0, 0, 0, 255);
//	//oSetTextPosition(GlobalSurfaceObject, 200, 200);
//	//oDrawPrintText(GlobalSurfaceObject, aw, h, 0);
//
//	static int x = int(screen_width / 2) - int(bar_width / 2);
//	static int y = (screen_height / 2) - int(bar_height / 2);
//
//	oSetRGBA(GlobalSurfaceObject, 0, 0, 0, 255);
//	oDrawFilledRect(GlobalSurfaceObject, x, y, x + bar_width, y + bar_height);
//}

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

DWORD __stdcall MAIN()
{
	//std::ios_base::sync_with_stdio(false);

	FILE *fpConsole;
	AllocConsole();

	freopen_s(&fpConsole, "CONOUT$", "wb", stdout);
	freopen_s(&fpConsole, "CONOUT$", "wb", stderr);

	hooks::init();

	//GlobalSurfaceObject = reinterpret_cast<void ***>(GetInterface("vguimatsurface.dll", "VGUI_Surface031"));
	//auto vgui_object = reinterpret_cast<void ***>(interfaces::make("vgui2.dll", "VGUI_Panel009"));

	//auto surface_vft = *GlobalSurfaceObject;
	
	//utils::vt::VTMananger vgui_vtable(vgui_object, 0x10C);

	//vgui_vtable.prepare_function(myPaintTraverse, 41);
	//vgui_vtable.commit_hook();

	//Sleep(5000);
	//vgui_vtable.release_hook();

	//std::cout << "Released" << std::endl;

	//while (1)
	//{
	//	if (GetAsyncKeyState(VK_END))
	//	{
	//		std::cout << "Released" << std::endl;
	//		break;
	//	}

	//	Sleep(10);
	//}

	//auto vgui_vft_original = reinterpret_cast<void **>(reinterpret_cast<std::uintptr_t>(*vgui_object));

	//oPaintTraverse = reinterpret_cast<hkPaintTraverse>(vgui_vft_original[41]);
	//oGetName = reinterpret_cast<hkGetName>(vgui_vft_original[36]);

	//std::cout << "oPaintTraverse: 0x" << std::hex << oPaintTraverse << std::endl;
	//std::cout << "GlobalSurfaceObject: 0x" << std::hex << GlobalSurfaceObject << std::endl;
	//std::cout << "surface_vft: 0x" << std::hex << surface_vft << std::endl;

	//oSetRGBA = getvfunc<fnSetRGBA>(surface_vft, 15);
	//oDrawFilledRect = getvfunc<fnDrawFilledRect>(surface_vft, 16);
	//oSetTextRGBA = getvfunc<fnSetTextRGBA>(surface_vft, 25);
	//oSetTextPosition = getvfunc<fnSetTextPosition>(surface_vft, 26);
	//oDrawPrintText = getvfunc<fnDrawPrintText>(surface_vft, 28);

	////std::size_t const maxSize = 100;
	////wchar_t aw[maxSize] = L"PRINT ME";
	////std::size_t h = wcsnlen_s(aw, maxSize);

	//void *vgui_vft_hooked = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x10C);

	//std::cout << "Allocated bytes for vtable at 0x" << std::hex << vgui_vft_hooked << std::endl;
	//std::cout << "Original at 0x" << std::hex << vgui_vft_original << std::endl;
	//std::memcpy(vgui_vft_hooked, *vgui_object, 0x10C);

	//reinterpret_cast<void **>(vgui_vft_hooked)[41] = reinterpret_cast<void *>(myPaintTraverse);
	//*vgui_object = reinterpret_cast<void **>(vgui_vft_hooked);

	//std::cout << "MyPaintTraverse 0x" << std::hex << myPaintTraverse << std::endl;
	//std::cout << "oPaintTraverse: 0x" << std::hex << oPaintTraverse << std::endl;

	//// TODO: NEEDS TESTING BELOW THIS POINT
	//// COMMENT OUT IF IT BUGS
	//while (1)
	//{
	//	if (GetAsyncKeyState(VK_END))
	//	{
	//		std::cout << "Released" << std::endl;
	//		break;
	//	}
	//	Sleep(10);
	//}

	//*vgui_object = vgui_vft_original;
	//HeapFree(GetProcessHeap(), 0, vgui_vft_hooked);

	/////////////////////////////////////////////////

	//oSetColor(surfaceObject, 0, 0, 0, 255);
	//oDrawFilledRect(surfaceObject, 100, 100, 100, 100);

	//oSetTextRGBA(surfaceObject, 0, 0, 0, 255);
	//oSetTextPosition(surfaceObject, 400, 300);
	//oDrawPrintText(surfaceObject, aw, h, 0);

	//auto Start = std::chrono::high_resolution_clock::now();

	//while (1)
	//{
	//	oSetColor(surfaceObject, 0, 0, 0, 255);
	//	oDrawFilledRect(surfaceObject, 100, 100, 100, 100);

	//	auto End = std::chrono::high_resolution_clock::now();
	//	std::chrono::duration<double, std::milli> Elapsed = End - Start;
	//	if (Elapsed.count() >= 5000.0)
	//	{
	//		std::cout << "Done" << std::endl;
	//		break;
	//	}
	//}

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

	//std::ptrdiff_t dwEntityListOffset = utilities::scanner::FindPattern("client_panorama.dll", "x????xxxxx?x", "\xBB\x11\x11\x11\x11\x90\x83\xFF\x01\x7C\x11\xA1", 1);
	//std::cout << "entity list offset: 0x" << std::hex << dwEntityListOffset << std::endl;

	//HeapFree(GetProcessHeap(), 0, vftpClientModeSharedCsNormal);

	fclose(fpConsole);
	FreeConsole();

	return DWORD(0);
}

