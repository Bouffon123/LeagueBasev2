#pragma once

#include <Windows.h>
#include <vector>

struct HookStructPG
{
	DWORD og_fun;
	DWORD hk_fun;
	DWORD oldProtection;
};

struct HookStructHWBP
{
	DWORD og_fun;
	DWORD hk_fun;
	uint8_t RegIndex;
};

class VEHHooks
{
public:
	static DWORD DR0;
	static DWORD DR1;
	static DWORD DR2;
	static DWORD DR3;
	static DWORD DR6;
	static DWORD DR7;

	static bool IsDoneInit;

	static PVOID VEH_Handle;
	static PVOID VCH_Handle;

	static std::vector<HookStructHWBP> hookListHWBP;

	static DWORD Initialize();

	static bool AreInSamePage(const DWORD* Addr1, const DWORD* Addr2);

	static LONG WINAPI LeoHandler(EXCEPTION_POINTERS* pExceptionInfo);
	static LONG WINAPI VCHandler(EXCEPTION_POINTERS* pExceptionInfo);

	static BOOL sys_VirtualProtect(LPVOID lpAddress, SIZE_T* dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);

	static DWORD RestoreRtlAddVectoredExceptionHandler();
	static DWORD RestoreZwQueryInformationProcess();
	static DWORD RestoreZwProtectVirtualMemory();

	static DWORD AllocateRegion(PVOID& NewFunction, DWORD OrigAddress, size_t size);
	static void CopyRegion(DWORD dest, DWORD source, size_t size);
	static void FixFuncRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size, size_t offset = 0);
};

class HWBPHook : VEHHooks
{
private:
	static bool Hook(DWORD original_fun, DWORD hooked_fun, uint8_t RegIndex);
public:
	static DWORD AddHook(DWORD address, DWORD hkAddress, uint8_t RegIndex, size_t functionSize);
};