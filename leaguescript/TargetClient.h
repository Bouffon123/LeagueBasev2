#pragma once

#include "Common.h"

namespace Script {
	namespace Native {
		class SCRIPT_API TargetClient final
		{
		public:
			MAKEGET(int, CasterGameObjectIndex)
			MAKEGET(int, TargetGameObjectIndex)
			MAKEGET(Vec3, SpellStartPosition)
			MAKEGET(Vec3, TargetPosition)
			MAKEGET(Vec3, CursorWorldPosition)
		};
}
}