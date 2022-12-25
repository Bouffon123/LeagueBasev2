#include "MissileClient.h"

#include "RiotString.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS MissileClient
	MAKEGET_C(SpellInfo*, spellInfo)

	MAKEGET_C(eSpellSlot, slot)
	MAKEGET_C(unsigned, missileIndex)

	MAKEGET_STR_C(casterName)
	MAKEGET_C(unsigned, casterIndex)
	MAKEGET_C(unsigned, targetIndex)

	MAKEGET_C(Vec3, startPosition)
	MAKEGET_C(Vec3, endPosition)
	MAKEGET_C(Vec3, castPosition)

	MAKEGET_C(float, windupTime)
	MAKEGET_C(float, cooldown)
	MAKEGET_C(float, manaCost)

	MAKEGET_C(float, castStartTime)
	MAKEGET_C(float, castEndTime)
#undef FOCUS_CLASS
}
}