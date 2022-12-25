#pragma once

#include "Menu.h"
#include "Context.h"
#include "EventHandler.h"


//EXTRA FOR TEST
#include "GameTime.h"
namespace Script {
namespace Plugins {
namespace MenuPlugin {
	bool isInactive = false;
	void OnWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, bool* execute)
	{
		if (message == WM_KEYDOWN && wparam == VK_INSERT)
		{
			g_ctx->mMenuOpened = !g_ctx->mMenuOpened;
		}

		if (g_ctx->mMenuOpened && !isInactive)
		{
			message = WM_ACTIVATE;
			wparam = WA_INACTIVE;
			isInactive = true;
		}
		else if (!g_ctx->mMenuOpened && isInactive)
		{
			message = WM_ACTIVATE;
			wparam = WA_ACTIVE;
			isInactive = false;
		}

		//*execute = true;
	}

	float LastUpdateT = 0;


	void OnPresent()
	{
		if (g_ctx->mMenuOpened)
		{
			Menu::DrawMenus();

			if(g_ctx->mShowConsole)
				g_ctx->mConsole->Draw("Console");
		}
		else if(g_ctx->mShowConsole && g_ctx->mShowConsoleAlways)
			g_ctx->mConsole->Draw("Console");

		if (GetAsyncKeyState(VK_SPACE))
		{
			if (LastUpdateT < Script::Native::GameTime::GetTickCount())
			{
				//here testing code

				LastUpdateT = Script::Native::GameTime::GetTickCount() + 150;
			}
			
		}
	}

	void Initialize()
	{
		Native::EventHandler<Native::EventType::OnWndProc, Native::evtWndProc, HWND, UINT, WPARAM, LPARAM, bool*>::GetInstance()->Add(OnWndProc);
		Native::EventHandler<Native::EventType::OnPresent, Native::evtPresent >::GetInstance()->Add(OnPresent);
	}
}
}
}