#include "Hook.h"

#include "Context.h"
#include "Logger.h"
#include "VEHHook.h"

#include <detours/detours.h>

#include "PGHook.h"

namespace Script {
namespace Native {

    void* HookManager::Install(void* target, void* detour) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        PDETOUR_TRAMPOLINE trampoline;
        DetourAttachEx(&target, detour, &trampoline, nullptr, nullptr);
        if (const auto error = DetourTransactionCommit() != NO_ERROR) {
            LOG_ERROR("hook install 0x%p failed - error %i", target, error);
        }
        if (trampoline) {
            m_hookList.push_back(HookEntry(trampoline, target, detour));
        }

        return trampoline;
    }

    void* HookManager::InstallHWBP(void* target, void* detour, uint8_t registerIndex, DWORD functionSize) {

        DWORD retAddr = HWBPHook::AddHook((DWORD)target, (DWORD)detour, registerIndex, functionSize);

        m_hwbpHookList.push_back(HookEntry((PVOID)retAddr, target, detour));

        return (PVOID)retAddr;
    }


    void* HookManager::InstallVEH(void* target, void* detour, size_t offset, DWORD functionSize) {

        DWORD retAddr = PGHook::AddHook((DWORD)target, (DWORD)detour, functionSize, offset);

        m_pgHookList.push_back(HookEntry((PVOID)retAddr, target, detour));

        return (PVOID)retAddr;
    }

    bool HookManager::UnloadHooks() const {
        for (auto hookEntry : m_hookList) {
            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourDetach(&hookEntry.m_original, hookEntry.m_detour);
            if (const auto error = DetourTransactionCommit() != NO_ERROR) {
                LOG_ERROR("hook unload 0x%p failed - error %i", hookEntry.m_original, error);
            }
        }

        //m_hwbp->RemoveHooks(); // TODO

        return true;
    }

}
}
