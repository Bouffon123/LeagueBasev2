#pragma once

#include "Common.h"

namespace Script {
namespace Native {
	class SCRIPT_API ItemData final
	{
	public:
		MAKEGET(int, itemId)
		MAKEGET(float, abilityHaste)
		MAKEGET(float, health)
		MAKEGET(float, movementSpeed)
		MAKEGET(float, armor)
		MAKEGET(float, magicResistance)
		MAKEGET(float, ad)
		MAKEGET(float, ap)
		MAKEGET(float, attackSpeed)
	};
}
}