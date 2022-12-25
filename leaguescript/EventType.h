#pragma once

namespace Script {
namespace Native {

	enum EventType : int {
		OnWndProc = 1,
		OnGameUpdate,
		OnEndScene,
		OnPresent,
		OnReset,
		OnPostReset,
	};
}
}