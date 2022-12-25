#include "NavigationPath.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS NavigationPath
	MAKEGET_C(int, currSegment)
	MAKEGET_C(Vec3, navStart)
	MAKEGET_C(Vec3, navEnd)
	MAKEGET_C(Vec3*, navArray)
	MAKEGET_C(Vec3*, navArrayEnd)
#undef FOCUS_CLASS

	int NavigationPath::GetWaypointNb()
	{
		return ((DWORD)navArrayEnd() - (DWORD)navArray()) / sizeof(Vec3);
	}

	float NavigationPath::GetPathLength()
	{
		float length = 0;
		for (int i = 0; i < GetWaypointNb(); ++i)
			length += navArray()[i + 1].Distance(this->navArray()[i]);

		return length;
	}

	std::vector<Vec2> NavigationPath::GetNavWaypoints2D()
	{
		std::vector<Vec2> res{};
		for (int i = 0; i < GetWaypointNb(); i++)
		{
			Vec2 path = navArray()[i].To2D();
			res.push_back(path);
		}

		return res;
	}

	std::vector<Vec3> NavigationPath::GetNavWaypoints()
	{
		std::vector<Vec3> res{};
		for (int i = 0; i < GetWaypointNb(); i++)
		{
			Vec3 path = navArray()[i];
			res.push_back(path);
		}

		return res;
	}

	void NavigationPath::Draw()
	{
		// TODO
	}
}
}