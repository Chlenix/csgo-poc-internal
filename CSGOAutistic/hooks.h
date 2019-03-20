#pragma once

#include "Utilities/VTHook.h"
#include <memory>
#include <Windows.h>

#define HK __stdcall

namespace hooks
{
	extern std::unique_ptr<utils::vt::VTMananger> vgui_manager;
	extern std::unique_ptr<utils::vt::VTMananger> surface_manager;
	extern std::unique_ptr<utils::vt::VTMananger> vengine_manager;
	extern std::unique_ptr<utils::vt::VTMananger> clientmode_manager;

#pragma region TYPEDEF_FUNCTIONS
	/* CLIENTMODE */
	typedef bool(__thiscall *hkCreateMove)(PVOID _this, float input_sample_time, DWORD *user_cmd);

	/* VGUI */
	typedef DWORD(__thiscall *hkPaintTraverse)(void *Panel, unsigned int vPanel, bool, bool);
	typedef const char* (__thiscall *hkGetName)(void *, unsigned int);

	/* SURFACE */
	typedef void(__thiscall *fnSetTextPosition)(PVOID _this, int x, int y);
	typedef void(__thiscall *fnSetTextRGBA)(PVOID _this, int r, int g, int b, int a);
	typedef void(__thiscall *fnSetRGBA)(PVOID _this, int r, int g, int b, int a);
	typedef void(__thiscall *fnDrawPrintText)(PVOID _this, wchar_t* text, int len, int wtf);
	typedef void(__thiscall *fnDrawFilledRect)(PVOID _this, int x, int y, int w, int h);
	typedef void(__thiscall *fnDrawOutlinedRect)(PVOID _this, int x, int y, int w, int h);

	typedef void(__thiscall *fnGetViewAngles)(PVOID _this, float *outAngles);
	typedef void(__thiscall *fnSetViewAngles)(PVOID _this, float *angles);
#pragma endregion

	void init();
	void clean_up();
	void loop(bool condition);

	void hook_vgui();
	void hook_surface();
	void hook_vengine();
	void hook_clientmode();
}