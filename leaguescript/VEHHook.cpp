#include "VEHHook.h"

#include <cinttypes>
#include <list>

#include "syscall.h"

#include <TlHelp32.h>
#include <Zydis/Zydis.h>

#include "Imgui/Console.h"
#include "Context.h"
#include "HooksCommons.h"
#include "Decrypt.h"
#include "HeavensGate.h"
#include "hexify.h"

std::vector<HookStructHWBP> VEHHooks::hookListHWBP{ };
PVOID						VEHHooks::VEH_Handle  { nullptr };
PVOID						VEHHooks::VCH_Handle  { nullptr };
bool						VEHHooks::IsDoneInit  { false };

DWORD VEHHooks::DR0{ 0 };
DWORD VEHHooks::DR1{ 0 };
DWORD VEHHooks::DR2{ 0 };
DWORD VEHHooks::DR3{ 0 };
DWORD VEHHooks::DR6{ 0 };
DWORD VEHHooks::DR7{ 0 };

DWORD VEHHooks::Initialize()
{
	EnableHeavensGateHook();

	std::vector<PVECTORED_EXCEPTION_HANDLER> PVECTORED_EXCEPTION_HANDLER_list = Process::GetAllHandlers();
	Process::RemoveAllHandlers();

	if (!IsDoneInit)
	{
		VEH_Handle = AddVectoredExceptionHandler(1, (PTOP_LEVEL_EXCEPTION_FILTER)(VEHHooks::LeoHandler));
		VCH_Handle = AddVectoredContinueHandler(1, (PVECTORED_EXCEPTION_HANDLER)VEHHooks::VCHandler);
		IsDoneInit = true;
	}

	Process::ReAddAllHandlers(PVECTORED_EXCEPTION_HANDLER_list);

	RestoreZwProtectVirtualMemory();
	RestoreRtlAddVectoredExceptionHandler();
	RestoreZwQueryInformationProcess();

	if (GetSystemDEPPolicy() != DEP_SYSTEM_POLICY_TYPE::DEPPolicyAlwaysOff)
		SetProcessDEPPolicy(PROCESS_DEP_ENABLE);

	//do
	//{
	//	LeagueDecrypt::_RtlDispatchExceptionAddress = LeagueDecrypt::find_RtlDispatchExceptionAddress();
	//} while (!LeagueDecrypt::_RtlDispatchExceptionAddress);

	//LeagueDecrypt::Decrypt(nullptr);

	return 0;
}

LONG WINAPI VEHHooks::VCHandler(EXCEPTION_POINTERS* pExceptionInfo) {
	pExceptionInfo->ContextRecord->Dr0 = DR0;
	pExceptionInfo->ContextRecord->Dr1 = DR1;
	pExceptionInfo->ContextRecord->Dr2 = DR2;
	pExceptionInfo->ContextRecord->Dr3 = DR3;
	pExceptionInfo->ContextRecord->Dr7 = DR7;
	return EXCEPTION_CONTINUE_EXECUTION;
}

