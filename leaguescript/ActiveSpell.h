#pragma once

#include "Common.h"
#include "SpellInfo.h"
#include "SpellEnums.h"
#include "Vector.h"

namespace Script {
namespace Native {
	class SCRIPT_API ActiveSpell final
	{
	public:
		MAKEGET(unsigned, srcIndex)
		MAKEGET(unsigned*, targetsArray)
		MAKEGET(unsigned*, targetsArraySize)
		MAKEGET(SpellInfo*, spellInfo)
		MAKEGET(Vec3, startPos)
		MAKEGET(Vec3, endPos)
		MAKEGET(bool, isAuto)
		MAKEGET(bool, isSpell)
		MAKEGET(eSpellSlot, slot)
	};
}
}