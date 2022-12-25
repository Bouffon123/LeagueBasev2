#pragma once

#include "Common.h"
#include "SpellInfo.h"
#include "TargetClient.h"

namespace Script {
namespace Native {
	class SCRIPT_API SpellSlot final
	{
	public:
		MAKEGET(float, chargeTime)
		MAKEGET(int, level)
		MAKEGET(float, readyAt)
		MAKEGET(int, charges)
		MAKEGET(float, damage)
		MAKEGET(TargetClient*, targetClient)
		MAKEGET(SpellInfo*, spellInfo)

		float Cooldown();
	};
}
}