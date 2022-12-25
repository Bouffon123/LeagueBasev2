#pragma once

#include "Common.h"

namespace Script {
namespace Native {
	class SCRIPT_API SpellData final
	{
	public:
		MAKEGET_STR(name)
		MAKEGET(float, missileSpeed)
	};
}
}