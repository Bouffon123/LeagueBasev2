#pragma once

#include "Common.h"
#include "BuffEntry.h"

namespace Script {
namespace Native {



	class SCRIPT_API BuffManager final
	{
	public:
		MAKEGET(DWORD, buffEntriesArray) //
		MAKEGET(DWORD, buffEntriesArrayEnd) //

		std::vector<BuffEntry*> GetBuffList();
		BuffEntry*				GetBuffEntryByName(std::string buffName);
		bool					HasBuffType(BuffType buffType);
	};
}
}