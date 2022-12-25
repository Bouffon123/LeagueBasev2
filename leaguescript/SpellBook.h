#pragma once

#include "SpellSlot.h"
#include "Common.h"
#include "SpellEnums.h"
#include "ActiveSpell.h"

namespace Script {
namespace Native {
	class SCRIPT_API SpellBook final
	{
	public:

		MAKEGET(ActiveSpell*, activeSpell)

		SpellSlot* GetSpellSlotById(eSpellSlot id);
		SpellState GetSpellState(eSpellSlot slot);

		/*
		 * TODO:
		 *	CASTSPELL
		 *	UpdateChargeableSpell ?
		 */
	};
}
}
