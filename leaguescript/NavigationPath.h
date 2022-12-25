#pragma once

#include "Common.h"
#include "Vector.h"

namespace Script {
namespace Native {
	class SCRIPT_API NavigationPath final
	{
		// memory allocation
		char pad[0x40];

	public:
		MAKEGET(int, currSegment)
		MAKEGET(Vec3, navStart)
		MAKEGET(Vec3, navEnd)
		MAKEGET(Vec3*, navArray)
		MAKEGET(Vec3*, navArrayEnd)

		int					GetWaypointNb();
		float				GetPathLength();
		std::vector<Vec3>	GetNavWaypoints();
		std::vector<Vec2>	GetNavWaypoints2D();
		void				Draw();
	};
}
}
