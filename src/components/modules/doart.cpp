#include "std_include.hpp"

#define VERSION "0.1"

using namespace game;

namespace components
{
	void draw_custom_hud()
	{
		// pmove speed hud
		if (dvars::doart_hud && dvars::doart_hud->current.enabled)
		{
			const float velo_x = game::cgs->predictedPlayerState.velocity[0];
			const float velo_z = game::cgs->predictedPlayerState.velocity[1];
			const float speed = sqrt(velo_x * velo_x + velo_z * velo_z);

			game::draw_text_with_engine(
				/* x	*/ dvars::doart_hud_x->current.value,
				/* y	*/ dvars::doart_hud_y->current.value,
				/* scaX */ dvars::doart_hud_fontScale->current.value,
				/* scaY */ dvars::doart_hud_fontScale->current.value,
				/* font */ fonts::get_font_for_style(dvars::doart_hud_fontStyle->current.integer),
				/* colr */ dvars::doart_hud_fontColor->current.vector,
				/* txt	*/ utils::va("X: %f\nY: %f\nZ: %f\nYAW: %f\nSPEED: %f",
					game::cgs->predictedPlayerState.origin[0],
					game::cgs->predictedPlayerState.origin[1], 
					game::cgs->predictedPlayerState.origin[2],
					game::cgs->predictedPlayerState.viewangles[1],
					speed
				));
		}
	}

	// hk call to CG_Drawcrosshair in CG_Draw2D ~ only active if cg_draw2d
	__declspec(naked) void draw_custom_hud_stub()
	{
		const static uint32_t func_addr = 0x4311A0;
		const static uint32_t retn_addr = 0x42F6BD;
		__asm
		{
			call	func_addr;
			add		esp, 4;

			pushad;
			call	draw_custom_hud;
			popad;

			jmp		retn_addr;
		}
	}

	__declspec(naked) void killfeed_filter_stub()
	{
		const static uint32_t stock1_addr = 0x4358C1;
		const static uint32_t stock2_addr = 0x43586B;
		const static uint32_t retn_addr = 0x435909;
		
		__asm
		{
			push	eax;
			mov		eax, dvars::doart_killfeed_filter;
			cmp		byte ptr[eax + 12], 1;
			pop		eax;

			je		YUMP;

			cmp		ecx, eax;
			jnz		STOCK_loc;

			jmp		stock2_addr;

		STOCK_loc:
			jmp		stock1_addr;

		YUMP:
			jmp		retn_addr;
		}
	}

	// this one fixes other player's suicides(changing teams etc.) still showing
	__declspec(naked) void killfeed_filter_stub2()
	{
		const static uint32_t stock_addr = 0x4358C1;
		const static uint32_t retn_addr = 0x435909;

		__asm
		{
			push	eax;
			mov		eax, dvars::doart_killfeed_filter;
			cmp		byte ptr[eax + 12], 1;
			pop		eax;

			je		YUMP;

			jmp		STOCK_loc;

		STOCK_loc:
			jmp		stock_addr;

		YUMP:
			jmp		retn_addr;
		}
	}

	__declspec(naked) void CG_DrawDisconnect_stub()
	{
		__asm
		{
			jnz		loc_42FAB3; // overwritten op

			push	eax;
			mov		eax, dvars::doart_lagIcon;
			cmp		byte ptr[eax + 12], 0;
			pop		eax;
			
			je		loc_42FAB3;

			// stock
			push	0x42F945;
			retn;

		loc_42FAB3:
			push	0x42FAB3;
			retn;
		}
	}

	doart::doart() {
		static auto version_str = std::string("DOART Tool v"s + VERSION + "> "s);

		// Remove improper quit popup
		utils::hook::set<BYTE>(0x5773F6, 0xEB);

		// hook in cg_obituary for killfeed filter
		utils::hook(0x435867, killfeed_filter_stub, HOOK_JUMP).install()->quick();
		utils::hook(0x435814, killfeed_filter_stub2, HOOK_JUMP).install()->quick();

		// hook in cg_drawdisconnect to hide connection interrupted icon with dvar
		utils::hook(0x42F93F, CG_DrawDisconnect_stub, HOOK_JUMP).install()->quick();

		// hook call to CG_Drawcrosshair in CG_Draw2D ~ only active if cg_draw2d
		utils::hook(0x42F6B5, draw_custom_hud_stub, HOOK_JUMP).install()->quick();

		// Disable the need for Forward/Backslash for console cmds
		utils::hook::nop(0x46752F, 5);

		dvars::doart_hud = game::Dvar_RegisterBool(
			/* name		*/ "doart_hud",
			/* desc		*/ "Display current player's position.",
			/* default	*/ false,
			/* flags	*/ game::dvar_flags::saved);

		dvars::doart_hud_x = game::Dvar_RegisterFloat(
			/* name		*/ "doart_hud_x",
			/* desc		*/ "Coordinate hud horizontal position",
			/* default	*/ 20.0f,
			/* minVal	*/ -10000.0f,
			/* maxVal	*/ 10000.0f,
			/* flags	*/ game::dvar_flags::saved);

		dvars::doart_hud_y = game::Dvar_RegisterFloat(
			/* name		*/ "doart_hud_y",
			/* desc		*/ "Coordinate hud vertical position",
			/* default	*/ 360.0f,
			/* minVal	*/ -10000.0f,
			/* maxVal	*/ 10000.0f,
			/* flags	*/ game::dvar_flags::saved);

		dvars::doart_hud_fontScale = game::Dvar_RegisterFloat(
			/* name		*/ "doart_hud_fontScale",
			/* desc		*/ "Coordinate hud font scale",
			/* default	*/ 0.75f,
			/* minVal	*/ 0.1f,
			/* maxVal	*/ 10.0f,
			/* flags	*/ game::dvar_flags::saved);

		dvars::doart_hud_fontStyle = game::Dvar_RegisterInt(
			/* name		*/ "doart_hud_fontStyle",
			/* desc		*/ "Coordinate hud font Style",
			/* default	*/ 3,
			/* minVal	*/ 0,
			/* maxVal	*/ 8,
			/* flags	*/ game::dvar_flags::saved);

		dvars::doart_hud_fontColor = game::Dvar_RegisterVec4(
			/* name		*/ "doart_hud_fontColor",
			/* desc		*/ "Coordinate hud font color",
			/* x		*/ 0.0f,
			/* y		*/ 1.0f,
			/* z		*/ 1.0f,
			/* w		*/ 1.0f,
			/* min		*/ 0.0f,
			/* max		*/ 1.0f,
			/* flags	*/ game::dvar_flags::saved);
		
		dvars::doart_killfeed_filter = game::Dvar_RegisterBool(
			/* name		*/ "doart_killfeed_filter",
			/* desc		*/ "Filter killfeed to show only your kills.",
			/* default	*/ false,
			/* flags	*/ game::dvar_flags::saved);

		dvars::doart_lagIcon = game::Dvar_RegisterBool(
			/* name		*/ "doart_lagIcon",
			/* desc		*/ "When set to 0, hides connection interrupted icon.",
			/* default	*/ true,
			/* flags	*/ game::dvar_flags::saved);
	}
}