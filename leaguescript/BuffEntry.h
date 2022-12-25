#pragma once

#include "Common.h"
#include "BuffType.h"

namespace Script {
namespace Native {
	class SCRIPT_API BuffEntry final {
	public:
		MAKEGET(BuffType, type)
		MAKEGET(float, startTime)
		MAKEGET(float, endTime)
		MAKEGET(int, countAlt)
		MAKEGET(int, countInt)
		MAKEGET(int, countFloat)

		std::string name();
	};
}
}

