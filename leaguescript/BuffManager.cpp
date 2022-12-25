#include "BuffManager.h"

#include "GameTime.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS BuffManager
	MAKEGET_C(DWORD, buffEntriesArray)
	MAKEGET_C(DWORD, buffEntriesArrayEnd) //
#undef FOCUS_CLASS


	bool buffFilter(BuffEntry* buff);

	std::vector<BuffEntry*> BuffManager::GetBuffList()
	{
		std::vector<BuffEntry*> buffs{};

		for (DWORD pBuffPtr = buffEntriesArray(); pBuffPtr != buffEntriesArrayEnd(); pBuffPtr += 0x8)
		{
			BuffEntry* pBuff = *(BuffEntry**)pBuffPtr;
			if (!buffFilter(pBuff))
				continue;

			buffs.push_back(pBuff);
		}

		return buffs;
	}

	BuffEntry* BuffManager::GetBuffEntryByName(std::string buffName)
	{
		std::vector<BuffEntry*> buffs = GetBuffList();

		for (BuffEntry*& buff : buffs)
		{
			if (!strcmp(buffName.c_str(), buff->name().c_str()))
				return buff;
		}

		return nullptr;
	}

	bool BuffManager::HasBuffType(BuffType buffType)
	{
		auto buffs = GetBuffList();

		for (const auto& buff : buffs)
		{
			if (buff->type() == buffType)
				return true;
		}

		return false;
	}

	bool buffFilter(BuffEntry* buff)
	{
		if (!buff || (DWORD)buff <= 0x1000)
			return false;

		if (static_cast<int>(buff->type()) == -1 || static_cast<int>(buff->type()) > 40)
			return false;

		if (static_cast<int>(GameTime::GetGameTime()) > buff->endTime())
			return false;

		if (strcmp(buff->name().c_str(), "invalidBuff") == 0 || buff->countAlt() == 0)
			return false;

		return true;
	}


}
}