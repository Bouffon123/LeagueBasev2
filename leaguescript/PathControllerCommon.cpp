#include "PathControllerCommon.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS PathControllerCommon
	MAKEGET_C(NavigationMesh*, navigationMesh)
	MAKEGET_C(bool, isMoving)
	MAKEGET_C(NavigationPath*, navigationPath)
	MAKEGET_C(bool, isDashing)
	MAKEGET_C(Vec3, serverPos)
#undef FOCUS_CLASS
}
}