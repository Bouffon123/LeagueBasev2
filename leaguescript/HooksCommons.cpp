#include "HooksCommons.h"

#include <string>
#include <Psapi.h>

namespace Process
{
#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))
#define ROR(x, y) ((unsigned)(x) >> (y) | (unsigned)(x) << 32 - (y))

	typedef
	enum _PROCESSINFOCLASS
	{
		ProcessBasicInformation,
		ProcessCookie = 36
	} PROCESSINFOCLASS;

	extern "C" NTSYSCALLAPI NTSTATUS NTAPI NtQueryInformationProcess(IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		OUT PULONG ReturnLength OPTIONAL);

	DWORD FindPattern(std::string moduleName, std::string Mask)
	{
		const char* pat = Mask.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
		MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;

		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2; //one ?
			}
			else
			{
				pat = Mask.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}

	static DWORD process_cookie_ = 0;

	DWORD GetProcessCookie()
	{
		DWORD dwProcessCookie = NULL;
		NTSTATUS NtStatus = NtQueryInformationProcess(GetCurrentProcess(), ProcessCookie, &dwProcessCookie, 4, nullptr);

		if (!NT_SUCCESS(NtStatus))
			return NULL;

		return dwProcessCookie;
	}

	DWORD DecodePointerHandle(DWORD pointer)
	{
		if (!process_cookie_)
		{
			process_cookie_ = GetProcessCookie();
			if (!process_cookie_)
			{
				return 0;
			}
		}

		unsigned char shift_size = 0x20 - (process_cookie_ & 0x1f);
		return ROR(pointer, shift_size) ^ process_cookie_;
	}

	DWORD GetVEHOffset()
	{
		HMODULE ntdll = GetModuleHandleA("ntdll.dll");

		std::string sig_str = "89 46 10 81 C3 ?? ?? ?? ??";

		uint8_t* match_list = (uint8_t*)FindPattern("ntdll.dll", sig_str);
		BYTE* veh_list = *reinterpret_cast<BYTE**>(match_list + 5);
		size_t veh_list_offset = veh_list - reinterpret_cast<BYTE*>(ntdll);

		return veh_list_offset;
	}

	_VECTORED_HANDLER_LIST* GetVECTORED_HANDLER_LIST(DWORD& veh_addr)
	{
		DWORD ntdll = (DWORD)GetModuleHandleA("ntdll.dll");
		_VECTORED_HANDLER_LIST* handler_list;
		DWORD VEHOffset = GetVEHOffset();
		veh_addr = ntdll + VEHOffset;
		handler_list = &*(_VECTORED_HANDLER_LIST*)veh_addr;

		return handler_list;
	}

	std::vector<PVECTORED_EXCEPTION_HANDLER> GetAllHandlers()
	{
		std::vector<PVECTORED_EXCEPTION_HANDLER> _PVECTORED_EXCEPTION_HANDLER_list;

		DWORD veh_addr;
		_VECTORED_HANDLER_LIST* handler_list = GetVECTORED_HANDLER_LIST(veh_addr);

		VECTORED_HANDLER_ENTRY* entry;
		entry = &*(VECTORED_HANDLER_ENTRY*)((DWORD)handler_list->first_exception_handler);

		while (true)
		{
			DWORD handler = reinterpret_cast<DWORD>(entry->handler);
			_PVECTORED_EXCEPTION_HANDLER_list.push_back((PVECTORED_EXCEPTION_HANDLER)(DecodePointerHandle(handler)));

			if (reinterpret_cast<DWORD>(entry->next) == veh_addr + sizeof(DWORD))
			{
				break;
			}
			entry = &*(VECTORED_HANDLER_ENTRY*)((DWORD)entry->next);
		}
		return _PVECTORED_EXCEPTION_HANDLER_list;
	}

	void RemoveAllHandlers()
	{
		HMODULE ntdll = GetModuleHandleA("ntdll.dll");

		DWORD remove_exception_handler = reinterpret_cast<DWORD>(GetProcAddress(ntdll, "RtlRemoveVectoredExceptionHandler")
			);


		typedef BOOL(__stdcall* t_RemoveVectoredExceptionHandler)(PVOID Handle);
		t_RemoveVectoredExceptionHandler fn_RemoveVectoredExceptionHandler = (t_RemoveVectoredExceptionHandler)remove_exception_handler;

		for (int i = 0; i < GetAllHandlers().size(); i++)
		{
			DWORD veh_addr;
			_VECTORED_HANDLER_LIST* handler_list = GetVECTORED_HANDLER_LIST(veh_addr);
			fn_RemoveVectoredExceptionHandler(static_cast<PVOID>(handler_list->first_exception_handler));
		}
	}

	void ReAddAllHandlers(std::vector<PVECTORED_EXCEPTION_HANDLER> handlerList)
	{
		for (PVECTORED_EXCEPTION_HANDLER pvh : handlerList)
		{
			AddVectoredExceptionHandler(0, pvh);
		}
	}
}
