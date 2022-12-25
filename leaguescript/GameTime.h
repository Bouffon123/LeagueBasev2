#pragma once

#include "Common.h"

namespace Script {
namespace Native {
	class SCRIPT_API GameTime final {
	public:
		static float GetGameTime();
		static int GetTickCount();
	};
}
}