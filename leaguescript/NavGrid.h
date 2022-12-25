#pragma once

#include "Common.h"
#include "Vector.h"

namespace Script {
namespace Native {
	class SCRIPT_API NavGrid final
	{
	public:
		NavGrid* Instance();
		float GetHeightForPosition(Vec3 worldPos);
		bool IsWall(Vec3 worldPos);
	};
}
}
