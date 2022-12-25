#include "NavGrid.h"

#include "fp_call.h"

namespace Script {
namespace Native {
	NavGrid* NavGrid::Instance()
	{
		return *reinterpret_cast<NavGrid**>(MAKE_RVA(Addresses::NavGrid::NavGridInstance));
	}

	float NavGrid::GetHeightForPosition(Vec3 worldPos)
	{
		return base::this_call<float>(MAKE_RVA(Addresses::NavGrid::GetHeightForPosition) + 0x4, this, worldPos.x, worldPos.y);
	}

	bool NavGrid::IsWall(Vec3 worldPos)
	{
		return base::c_call<bool>(MAKE_RVA(Addresses::NavGrid::IsNotWall), &worldPos, 1);
	}
}
}