LONG WINAPI VEHHooks::LeoHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
	pExceptionInfo->ContextRecord->Dr0 = 0;
	pExceptionInfo->ContextRecord->Dr1 = 0;
	pExceptionInfo->ContextRecord->Dr2 = 0;
	pExceptionInfo->ContextRecord->Dr3 = 0;
	pExceptionInfo->ContextRecord->Dr6 = 0;
	pExceptionInfo->ContextRecord->Dr7 = 0;

	if (pExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
	{
		for (HookStructHWBP hs : VEHHooks::hookListHWBP)
		{
			if (pExceptionInfo->ContextRecord->Eip == static_cast<DWORD>(hs.og_fun))
			{
				pExceptionInfo->ContextRecord->Dr6 = 0;
				pExceptionInfo->ContextRecord->Eip = static_cast<DWORD>(hs.hk_fun);
			}
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

bool HWBPHook::Hook(DWORD original_fun, DWORD hooked_fun, uint8_t RegIndex)
{
	std::vector<HookStructHWBP> hookListCopy = hookListHWBP;
	for (HookStructHWBP hs : hookListCopy)
	{
		if (hs.RegIndex == RegIndex)
		{
			return false;
		}
	}
	HookStructHWBP hs;
	hs.og_fun = original_fun;
	hs.hk_fun = hooked_fun;
	hs.RegIndex = RegIndex;

	//Register the Custom Exception Handler
	if (!IsDoneInit)
	{
		VEH_Handle = AddVectoredExceptionHandler(true, static_cast<PTOP_LEVEL_EXCEPTION_FILTER>(VEHHooks::LeoHandler));
		IsDoneInit = true;
	}

	//Toggle PAGE_GUARD flag on the page
	if (VEH_Handle)
	{
		hookListHWBP.push_back(hs);
		HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
		THREADENTRY32 te32;
		hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		if (hThreadSnap)
		{
			te32.dwSize = sizeof(THREADENTRY32);

			if (!Thread32First(hThreadSnap, &te32))
			{
				CloseHandle(hThreadSnap);
			}

			do
			{
				if (te32.th32OwnerProcessID == GetCurrentProcessId() && te32.th32ThreadID != GetCurrentThreadId())
					//Ignore threads from other processes AND the own thread of course
				{
					std::string threadId = hexify<DWORD>(static_cast<DWORD>(te32.th32ThreadID));

					HANDLE ThreadHandle = nullptr;
					OBJECT_ATTRIBUTES ObjectAttributes;
					CLIENT_ID ClientId;
					InitializeObjectAttributes(&ObjectAttributes, NULL, NULL, NULL, NULL);

					ClientId.UniqueProcess = (PVOID)te32.th32OwnerProcessID;
					ClientId.UniqueThread = (PVOID)te32.th32ThreadID;

					NTSTATUS res = makesyscall<NTSTATUS>(0x2E, 0x01, 0x00, 0x00, "RtlInterlockedCompareExchange64",
						0x170, 0xC2, 0x10, 0x00)(
							&ThreadHandle, THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME,
							&ObjectAttributes, &ClientId);
					if (NT_SUCCESS(res))
					{
						if (ThreadHandle)
						{
							CONTEXT Ctx;
							Ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

							if (NT_SUCCESS(
								makesyscall<NTSTATUS>(0xF2, 0x00, 0x00, 0x00, "RtlInterlockedCompareExchange64", 0x170,
									0xC2, 0x08, 0x00)(ThreadHandle, &Ctx)))
							{
								if (te32.th32ThreadID != GetCurrentThreadId())
								{
									NTSTATUS res = makesyscall<NTSTATUS>(
										0xBC, 0x01, 0x07, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08,
										0x00)(ThreadHandle, NULL);
								}
								else
								{
								}
								bool FoundReg = true;
								if (FoundReg)
								{
									bool isInvalidReg = false;

									switch (RegIndex)
									{
									case 0:
										Ctx.Dr0 = static_cast<DWORD_PTR>(original_fun);
										DR0 = (DWORD_PTR)original_fun;
										break;
									case 1:
										Ctx.Dr1 = static_cast<DWORD_PTR>(original_fun);
										DR1 = (DWORD_PTR)original_fun;
										break;
									case 2:
										Ctx.Dr2 = static_cast<DWORD_PTR>(original_fun);
										DR2 = (DWORD_PTR)original_fun;
										break;
									case 3:
										Ctx.Dr3 = static_cast<DWORD_PTR>(original_fun);
										DR3 = (DWORD_PTR)original_fun;
										break;
									default:
										isInvalidReg = true;
										break;
									}

									if (!isInvalidReg)
									{
										//Turn a local register on
										//Ctx.Dr7 |= ((1 << (2 * RegIndex)) | (3 << (((2 * (RegIndex)) * 2) + 16)) | (3 << (((2 * (RegIndex)) * 2) + 18)));
										Ctx.Dr7 &= ~(3ULL << (16 + 4 * RegIndex));
										//00b at 16-17, 20-21, 24-25, 28-29 is execute bp
										Ctx.Dr7 &= ~(3ULL << (18 + 4 * RegIndex));
										// size of 1 (val 0), at 18-19, 22-23, 26-27, 30-31
										Ctx.Dr7 |= 1ULL << (2 * RegIndex);
										DR7 = Ctx.Dr7;

										if (NT_SUCCESS(
											makesyscall<NTSTATUS>(0x8B, 0x01, 0x00, 0x00,
												"RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08, 0x00)(ThreadHandle
													, &Ctx)))
										{
										}
										else
										{
										}
									}
									else
									{
									}
								}
								else
								{
								}
								if (te32.th32ThreadID != GetCurrentThreadId())
								{
									NTSTATUS res = makesyscall<NTSTATUS>(
										0x6E, 0x00, 0x07, 0x00, "RtlInterlockedCompareExchange64", 0x170, 0xC2, 0x08,
										0x00)(ThreadHandle, NULL);
								}
								else
								{
								}
							}
							else
							{
							}
							CloseHandle(ThreadHandle);
						}
						else
						{
						}
					}
					else
					{
					}
				}
			} while (Thread32Next(hThreadSnap, &te32));
			CloseHandle(hThreadSnap);

			return true;
		}
	}
	else
	{
	}

	return false;
}

DWORD HWBPHook::AddHook(DWORD address, DWORD hkAddress, uint8_t RegIndex, size_t functionSize)
{
	PVOID NewFunction;
	DWORD hookReturn = AllocateRegion(NewFunction, address, functionSize);
	CopyRegion((DWORD)(NewFunction), address, functionSize);
	FixFuncRellocation(address, address + functionSize, (DWORD)NewFunction, functionSize);
	
	Hook(address, hkAddress, RegIndex);

	return hookReturn;
}

DWORD VEHHooks::AllocateRegion(PVOID& NewFunction, DWORD OrigAddress, size_t size)
{
	NewFunction = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE);
	return (DWORD)NewFunction;
}

void VEHHooks::CopyRegion(DWORD dest, DWORD source, size_t size) {
	(void)memcpy((void*)dest, (PVOID)source, size);
}

#define ZYAN_SUCCESS(status) \
    (!((status) & 0x80000000u))

void VEHHooks::FixFuncRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size, size_t _offset)
{
	// Initialize decoder context
	ZydisDecoder decoder;
	ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

	// Initialize formatter. Only required when you actually plan to do instruction
	// formatting ("disassembling"), like we do here
	ZydisFormatter formatter;
	ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

	// Loop over the instructions in our buffer.
	// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
	// visualize relative addressing
	ZyanU32 runtime_address = NewFnAddress + _offset;
	ZyanUSize offset = _offset;
	const ZyanUSize length = size;
	ZydisDecodedInstruction instruction;
	int fixedAddressesCount = 0;

	while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction)))
	{
		// Format & print the binary instruction structure to human readable format
		char buffer[256];
		ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
			runtime_address);

		std::string mnemonic(buffer);

		if (mnemonic.find("call 0x") != std::string::npos) {

			DWORD originalCall = 0;
			/////////////////////////////////////////////
			// GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			{
				// Initialize decoder context
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

				// Initialize formatter. Only required when you actually plan to do instruction
				// formatting ("disassembling"), like we do here
				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

				// Loop over the instructions in our buffer.
				// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
				// visualize relative addressing
				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);

						if (mnemonic1.find("call 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(5, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			/////////////////////////////////////////////
			// END GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			/////////////////////////////////////////////
			// APPLY RELOCATION FIXES
			/////////////////////////////////////////////
			{
				DWORD calc1 = (runtime_address - originalCall + 4);
				DWORD calc = 0xFFFFFFFF - calc1;
				*(DWORD*)(runtime_address + 1) = calc;

				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					//////////////////////////////////////////////////////////////////////////////////////////
					// CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
					{
						DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress) && (hex <= OldFnAddressEnd)) {
							DWORD calc1 = (runtime_address - hex + 4);
							DWORD calc = 0xFFFFFFFF - calc1;
							*(DWORD*)(runtime_address + 1) = calc;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
							}

						}
					}
					//////////////////////////////////////////////////////////////////////////////////////////
					// END CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
				}
			}
			/////////////////////////////////////////////
			// END APPLY RELOCATION FIXES
			/////////////////////////////////////////////

			fixedAddressesCount++;
		}
		else if (mnemonic.find("int3") != std::string::npos) {
			*(BYTE*)(NewFnAddress + offset) = 0x90;
			fixedAddressesCount++;
		}
		else if ((mnemonic.find("jmp 0x") != std::string::npos) && (*(BYTE*)(runtime_address) == 0xe9)) {

			DWORD originalCall = 0;
			/////////////////////////////////////////////
			// GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			{
				// Initialize decoder context
				ZydisDecoder decoder1;
				ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

				// Initialize formatter. Only required when you actually plan to do instruction
				// formatting ("disassembling"), like we do here
				ZydisFormatter formatter1;
				ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

				// Loop over the instructions in our buffer.
				// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
				// visualize relative addressing
				const ZyanUSize length = size;
				ZydisDecodedInstruction instruction1;
				{
					if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder1, (PVOID)(OldFnAddress + offset), length - offset, &instruction1))) {
						char buffer1[256];
						ZydisFormatterFormatInstruction(&formatter1, &instruction1, buffer1, sizeof(buffer1), OldFnAddress + offset);

						std::string mnemonic1(buffer1);
						if (mnemonic1.find("jmp 0x") != std::string::npos) {
							originalCall = std::strtoul((mnemonic1.substr(4, 10)).c_str(), NULL, 16);
						}
					}
				}
			}
			/////////////////////////////////////////////
			// END GET ORIGINAL OFFSET FROM ORIGINAL FUNCTION
			/////////////////////////////////////////////
			/////////////////////////////////////////////
			// APPLY RELOCATION FIXES
			/////////////////////////////////////////////
			{
				DWORD calcx = originalCall - (OldFnAddress + offset);
				DWORD calcy = calcx + (OldFnAddress + offset);
				DWORD calc = calcy - runtime_address - 0x5;

				*(DWORD*)(runtime_address + 1) = calc;

				if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

					char buffer[256];
					ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

					std::string mnemonic(buffer);
					//////////////////////////////////////////////////////////////////////////////////////////
					// CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
					{
						DWORD hex = std::strtoul((mnemonic.substr(4, 10)).c_str(), NULL, 16);
						if ((hex >= OldFnAddress) && (hex <= OldFnAddressEnd)) {
							DWORD calc = calcx - 0x5;
							*(DWORD*)(runtime_address + 1) = calc;

							if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {

								char buffer[256];
								ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

								std::string mnemonic(buffer);
							}

						}
					}
					//////////////////////////////////////////////////////////////////////////////////////////
					// END CHECK IF THE RELOCATED FIXES HAVE INSTRUCTIONS THAT CALL ORIGINAL. THEN CORRECT IT
					//////////////////////////////////////////////////////////////////////////////////////////
				}
			}
			/////////////////////////////////////////////
			// END APPLY RELOCATION FIXES
			/////////////////////////////////////////////

			fixedAddressesCount++;

		}

		offset += instruction.length;
		runtime_address += instruction.length;
	}
}

