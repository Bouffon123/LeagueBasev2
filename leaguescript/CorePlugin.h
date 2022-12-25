#pragma once

#include "Menu.h"
#include "Context.h"
#include "EventHandler.h"

namespace Script {
namespace Plugins {
namespace CorePlugin {
	void Initialize()
	{
		auto rootMenu = Menu::CreateMenu("coremenu", ICON_FA_HEART "   Core");
		rootMenu->AddHeader("Core");
		rootMenu->AddCheckbox("showconsole", "Show console", &g_ctx->mShowConsole);
		rootMenu->AddCheckbox("showconsole_always", "Show console when menu closed", &g_ctx->mShowConsoleAlways);
	}
}
}
}