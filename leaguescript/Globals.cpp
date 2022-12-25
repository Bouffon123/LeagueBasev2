#include "Globals.h"

#include "fp_call.h"

#include <string>

namespace Script {
namespace Native {
	char* Globals::GameVersion() {
		return reinterpret_cast<char*>(MAKE_RVA(Addresses::Game::GameVersion));
	}

	bool Globals::IsScriptVersionLatest()
	{
		std::string v1{ GameVersion() };
		std::string v2{ SCRIPT_VERSION };
		return v1 == v2;
	}
}
}