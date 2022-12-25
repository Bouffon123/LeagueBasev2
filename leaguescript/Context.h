#pragma once

#include <d3d9.h>

#include "DrawManager.h"
#include "Imgui/Console.h"

class Menu;

class Context
{
public:
	HWND GameHWND;

	LPDIRECT3DDEVICE9 mdx9Device;
	bool mInitializedUI = false;
	bool mMenuOpened = false;

	GameConsole* mConsole;
	DrawManager* mDrawManager;

	std::vector<Menu*> mRootMenus;
	std::vector<Menu*> mApiMenus;

	unsigned mModuleBase;
	unsigned mD3d9Base;

#ifdef __DEV
	bool mShowConsole = true;
	bool mShowConsoleAlways = true;
#else
	bool mShowConsole = false;
	bool mShowConsoleAlways = false;
#endif
};
extern Context* g_ctx;