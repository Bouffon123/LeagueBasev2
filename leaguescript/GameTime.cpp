#include "GameTime.h"

namespace Script {
namespace Native {
	float GameTime::GetGameTime() {
		return *reinterpret_cast<float*>(MAKE_RVA(Script::Native::Addresses::Game::GameTime));
	}

	int GameTime::GetTickCount() {
		return static_cast<int>(GetGameTime() * 1000);
	}
}
}