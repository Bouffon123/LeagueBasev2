#include "syscall.h"
#include "Context.h"

const char* call = nullptr;
DWORD syscall;

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
LPVOID HGateCopy = nullptr;

DWORD Backup_Eax;

static bool IsHeavensGateInitialized = false;

const DWORD_PTR __declspec(naked) GetGateAddress()
{
	__asm
	{
		mov eax, dword ptr fs : [0xC0]
		ret
	}
}

DWORD Handle, * Address_1, New, Old, * DwSizee;
DWORD _ret1, _ret2;

void __declspec(naked) hk_NtProtectVirtualMemory()
{
	__asm {
		mov Backup_Eax, eax
		mov eax, [esp] // - 4 bytes ret1
		mov _ret1, eax
		mov eax, [esp + 0x4] // - 4 bytes ret2
		mov _ret2, eax
		mov eax, [esp + 0x8] //HANDLE - 4 bytes ProcessHandle
		mov Handle, eax
		mov eax, [esp + 0xC] //PVOID - 4 bytes *BaseAddress
		mov Address_1, eax
		mov eax, [esp + 0x10] //PULONG - 4 bytes NumberOfBytesToProtect
		mov DwSizee, eax
		mov eax, [esp + 0x14] //ULONG - 4 bytes NewAccessProtection
		mov New, eax
		mov eax, [esp + 0x18] //ULONG OldAccessProtection 
		mov Old, eax
		mov eax, Backup_Eax
		pushad
	}

	__asm popad
	__asm jmp HGateCopy
}

DWORD ProcessHandle, BaseAddress, BaseAddress2, AllocationBase, AllocationProtect, PartitionId,
RegionSize, State, Protect, typex, MemoryInformation, MemoryInformationLength, ReturnLength;

DWORD ret1, ret2;

void __declspec(naked) hk_NtQueryVirtualMemory()
{
	__asm {
		mov Backup_Eax, eax
		mov eax, [esp] // - 4 bytes ret1
		mov ret1, eax
		mov eax, [esp + 0x4] // - 4 bytes ret2
		mov ret2, eax
		mov eax, [esp + 0x8] //HANDLE - 4 bytes ProcessHandle
		mov ProcessHandle, eax
		mov eax, [esp + 0xC] //PVOID - 4 bytes BaseAddress
		mov BaseAddress, eax
		mov eax, Backup_Eax
		pushad
	}

	__asm popad
	__asm jmp HGateCopy
}

void __declspec(naked) hk_NtReadVirtualMemory()
{
	__asm pushad
	__asm popad
	__asm jmp HGateCopy
}

DWORD RETURN1, RETURN2;

void __declspec(naked) hk_NtGetContextThread()
{
	HANDLE ThreadHandle;
	PCONTEXT pContext;
	DWORD _backup_eax;

	__asm pushad
	__asm mov _backup_eax, eax

	__asm mov eax, [esp + 0x28]
		__asm mov ThreadHandle, eax

	__asm mov eax, [esp + 0x2C]
		__asm mov pContext, eax

	__asm mov eax, _backup_eax


	if (((DWORD)ThreadHandle == 0xfffffffe) || (pContext->ContextFlags == 0x1003F) || (pContext->ContextFlags == 0x2F)) {
		pContext->ContextFlags &= ~CONTEXT_DEBUG_REGISTERS;
	}

	__asm popad
	__asm jmp HGateCopy
}

NTSTATUS hk_NtSetContextThread(HANDLE ThreadHandle, PCONTEXT pContext)
{
	return static_cast<NTSTATUS>(0);
}

NTSTATUS hk_NtSuspendThread(HANDLE ThreadHandle, PULONG PreviousSuspendCount)
{
	return static_cast<NTSTATUS>(0);
}

NTSTATUS hk_NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus)
{
	return static_cast<NTSTATUS>(0);
}

void __declspec(naked) hk_NtRaiseException()
{
	__asm {
		pushad
	}
	__asm popad
	__asm jmp HGateCopy
}

void __declspec(naked) hk_Wow64Trampoline()
{
	//////////////////////////
	//  create a filter on desired module range to avoid crash
	//////////////////////////

	__asm {
		//cmp eax, 0x23 //64bit Syscall id of NtQVM
		//je hk_NtQueryVirtualMemory

		//cmp eax, 0x50 //64bit Syscall id of NtPVM
		//je hk_NtProtectVirtualMemory

		cmp eax, 0xF2 //64bit Syscall id of NtGCT
		je hk_NtGetContextThread

		//cmp eax, 0x18B //64bit Syscall id of NtSCT
		//je hk_NtSetContextThread

		//cmp eax, 0x1BC //64bit Syscall id of NtSuspendThread
		//je hk_NtSuspendThread

		//cmp eax, 0x2C //64bit Syscall id of NtTerminateProcess
		//je hk_NtTerminateProcess

		//cmp eax, 0x166 //64bit Syscall id of NtRaiseException
		//je hk_NtRaiseException
	}
	/*
	__asm pushad
	__asm mov syscall, eax
	__asm {
		mov Backup_Eax, eax
		mov eax, [esp] // - 4 bytes ret1
		mov RETURN1, eax
		mov eax, [esp + 0x4] // - 4 bytes ret2
		mov RETURN2, eax
		mov eax, Backup_Eax
	}
	__asm popad
	*/

	__asm jmp HGateCopy
}

const LPVOID CreateNewJump()
{
	DWORD_PTR Gate = GetGateAddress();
	HGateCopy = VirtualAlloc(nullptr, 0x1000, MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	memcpy(HGateCopy, (void*)Gate, 9);

	return HGateCopy;
}

const void WriteJump(const DWORD_PTR dwWow64Address, const void* pBuffer, size_t ulSize)
{
	DWORD dwOldProtect = 0;

	auto addr = (PVOID)dwWow64Address;
	auto size = static_cast<SIZE_T>(0x1000);

	if (NT_SUCCESS(
		makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
			GetCurrentProcess(), &addr, &size, PAGE_EXECUTE_READWRITE, &dwOldProtect)))
	{
	}
	else
	{
	}

	(void)memcpy((void*)dwWow64Address, pBuffer, ulSize);

	if (NT_SUCCESS(
		makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x14, 0x00)(
			GetCurrentProcess(), &addr, &size, dwOldProtect, &dwOldProtect)))
	{
	}
	else
	{
	}
}

const void EnableHeavensGateHook()
{
	DWORD TrampolineGate = (DWORD)CreateNewJump();

	static char trampolineBytes[] =
	{
		0x68, 0xDD, 0xCC, 0xBB, 0xAA, /*push 0xAABBCCDD*/
		0xC3, /*ret*/
		0xCC, 0xCC, 0xCC /*padding*/
	};

	if (IsHeavensGateInitialized) {
		WriteJump(GetGateAddress(), trampolineBytes, sizeof(trampolineBytes));
		return;
	}

	LPVOID Hook_Gate = &hk_Wow64Trampoline;

	memcpy(&trampolineBytes[1], &Hook_Gate, 4);
	WriteJump(GetGateAddress(), trampolineBytes, sizeof(trampolineBytes));
	IsHeavensGateInitialized = true;
}

const void DisableHeavensGateHook()
{
	if (!IsHeavensGateInitialized)
		return;

	WriteJump(GetGateAddress(), HGateCopy, 9);
}