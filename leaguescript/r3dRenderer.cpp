#include "r3dRenderer.h"

#include <d3d9types.h>

#include "Context.h"
#include "DirectX.h"
#include "Hook.h"
#include "EventHandler.h"
#include "fp_call.h"
#include "Logger.h"
#include "Imgui/impl/imgui_impl_dx9.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Script {
namespace Native {
#define FOCUS_CLASS r3dRenderer
	MAKEGET_C(D3DMATRIX, viewMatrix)
	MAKEGET_C(D3DMATRIX, projectionMatrix)
	MAKEGET_C(Vec2, rendererSize)
#undef FOCUS_CLASS

	Hook< convention_type::stdcall_t, long, IDirect3DDevice9* > HookEndScene;
	Hook< convention_type::stdcall_t, long, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* > HookReset;
	Hook< convention_type::stdcall_t, long, LPDIRECT3DDEVICE9, const RECT*, const RECT*, HWND, const RGNDATA* > HookPresent;

	WNDPROC m_wndProc;

	class Wrapper
	{
	public:
		static long __stdcall PresentCallee(
			LPDIRECT3DDEVICE9 pDevice, 
			const RECT* pSourceRect, 
			const RECT* pDestRect,
			HWND hDestWindowOverride, 
			const RGNDATA* pDirtyRegion
		) {
			if (g_ctx->mInitializedUI) {
				g_dx->StartFrame();

				EventHandler< EventType::OnPresent, evtPresent >::GetInstance()->Trigger();

				g_dx->EndFrame();
			}

			return HookPresent.CallOriginal(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		}

		static long __stdcall EndSceneCallee(IDirect3DDevice9* device) {

			if (!g_ctx->mInitializedUI)
			{
				g_ctx->mdx9Device = device;
				g_dx->Init();
				m_wndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_ctx->GameHWND, GWL_WNDPROC, reinterpret_cast<LONG>(Wrapper::WndProcCallee)));
			}

			return HookEndScene.CallOriginal(device);
		}

		static long __stdcall ResetCallee(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters) {
			ImGui_ImplDX9_InvalidateDeviceObjects();

			EventHandler< EventType::OnReset, evtReset, D3DPRESENT_PARAMETERS* >::GetInstance()->Trigger(pPresentationParameters);

			auto res = HookReset.CallOriginal(device, pPresentationParameters);

			ImGui_ImplDX9_CreateDeviceObjects();

			EventHandler< EventType::OnPostReset, evtPostReset, D3DPRESENT_PARAMETERS* >::GetInstance()->Trigger(pPresentationParameters);

			return res;
		}

		static LRESULT __stdcall WndProcCallee(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
				return 1;
			}

			bool execute = true;
			EventHandler< EventType::OnWndProc, evtWndProc, HWND, UINT, WPARAM, LPARAM, bool* >::GetInstance()->Trigger(hWnd, uMsg, wParam, lParam, &execute);

			if (execute) {
				return CallWindowProc(m_wndProc, hWnd, uMsg, wParam, lParam);
			}
			return 1;
		}
	};

	r3dRenderer* r3dRenderer::Instance() {
		return *reinterpret_cast<r3dRenderer**>(MAKE_RVA(Addresses::r3dRenderer::r3dRendererInstance));
	}

	Vec2 r3dRenderer::WorldToScreen(Vec3 in) {
		Vec3 screen{};

		base::c_call<bool>(MAKE_RVA(Addresses::r3dRenderer::WorldToScreen), &in, &screen);

		return Vec2{ screen.x, screen.y };
	}



	bool getD3D9Device(void** pTable, size_t Size)
	{
		// no ptable.
		if (!pTable)
			return false;

		IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

		// Direct3DCreate9 failed.
		if (!pD3D)
			return false;

		D3DPRESENT_PARAMETERS d3dpp = { 0 };
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = GetForegroundWindow();
		d3dpp.Windowed = ((GetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;;


		IDirect3DDevice9* pDummyDevice = nullptr;
		HRESULT create_device_ret = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);
		if (!pDummyDevice || FAILED(create_device_ret))
		{
			// CreateDevice failed
			pD3D->Release();
			return false;
		}

		memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);

		pDummyDevice->Release();
		pD3D->Release();

		return true;
	}

	bool r3dRenderer::ApplyHooks()
	{
		void* d3d9Device[119];
		while (!getD3D9Device(d3d9Device, sizeof d3d9Device)) {}

		HookEndScene.Apply(d3d9Device[42], Wrapper::EndSceneCallee);
		HookReset.Apply(d3d9Device[16], Wrapper::ResetCallee);
		HookPresent.Apply(d3d9Device[17], Wrapper::PresentCallee);

		return HookEndScene.IsApplied()
			&& HookReset.IsApplied()
			&& HookPresent.IsApplied();
	}

}
}
