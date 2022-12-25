#include "SpellInfo.h"

#include "RiotString.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS SpellInfo
	MAKEGET_STR_C(spellName)
	MAKEGET_C(SpellData*, spellData)
#undef FOCUS_CLASS
}
}