#pragma once
#include <Windows.h>
#include <vector>
#include <VersionHelpers.h>
#include <Zydis/Zydis.h>
#include <memory>
#include <list>

#include "HooksCommons.h"

BOOL sys_VirtualProtect(LPVOID lpAddress, SIZE_T* dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);

LONG WINAPI LeoHandler(EXCEPTION_POINTERS* pExceptionInfo);

namespace PGHook {
	inline std::list<HookEntries> hookEntries;

	bool Deinitialize();
	DWORD RestoreRtlAddVectoredExceptionHandler();
	DWORD RestoreZwQueryInformationProcess();
	DWORD RestoreZwProtectVirtualMemory();
	bool AddHook(DWORD address, DWORD hkAddress, size_t offset);

	inline bool isInit{};
	inline PVOID VEH_Handle{};

	DWORD VirtualAllocateRegion(PVOID& NewFunction, DWORD OrigAddress, size_t size);
	void CopyRegion(DWORD dest, DWORD source, size_t size);
	bool Hook(DWORD original_fun, DWORD hooked_fun, size_t offset);
	void FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size, size_t _offset);

	DWORD AddHook(DWORD address, DWORD hkAddress, size_t functionSize, size_t offset);
};