#include "TargetClient.h"

#include "RiotString.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS TargetClient
			MAKEGET_C(int, CasterGameObjectIndex)
			MAKEGET_C(int, TargetGameObjectIndex)
			MAKEGET_C(Vec3, SpellStartPosition)
			MAKEGET_C(Vec3, TargetPosition)
			MAKEGET_C(Vec3, CursorWorldPosition)
#undef FOCUS_CLASS
}
}