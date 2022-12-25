#include "PGHook.h"

#include "syscall.h"

BOOL sys_VirtualProtect(LPVOID lpAddress, SIZE_T* dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) {
	return NT_SUCCESS(syscall_t<NTSTATUS>("ZwProtectVirtualMemory")(GetCurrentProcess(), lpAddress, dwSize, flNewProtect, lpflOldProtect));
}

int filterException(int code, PEXCEPTION_POINTERS ex) {
	return EXCEPTION_EXECUTE_HANDLER;
}

bool inRange(unsigned low, unsigned high, unsigned x)
{
	return  ((x - low) <= (high - low));
}

LONG __stdcall LeoHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && pExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 8)
	{
		for (HookEntries hs : PGHook::hookEntries)
		{
			for (HookDetails hd : hs.hookDetails)
			{
				if (hd.addressToHook == pExceptionInfo->ContextRecord->Eip) {
					pExceptionInfo->ContextRecord->Eip = hd.hookAddress;
					return EXCEPTION_CONTINUE_EXECUTION;
				}
			}
			if (inRange(hs.addressToHookMbiStart - 0x1000, hs.addressToHookMbiEnd + 0x1000, pExceptionInfo->ContextRecord->Eip)) {
				int offset = pExceptionInfo->ContextRecord->Eip - hs.addressToHookMbiStart;
				pExceptionInfo->ContextRecord->Eip = static_cast<DWORD>(hs.allocatedAddressStart + offset);
				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

namespace PGHook
{
	DWORD AddHook(DWORD address, DWORD hkAddress, size_t functionSize, size_t offset)
	{
		PVOID NewFunction;
		DWORD hookReturn = VirtualAllocateRegion(NewFunction, address, functionSize);
		CopyRegion((DWORD) NewFunction, address, functionSize);
		FixRellocation(address, address + functionSize, (DWORD) NewFunction, functionSize, offset);

		AddHook(address, hkAddress, offset);

		return hookReturn;
	}


	bool Deinitialize()
	{
		DWORD old;
		if (VEH_Handle)
		{
			if (RemoveVectoredExceptionHandler(VEH_Handle))
			{
				for (HookEntries hs : hookEntries)
				{
					for (HookDetails hd : hs.hookDetails)
					{
						auto addr = (PVOID)hd.addressToHook;
						auto size = static_cast<SIZE_T>(static_cast<int>(1));
						if (sys_VirtualProtect(&addr, &size, hs.addressToHookOldProtect, &old))
						{
						}
					}
				}
				hookEntries.clear();
				return true;
			}
		}

		return false;
	}

	DWORD RestoreRtlAddVectoredExceptionHandler() {
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
	DWORD RestoreZwQueryInformationProcess() {
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
	DWORD RestoreZwProtectVirtualMemory() {
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

	DWORD VirtualAllocateRegion(PVOID& NewFunction, DWORD OrigAddress, size_t size) {
		NewFunction = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT,
			PAGE_EXECUTE_READWRITE);
		return (DWORD)NewFunction;
	}

	void CopyRegion(DWORD dest, DWORD source, size_t size) {
		(void)memcpy((void*)dest, (PVOID)source, size);
	}

	bool AddHook(DWORD address, DWORD hkAddress, size_t offset)
	{
		if (Hook(address, hkAddress, offset))
		{
			return true;
		}
		return false;
	}

	bool Hook(DWORD original_fun, DWORD hooked_fun, size_t offset)
	{
		auto mbi = MEMORY_BASIC_INFORMATION{ 0 };
		if (!VirtualQuery(reinterpret_cast<void*>(original_fun), &mbi, sizeof(mbi))) {
			return false;
		}

		HookEntries hs;
		HookDetails hd;

		hd.addressToHook = original_fun;
		hd.hookAddress = hooked_fun;
		hs.addressToHookOldProtect = mbi.Protect;

		std::list<HookEntries> _hookEntries;
		bool isFound = false;
		for (HookEntries hs : hookEntries) {
			if (hs.addressToHookMbiStart == (DWORD)mbi.BaseAddress) {
				bool isExisting = false;
				for (HookDetails hd : hs.hookDetails) {
					if (original_fun == hd.addressToHook) {
						isExisting = true;
					}
				}
				if (!isExisting) {
					isFound = true;
					hs.hookDetails.emplace_back(hd);
				}
			}

			_hookEntries.emplace_back(hs);
		}
		if (isFound) {
			hookEntries = _hookEntries;
			return true;
		}

		hs.addressToHookMbiStart = ((DWORD)mbi.BaseAddress);
		hs.addressToHookMbiEnd = ((DWORD)mbi.BaseAddress) + 0x1000;
		hs.addressToHookMbiSize = 0x1000;

		PVOID NewRegionPVOID = nullptr;
		DWORD NewRegion = VirtualAllocateRegion(NewRegionPVOID, ((DWORD)mbi.BaseAddress - 0x1000), 0x3000) + 0x1000;
		CopyRegion((DWORD)NewRegionPVOID, ((DWORD)mbi.BaseAddress - 0x1000), 0x3000);
		FixRellocation(((DWORD)mbi.BaseAddress - 0x1000), ((DWORD)mbi.BaseAddress - 0x1000) + 0x3000, (DWORD)NewRegionPVOID, 0x3000, offset);

		hs.allocatedAddressStart = NewRegion;
		hs.allocatedAddressEnd = NewRegion + 0x1000;
		hs.allocatedAddressSize = 0x1000;
		hs.addressToHookoffsetFromStart = original_fun - ((DWORD)mbi.BaseAddress);
		hs.hookDetails.emplace_back(hd);

		for (HookEntries he : hookEntries) {
			if ((he.addressToHookMbiStart == hs.addressToHookMbiStart) &&
				(he.addressToHookMbiEnd == hs.addressToHookMbiEnd))
			{
				return true;
			}
		}

		if (!isInit)
		{
			VEH_Handle = AddVectoredExceptionHandler(true, static_cast<PTOP_LEVEL_EXCEPTION_FILTER>(LeoHandler));
			isInit = true;
		}

		if (VEH_Handle)
		{
			auto addr = (PVOID)original_fun;
			auto size = static_cast<SIZE_T>(static_cast<int>(1));
			if (sys_VirtualProtect(&addr, &size, PAGE_READONLY, &hs.addressToHookOldProtect))
			{
				hookEntries.emplace_back(hs);
				return true;
			}
		}

		return false;
	}

	void FixRellocation(DWORD OldFnAddress, DWORD OldFnAddressEnd, DWORD NewFnAddress, size_t size, size_t _offset)
	{
		ZydisDecoder decoder;
		ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
		ZydisFormatter formatter;
		ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

		ZyanU32 runtime_address = NewFnAddress + _offset;
		ZyanUSize offset = _offset;
		const ZyanUSize length = size;
		ZydisDecodedInstruction instruction;
		int fixedAddressesCount = 0;
		try
		{
			while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction)))
			{
				char buffer[256];
				ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
					runtime_address);

				std::string mnemonic(buffer);
				if (mnemonic.find("call 0x") != std::string::npos) {
					DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

					DWORD originalCall = 0;
					{
						ZydisDecoder decoder1;
						ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);
						ZydisFormatter formatter1;
						ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);
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
					{
						DWORD calc1 = (runtime_address - originalCall + 4);
						DWORD calc = 0xFFFFFFFF - calc1;
						*(DWORD*)(runtime_address + 1) = calc;

						if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {
							char buffer[256];
							ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

							std::string mnemonic(buffer);
							{
								DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);
								if ((hex >= OldFnAddress + 0x1000) && (hex <= OldFnAddressEnd - 0x1000)) {
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
						}
					}
					fixedAddressesCount++;
				}
				else if (mnemonic.find("int3") != std::string::npos) {
					*(BYTE*)(NewFnAddress + offset) = 0x90;
					fixedAddressesCount++;
				}
				else if ((mnemonic.find("jmp 0x") != std::string::npos) && (*(BYTE*)(runtime_address) == 0xe9)) {
					DWORD hex = std::strtoul((mnemonic.substr(5, 10)).c_str(), NULL, 16);

					DWORD originalCall = 0;
					{
						ZydisDecoder decoder1;
						ZydisDecoderInit(&decoder1, ZYDIS_MACHINE_MODE_LONG_COMPAT_32, ZYDIS_ADDRESS_WIDTH_32);

						ZydisFormatter formatter1;
						ZydisFormatterInit(&formatter1, ZYDIS_FORMATTER_STYLE_INTEL);

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
					{
						DWORD calcx = originalCall - (OldFnAddress + offset);
						DWORD calcy = calcx + (OldFnAddress + offset);
						DWORD calc = calcy - runtime_address - 0x5;

						*(DWORD*)(runtime_address + 1) = calc;

						if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {
							char buffer[256];
							ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

							std::string mnemonic(buffer);
							{
								DWORD hex = std::strtoul((mnemonic.substr(4, 10)).c_str(), NULL, 16);
								if ((hex >= OldFnAddress + 0x1000) && (hex <= OldFnAddressEnd - 0x1000)) {
									DWORD calc = calcx - 0x5;
									*(DWORD*)(runtime_address + 1) = calc;

									if (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (PVOID)(NewFnAddress + offset), length - offset, &instruction))) {
										char buffer[256];
										ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer), runtime_address);

										std::string mnemonic(buffer);
									}
								}
							}
						}
					}
					fixedAddressesCount++;
				}

				offset += instruction.length;
				runtime_address += instruction.length;
			}
		}
		catch (...) {
		}
	}
}