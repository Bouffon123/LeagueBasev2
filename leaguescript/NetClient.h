#pragma once

#include "Common.h"

namespace Script {
namespace Native {
	class SCRIPT_API NetClient final
	{
	public:
		MAKEGET(int, GetPing)

		static NetClient* Instance();
	};
}
}