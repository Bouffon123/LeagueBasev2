#pragma once

namespace Script {
namespace Native {
	class HookManager;

	class Core {
	private:
		HookManager* m_hookManager;

	public:

		Core();
		~Core();

		HookManager* GetHookManager() const;

		bool ApplyHooks() const;
		void LinkHooks() const;
		void LoadFeatures() const;
	};

	extern Core* m_core;

	bool Initialize();
}
}