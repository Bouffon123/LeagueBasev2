#include "Hud.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS Hud
	MAKEGET_C(Cursor*, cursor)
#undef FOCUS_CLASS

	Hud* Hud::Instance()
	{
		return *reinterpret_cast<Hud**>(MAKE_RVA(Addresses::Hud::HudInstance));
	}

	bool Hud::isWindowFocused()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + static_cast<DWORD>(Structures::Hud::isWindowFocused)) == 1;
	}
}
}