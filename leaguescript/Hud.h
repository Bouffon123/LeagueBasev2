#pragma once

#include "Common.h"
#include "Cursor.h"

namespace Script {
namespace Native {
	class SCRIPT_API Hud final
	{
	public:
		static Hud* Instance();
		MAKEGET(Cursor*, cursor)
		MAKEGET(bool, isWindowFocused)
	};
}
}