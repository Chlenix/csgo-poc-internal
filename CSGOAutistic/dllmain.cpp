// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "CSGOAutistic.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		self = hModule;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE) &MAIN, NULL, 0, NULL);
		break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

