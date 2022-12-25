#pragma once

#include "Common.h"
#include "Vector.h"

namespace Script {
namespace Native {
	class SCRIPT_API Cursor final
	{
		MAKEGET(Vec3, position)
		MAKEGET(Vec3, lastClickPosition)
		MAKEGET(unsigned, hoveredUnitId)
	};
}
}
