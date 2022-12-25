#include "Core.h"

#include <thread>

#include "Context.h"
#include "CorePlugin.h"
#include "Hook.h"
#include "Logger.h"
#include "EventHandler.h"
#include "MenuPlugin.h"
#include "r3dRenderer.h"
#include "Globals.h"
#include "GameObject.h"
#include "GameTime.h"
#include "VEHHook.h"

#define VERIFY_HOOK( CLASS, NAME) if (!##CLASS##::ApplyHooks()) { LOG_MSG("[!] %s Hooks Failed~", #CLASS); return false; }

namespace Script {
namespace Native {
	Core* m_core;

	void InitThread();

	bool Initialize()
	{
		std::thread cheat(InitThread);

		if(cheat.joinable())
		{
			cheat.join();
			return true;
		}

		return false;
	}

	void InitThread()
	{
		m_core = new Core();

		if (Globals::IsScriptVersionLatest())
		{
			while (GameTime::GetGameTime() < 5.f && !GameObject::LocalPlayer())
				Sleep(1);

			if (!m_core->ApplyHooks()) {
				LOG_ERROR("[!] Failed to apply Core hooks");
				return;
			}

			// wait until device gets intialized from endscene hook
			while (!g_ctx->mdx9Device) { Sleep(100); }

			m_core->LoadFeatures();
		}
		else
		{
			MessageBoxA(nullptr, Globals::GameVersion(), "NOT SUPPORTED", 0);
		}
	}

	Core::Core() : m_hookManager(nullptr)
	{
		g_ctx->mModuleBase = reinterpret_cast<unsigned>(GetModuleHandleA(nullptr));
		g_ctx->mD3d9Base = reinterpret_cast<unsigned>(GetModuleHandleA("d3d9.dll"));

		m_hookManager = new HookManager();

		Log::Init();
	}

	Core::~Core() {
		m_hookManager->UnloadHooks();
		delete m_hookManager;
		Log::UnInit();
	}

	HookManager* Core::GetHookManager() const {
		return m_hookManager;
	}

	void Core::LinkHooks() const
	{
		LINK_EVENT(EventType::OnWndProc, evtWndProc, HWND, UINT, WPARAM, LPARAM, bool*)

		// r3dRenderer
		LINK_EVENT(EventType::OnEndScene, evtEndScene)
		LINK_EVENT(EventType::OnReset, evtReset, D3DPRESENT_PARAMETERS*)
		LINK_EVENT(EventType::OnPostReset, evtPostReset, D3DPRESENT_PARAMETERS*)
	}

	bool Core::ApplyHooks() const
	{
		if (!m_hookManager) {
			return false;
		}

		VEHHooks::Initialize();

		VERIFY_HOOK(r3dRenderer, "r3dRenderer");
		VERIFY_HOOK(GameObject, "GameObject");

		return true;
	}

	void Core::LoadFeatures() const
	{
		Plugins::MenuPlugin::Initialize();
		Plugins::CorePlugin::Initialize();
	}

}
}
