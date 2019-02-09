#include "hooks.h"
#include "interfaces.h"

#include "stdafx.h"

namespace hooks
{	
	constexpr static std::uint32_t screen_width = 1600;
	constexpr static std::uint32_t screen_height = 900;

	constexpr static std::uint32_t bar_width = 300;
	constexpr static std::uint32_t bar_height = 50;

	std::unique_ptr<utils::vt::VTMananger> vgui_manager;
	std::unique_ptr<utils::vt::VTMananger> surface_manager;

	void init();
	void hook_vgui();
	void hook_surface();

	void __fastcall myPaintTraverse(void *, int, unsigned int, bool, bool);
	void __stdcall paint_traverse(unsigned int vPanel, bool forceRepaint, bool allowForce);


#pragma region TYPEDEF_FUNCTIONS
	/* VGUI */
	typedef DWORD(__thiscall *hkPaintTraverse)(void *Panel, unsigned int vPanel, bool, bool);
	typedef const char* (__thiscall *hkGetName)(void *, unsigned int);

	/* SURFACE */
	typedef void(__thiscall *fnSetTextPosition)(PVOID _this, int x, int y);
	typedef void(__thiscall *fnSetTextRGBA)(PVOID _this, int r, int g, int b, int a);
	typedef void(__thiscall *fnSetRGBA)(PVOID _this, int r, int g, int b, int a);
	typedef void(__thiscall *fnDrawPrintText)(PVOID _this, wchar_t* text, int len, int wtf);
	typedef void(__thiscall *fnDrawFilledRect)(PVOID _this, int x, int y, int w, int h);
#pragma endregion
	
	void init()
	{
		hook_vgui();
		hook_surface();

		Sleep(3000);
		vgui_manager->release_hook();
		surface_manager->release_hook();

		std::cout << "released" << std::endl;
		std::cout << vgui_manager->get_value() << std::endl;
	}

	void hook_surface()
	{
		std::uintptr_t surface = interfaces::make("vguimatsurface.dll", "VGUI_Surface031");
		surface_manager = std::make_unique<utils::vt::VTMananger>(surface, 0x4);
	}

	void hook_vgui()
	{
		std::uintptr_t vgui = interfaces::make("vgui2.dll", "VGUI_Panel009");

		vgui_manager = std::make_unique<utils::vt::VTMananger>(vgui, 0x10C);

		vgui_manager->prepare_function(paint_traverse, 41);

		vgui_manager->commit_hook();
	}

#pragma region HOOKED_FUNCTIONS

	void __stdcall paint_traverse(unsigned int vgui_panel_index, bool force_repaint, bool allow_force)
	{
		vgui_manager->get_original_vfunc<hkPaintTraverse>(41)(vgui_manager->get_class(), vgui_panel_index, force_repaint, allow_force);

		static auto focus_overlay_panel = 0;
		static bool found = false;

		if (!found)
		{
			const auto panel_name = vgui_manager->get_original_vfunc<hkGetName>(36)(vgui_manager->get_class(), vgui_panel_index);

			if (!std::strcmp(panel_name, "FocusOverlayPanel")) // or MatSystemTopPanel
			{
				focus_overlay_panel = vgui_panel_index;
				found = true;
			}
			else
				return;
		}

		if (focus_overlay_panel != vgui_panel_index)
		{
			return;
		}

		std::size_t const maxSize = 100;
		wchar_t aw[maxSize] = L"PRINT ME";
		std::size_t h = wcsnlen_s(aw, maxSize);

		auto surface_object = surface_manager->get_class();

		surface_manager->get_original_vfunc<fnSetTextRGBA>(25)(surface_object, 0, 0, 0, 255);

		surface_manager->get_original_vfunc<fnSetTextPosition>(26)(surface_object, 200, 200);
		surface_manager->get_original_vfunc<fnDrawPrintText>(28)(surface_object, aw, h, 0);

		static int x = int(screen_width / 2) - int(bar_width / 2);
		static int y = (screen_height / 2) - int(bar_height / 2);

		surface_manager->get_original_vfunc<fnSetRGBA>(15)(surface_object, 0, 0, 0, 255);
		surface_manager->get_original_vfunc<fnDrawFilledRect>(16)(surface_object, x, y, x + bar_width, y + bar_height);
	}

	void __fastcall myPaintTraverse(void *Panel, int edx, unsigned int vPanel, bool forceRepaint, bool allowForce)
	{
		//reinterpret_cast<hkPaintTraverse>(vgui_manager->get_original_f<int>(41))(Panel, vPanel, forceRepaint, allowForce);

		vgui_manager->get_original_vfunc<hkPaintTraverse>(41)(Panel, vPanel, forceRepaint, allowForce);

		vgui_manager->set_test_value(0x30);

		//static auto FocusOverlayPanel = 0;
		//static bool FoundPanel = false;

		//if (!FoundPanel)
		//{
		//	const auto panelName = vgui_manager->get_original_vfunc<hkGetName>(36)(Panel, vPanel);

		//	if (!strcmp(panelName, "FocusOverlayPanel")) // or MatSystemTopPanel
		//	{
		//		FocusOverlayPanel = vPanel;
		//		FoundPanel = true;
		//	}
		//	else
		//		return;
		//}

		//if (FocusOverlayPanel != vPanel)
		//{
		//	return;
		//}

		//std::size_t const maxSize = 100;
		//wchar_t aw[maxSize] = L"PRINT ME";
		//std::size_t h = wcsnlen_s(aw, maxSize);

		//oSetTextRGBA(GlobalSurfaceObject, 0, 0, 0, 255);
		//oSetTextPosition(GlobalSurfaceObject, 200, 200);
		//oDrawPrintText(GlobalSurfaceObject, aw, h, 0);

		//static int x = int(screen_width / 2) - int(bar_width / 2);
		//static int y = (screen_height / 2) - int(bar_height / 2);

		//oSetRGBA(GlobalSurfaceObject, 0, 0, 0, 255);
		//oDrawFilledRect(GlobalSurfaceObject, x, y, x + bar_width, y + bar_height);
	}
#pragma endregion
}