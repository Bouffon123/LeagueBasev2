#pragma once

#include "Common.h"
#include "SpellData.h"

namespace Script {
namespace Native {
	class SCRIPT_API SpellInfo final
	{
	public:
		MAKEGET_STR(spellName)
		MAKEGET(SpellData*, spellData)
	};
}
}