#pragma once
#include <Windows.h>
#include "EventType.h"
#include "RawCommon.h"

#include <d3dx9.h>
#include <vector>

#define LINK_EVENT(NUM, SKELETON, ... ) EventHandler<##NUM##, ##SKELETON##, __VA_ARGS__##>::GetInstance()->Add( nullptr ); \
	 EventHandler<##NUM##, ##SKELETON##, __VA_ARGS__##>::GetInstance()->Remove( nullptr );
#define LINK_EVENT_T(NUM, SKELETON, ... ) template class SCRIPT_API EventHandler<##NUM##, ##SKELETON##, __VA_ARGS__##>;

namespace Script {
namespace Native {
	typedef void (evtWndProc)(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, bool* execute);
	typedef void (evtGameUpdate)();

	// r3dRenderer
	typedef void (evtEndScene)(IDirect3DDevice9* device);
    typedef void (evtPresent)();
	typedef void (evtReset)(D3DPRESENT_PARAMETERS* presentParams);
	typedef void (evtPostReset)(D3DPRESENT_PARAMETERS* presentParams);

    template < int uniqueEventNumber, typename skeleton, typename ... TArgs >
    class SCRIPT_API EventHandler
    {
    private:
        std::vector< void* > m_eventCallbacks;

    public:
        EventHandler() {
        }

        static __declspec(noinline) EventHandler* GetInstance() {
            static EventHandler* instance;
            if (instance == nullptr) {
                instance = new EventHandler();
            }

            return instance;
        }

        void __declspec(noinline) Add(void* callback) {
            if (callback != nullptr) {
                m_eventCallbacks.push_back(callback);
            }
        }

        void __declspec(noinline) Remove(void* listener) {
            if (listener == nullptr) {
                return;
            }

            auto eventPtr = find(m_eventCallbacks.begin(), m_eventCallbacks.end(), listener);
            if (eventPtr != m_eventCallbacks.end()) {
                m_eventCallbacks.erase(find(m_eventCallbacks.begin(), m_eventCallbacks.end(), listener));
            }
        }

        bool __cdecl Trigger(TArgs ... args) {
            for (auto i = m_eventCallbacks.size(); i-- > 0; ) {
                auto ptr = m_eventCallbacks[i];
                if (ptr != nullptr) {
                    static_cast<skeleton*>(ptr)(args...);
                }
            }

            return true;
        }
    };

    LINK_EVENT_T(EventType::OnWndProc, evtWndProc, HWND, UINT, WPARAM, LPARAM, bool*)
    LINK_EVENT_T(EventType::OnGameUpdate, evtGameUpdate,)

    // r3dRenderer
    LINK_EVENT_T(EventType::OnEndScene, evtEndScene, IDirect3DDevice9*) //
    LINK_EVENT_T(EventType::OnReset, evtReset, D3DPRESENT_PARAMETERS*) //
    LINK_EVENT_T(EventType::OnPostReset, evtPostReset, D3DPRESENT_PARAMETERS*) //
}
}