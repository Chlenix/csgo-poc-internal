#include "index.h"
#include "hooks.h"
#include "interfaces.h"

#include "Features/esp.h"
#include "Features/AimTracker.h"

#include "stdafx.h"
#include "Utilities/Scanner.h"
#include "sdk.h"

namespace hooks
{	
	constexpr static std::uint32_t screen_width = 1600;
	constexpr static std::uint32_t screen_height = 900;

	constexpr static std::uint32_t bar_width = 300;
	constexpr static std::uint32_t bar_height = 50;

	std::unique_ptr<utils::vt::VTMananger> vgui_manager = nullptr;
	std::unique_ptr<utils::vt::VTMananger> surface_manager = nullptr;
	std::unique_ptr<utils::vt::VTMananger> vengine_manager = nullptr;

	std::unique_ptr<utils::vt::VTMananger> clientmode_manager = nullptr;

	bool HK create_move(float input_sample_time, DWORD *user_cmd);
	void __stdcall paint_traverse(unsigned int vPanel, bool forceRepaint, bool allowForce);
	
	void clean_up()
	{
		// release clientmode first
		clientmode_manager->release_hook();

		vgui_manager->release_hook();
		surface_manager->release_hook();
		vengine_manager->release_hook();

		std::cout << "released" << std::endl;
	}

	void loop(bool condition)
	{
		while (condition)
		{

			//features::aim_tracker->track();

			if (GetAsyncKeyState(VK_END))
			{
				std::cout << "Releasing ..." << std::endl;
				break;
			}

			//features::aim_tracker->track();

			Sleep(10);
		}
	}

	void init()
	{
		std::uintptr_t entity_list_offset = utils::scanner::FindPattern("client_panorama.dll", utils::masks::entity_list, utils::patterns::entity_list, 1);
		auto client_panorama = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client_panorama.dll"));
		valve::sdk::EntityList entity_list = reinterpret_cast<valve::sdk::EntityList>(client_panorama + entity_list_offset);

		std::cout << "WTF: 0x" << std::hex << entity_list_offset << std::endl;

		features::aim_tracker = std::make_unique<features::AimTracker>(entity_list->entity, entity_list, reinterpret_cast<utils::vt::VTMananger *>(vengine_manager.get()));

		std::cout << "AimTracker on: " << features::aim_tracker->is_enabled() << std::endl;
		
		// BIG TEST BOYS
		hook_vengine();

		hook_vgui();
		hook_surface();

		// hook clientmode's createmove final
		hook_clientmode();

		std::cout << "hooked" << std::endl;
	}

	void hook_surface()
	{
		std::uintptr_t surface = interfaces::make("vguimatsurface.dll", "VGUI_Surface031");
		surface_manager = std::make_unique<utils::vt::VTMananger>(surface);
	}

	void hook_vengine()
	{
		std::uintptr_t vengine = interfaces::make("engine.dll", "VEngineClient014");
		vengine_manager = std::make_unique<utils::vt::VTMananger>(vengine);
	}

	void hook_vgui()
	{
		std::uintptr_t vgui = interfaces::make("vgui2.dll", "VGUI_Panel009");
		vgui_manager = std::make_unique<utils::vt::VTMananger>(vgui, 0x10C);

		vgui_manager->prepare_function(paint_traverse, index::vgui::paint_traverse);
		vgui_manager->commit_hook();
	}

	void hook_clientmode()
	{
		std::ptrdiff_t clientmode_offset = utils::scanner::FindPattern("client_panorama.dll", utils::masks::clientmode_shared, utils::patterns::clientmode_shared, 1);
		auto panorama_module = reinterpret_cast<std::uintptr_t>(GetModuleHandleA("client_panorama.dll"));

		clientmode_manager = std::make_unique<utils::vt::VTMananger>(*reinterpret_cast<utils::vt::pvftable *>(panorama_module + clientmode_offset), 0x11C);
		
		clientmode_manager->prepare_function(create_move, index::clientmode::create_move);
		clientmode_manager->commit_hook();
	}

#pragma region HOOKED_FUNCTIONS

	void __stdcall paint_traverse(unsigned int vgui_panel_index, bool force_repaint, bool allow_force)
	{
		// call original function
		vgui_manager->get_original_vfunc<hkPaintTraverse>(index::vgui::paint_traverse)
			(vgui_manager->get_class(), vgui_panel_index, force_repaint, allow_force);

		static auto focus_overlay_panel = 0;
		static bool found = false;

		if (!found)
		{
			const auto panel_name = vgui_manager->get_original_vfunc<hkGetName>(index::vgui::get_name)(vgui_manager->get_class(), vgui_panel_index);

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

		features::esp->render();

		//std::size_t const maxSize = 100;
		//wchar_t aw[maxSize] = L"PRINT ME";
		//std::size_t h = wcsnlen_s(aw, maxSize);

		//auto surface_object = surface_manager->get_class();

		//surface_manager->get_original_vfunc<fnSetTextRGBA>(index::surface::set_text_color)(surface_object, 0, 0, 0, 255);

		//surface_manager->get_original_vfunc<fnSetTextPosition>(index::surface::set_text_position)(surface_object, 200, 200);
		//surface_manager->get_original_vfunc<fnDrawPrintText>(index::surface::draw_print_text)(surface_object, aw, h, 0);

		//static int x = int(screen_width / 2) - int(bar_width / 2);
		//static int y = (screen_height / 2) - int(bar_height / 2);

		//surface_manager->get_original_vfunc<fnSetRGBA>(index::surface::set_color)(surface_object, 0, 0, 0, 255);
		//surface_manager->get_original_vfunc<fnDrawFilledRect>(index::surface::draw_filled_rect)(surface_object, x, y, x + bar_width, y + bar_height);
	}

	bool HK create_move(float input_sample_time, DWORD *user_cmd)
	{
		clientmode_manager->get_original_vfunc<hkCreateMove>(index::clientmode::create_move)
			(clientmode_manager->get_class(), input_sample_time, user_cmd);

		DWORD command_number = *(user_cmd + 1);

		if (!command_number)
		{
			return true;
		}

		/* Logic Block: */
		features::aim_tracker->track();

		/* Logic Block End */

		return false;
	}
#pragma endregion
}