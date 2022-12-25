#include "SpellBook.h"

#include "fp_call.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS SpellBook
	MAKEGET_C(ActiveSpell*, activeSpell)
#undef FOCUS_CLASS

	SpellSlot* SpellBook::GetSpellSlotById(eSpellSlot id)
	{
		return *reinterpret_cast<SpellSlot**>(this + static_cast<unsigned int>(Structures::SpellBook::firstSlot) + 0x4 * static_cast<int>(id));
	}

	SpellState SpellBook::GetSpellState(eSpellSlot slot)
	{
		bool junk{ };

		return static_cast<SpellState>(base::this_call<int>(MAKE_RVA(Addresses::Spellbook::GetSpellState), this, static_cast<int>(slot), &junk));
	}
}
}
