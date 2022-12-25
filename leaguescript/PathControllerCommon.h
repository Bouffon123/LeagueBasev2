#pragma once

#include "Common.h"
#include "Vector.h"
#include "NavigationPath.h"

namespace Script {
namespace Native {

	class NavigationMesh {};

	class SCRIPT_API PathControllerCommon final
	{
		MAKEGET(NavigationMesh*, navigationMesh)
		MAKEGET(bool, isMoving)
		MAKEGET(NavigationPath*, navigationPath)
		MAKEGET(bool, isDashing)
		MAKEGET(Vec3, serverPos)

		NavigationPath BuildPath(Vec3 from, Vec3 to);
	};
}
}
