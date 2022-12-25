#pragma once

#include "Common.h"

namespace Script {
namespace Native {
	class SCRIPT_API Globals
	{
	public:

		static char* GameVersion();
		static bool IsScriptVersionLatest();
	};
}
}