DWORD VEHHooks::RestoreRtlAddVectoredExceptionHandler() {
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	DWORD RtlAddVectoredExceptionHandlerAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "RtlAddVectoredExceptionHandler"));

	BYTE RtlAVE[] = {
		0x8B, 0xFF, 0x55, 0x8B, 0xEC
	};

	DWORD oldProt;

	auto addr = (PVOID)RtlAddVectoredExceptionHandlerAddr;
	auto size = static_cast<SIZE_T>(5);
	if (sys_VirtualProtect(&addr, &size, PAGE_EXECUTE_READWRITE, &oldProt))
	{
		int i = 0;
		for (BYTE _byte : RtlAVE) {
			*(BYTE*)(RtlAddVectoredExceptionHandlerAddr + i) = _byte;
			i++;
		}

		sys_VirtualProtect(&addr, &size, oldProt, &oldProt);
	}

	return RtlAddVectoredExceptionHandlerAddr;
}
DWORD VEHHooks::RestoreZwQueryInformationProcess() {
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	DWORD ZwQueryInformationProcessAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "ZwQueryInformationProcess"));

	BYTE ZwQIP[] = {
		0xB8, 0x19, 0x00, 0x00, 0x00
	};

	int i = 0;
	for (BYTE _byte : ZwQIP) {
		*(BYTE*)(ZwQueryInformationProcessAddr + i) = _byte;
		i++;
	}

	return ZwQueryInformationProcessAddr;
}
DWORD VEHHooks::RestoreZwProtectVirtualMemory() {
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	DWORD ZwQueryInformationProcessAddr = reinterpret_cast<DWORD>(
		GetProcAddress(ntdll, "ZwProtectVirtualMemory"));

	BYTE ZwQIP[] = {
		0xB8, 0x50, 0x00, 0x00, 0x00
	};

	int i = 0;
	for (BYTE _byte : ZwQIP) {
		*(BYTE*)(ZwQueryInformationProcessAddr + i) = _byte;
		i++;
	}

	return ZwQueryInformationProcessAddr;
}

BOOL VEHHooks::sys_VirtualProtect(LPVOID lpAddress, SIZE_T* dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) {
	return NT_SUCCESS(syscall_t<NTSTATUS>("ZwProtectVirtualMemory")(GetCurrentProcess(), lpAddress, dwSize, flNewProtect, lpflOldProtect));
}