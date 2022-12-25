#include "ProcessedSpell.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS ProcessedSpell
	MAKEGET_C(unsigned, srcIndex)
	MAKEGET_C(unsigned*, targetsArray)
	MAKEGET_C(unsigned*, targetsArraySize)
	MAKEGET_C(SpellInfo*, spellInfo)
	MAKEGET_C(Vec3, startPos)
	MAKEGET_C(Vec3, endPos)
	MAKEGET_C(bool, isAuto)
	MAKEGET_C(bool, isSpell)
	MAKEGET_C(eSpellSlot, slot)
#undef FOCUS_CLASS
}
}