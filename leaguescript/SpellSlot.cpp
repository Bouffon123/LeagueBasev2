#include "SpellSlot.h"

#include "GameTime.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS SpellSlot
	MAKEGET_C(float, chargeTime)
	MAKEGET_C(int, level)
	MAKEGET_C(float, readyAt)
	MAKEGET_C(int, charges)
	MAKEGET_C(float, damage)
	MAKEGET_C(TargetClient*, targetClient)
	MAKEGET_C(SpellInfo*, spellInfo)
#undef FOCUS_CLASS

	float SpellSlot::Cooldown()
	{
		const auto gameTime = GameTime::GetGameTime();
		return readyAt() > gameTime ? readyAt() - gameTime : 0.f;
	}
}
}