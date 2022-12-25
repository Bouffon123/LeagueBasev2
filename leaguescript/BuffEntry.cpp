#include "BuffEntry.h"

#include "RiotString.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS BuffEntry
		MAKEGET_C(BuffType, type)
		MAKEGET_C(float, startTime)
		MAKEGET_C(float, endTime)
		MAKEGET_C(int, countAlt)
		MAKEGET_C(int, countInt)
		MAKEGET_C(int, countFloat)
#undef FOCUS_CLASS

	std::string BuffEntry::name()
	{
		void* buffScript = *reinterpret_cast<void**>((unsigned)this + (unsigned)Structures::BuffEntry::BuffData);
		return reinterpret_cast<LolString*>((unsigned)buffScript + (unsigned)Structures::BuffData::name)->_content;
	}
}
}
