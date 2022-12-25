#pragma once

#include "Common.h"
#include "SpellEnums.h"
#include "SpellInfo.h"
#include "Vector.h"

namespace Script {
namespace Native {
	class SCRIPT_API MissileClient final
	{
	public:
		MAKEGET(SpellInfo*, spellInfo)
		MAKEGET(eSpellSlot, slot)
		MAKEGET(unsigned, missileIndex)
		MAKEGET(std::string, casterName)
		MAKEGET(unsigned, casterIndex)
		MAKEGET(unsigned, targetIndex)
		MAKEGET(Vec3, startPosition)
		MAKEGET(Vec3, endPosition)
		MAKEGET(Vec3, castPosition)
		MAKEGET(float, windupTime)
		MAKEGET(float, cooldown)
		MAKEGET(float, manaCost)
		MAKEGET(float, castStartTime)
		MAKEGET(float, castEndTime)
	};
}
}
