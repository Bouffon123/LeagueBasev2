#include "Decrypt.h"

LeagueDecryptData LeagueDecrypt::Decrypt(const wchar_t* szModule) {
	std::string szSectionName = ".text";
	LeagueDecryptData ldd;
	ldd.totalFailedDecrypted = 0;
	ldd.totalSuccessDecrypted = 0;
	ldd.totalSuccess_PAGE_NOACCESS = 0;
	ldd.totalSuccess_EXCEPTION_CONTINUE_EXECUTION = 0;
	DWORD dllImageBase = (DWORD)GetModuleHandle(szModule); //suppose hModule is the handle to the loaded Module (.exe or .dll)
	if (dllImageBase) {
		//get the address of NT Header
		IMAGE_NT_HEADERS* pNtHdr = ImageNtHeader((PVOID)dllImageBase);

		IMAGE_SECTION_HEADER* pSectionHdr = (IMAGE_SECTION_HEADER*)(pNtHdr + 1);

		ImageSectionInfo* pSectionInfo = NULL;

		//iterate through the list of all sections, and check the section name in the if conditon. etc
		for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++)
		{
			char* name = (char*)pSectionHdr->Name;
			if (memcmp(name, szSectionName.c_str(), szSectionName.length()) == 0)
			{
				pSectionInfo = new ImageSectionInfo(szSectionName.c_str());
				pSectionInfo->SectionAddress = dllImageBase + pSectionHdr->VirtualAddress;
				pSectionInfo->SectionSize = pSectionHdr->Misc.VirtualSize;
				auto mbi = MEMORY_BASIC_INFORMATION{ nullptr };
				uint8_t* nextCheckAddress = NULL;
				bool isFirstRegion = true;
				for (unsigned int i = 0; i < pSectionInfo->SectionSize; ++i)
				{
					uint8_t* currentAddress = (uint8_t*)pSectionInfo->SectionAddress + i;
					if (currentAddress >= nextCheckAddress)
					{
						if (!VirtualQuery(reinterpret_cast<void*>(currentAddress), &mbi, sizeof(mbi)))
							continue;

						if (mbi.Protect != PAGE_NOACCESS || isFirstRegion)
						{
							isFirstRegion = false;
							i += reinterpret_cast<std::uintptr_t>(mbi.BaseAddress) + mbi.RegionSize - (pSectionInfo->SectionAddress + i);
							i--;
							continue;
						}
						nextCheckAddress = static_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
					}
					const int ret = IsMemoryDecrypted(reinterpret_cast<PVOID>(reinterpret_cast<DWORD>(currentAddress)));
					if (ret != 0)
					{
						if (ret == 1)
							ldd.totalSuccess_PAGE_NOACCESS++;

						else if (ret == 2)
							ldd.totalSuccess_EXCEPTION_CONTINUE_EXECUTION++;

						ldd.totalSuccessDecrypted++;
					}
					else
					{
						ldd.totalFailedDecrypted++;
					}
				}
				break;
			}
			pSectionHdr++;
		}
	}
	return ldd;
}

typedef BOOLEAN(__stdcall* t_RtlDispatchException)(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ContextRecord);
t_RtlDispatchException fn_RtlDispatchException;

uint8_t* LeagueDecrypt::_RtlDispatchExceptionAddress = nullptr;

int LeagueDecrypt::IsMemoryDecrypted(PVOID Address)
{
	CONTEXT ctx;
	EXCEPTION_RECORD exr;
	MEMORY_BASIC_INFORMATION mbi;
	memset(&mbi, 0, sizeof(mbi));
	VirtualQuery(Address, &mbi, sizeof(mbi));
	if (mbi.Protect != PAGE_NOACCESS)
	{
		return 1;
	}
	RtlCaptureContext(&ctx);
	memset(&exr, 0, sizeof(EXCEPTION_RECORD));

#ifdef _WIN64
	ctx.Rip = reinterpret_cast<DWORD64>(Address);// (DWORD)FinishThread;
#else
	ctx.Eip = reinterpret_cast<DWORD>(Address);// (DWORD)FinishThread;
#endif // 

	ctx.ContextFlags = 0x1007F;
	ctx.SegCs = 0x23;
	ctx.SegDs = 0x2b;
	ctx.SegEs = 0x2b;
	ctx.SegFs = 0x53;
	ctx.SegGs = 0x2b;
	ctx.SegSs = 0x2b;
	exr.ExceptionAddress = Address;
	exr.NumberParameters = 2;
	exr.ExceptionCode = EXCEPTION_ACCESS_VIOLATION;
	exr.ExceptionInformation[1] = reinterpret_cast<DWORD>(Address);
	_EXCEPTION_POINTERS ei;
	ei.ContextRecord = &ctx;
	ei.ExceptionRecord = &exr;

	if (!_RtlDispatchExceptionAddress)
		return 0;

	DWORD RtlDispatchExceptionAddr = (DWORD)(_RtlDispatchExceptionAddress);

	if (RtlDispatchExceptionAddr) {
		fn_RtlDispatchException = (t_RtlDispatchException)(RtlDispatchExceptionAddr);
		if (fn_RtlDispatchException(&exr, &ctx)) {
			return 2;
		}
	}

	return 0;
}

inline uint8_t* find_signature(LPCSTR szModule, const char* szSignature) {
	auto module = GetModuleHandleA(szModule);
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t*)module + dosHeader->e_lfanew);
	auto textSection = IMAGE_FIRST_SECTION(ntHeaders);

	auto sizeOfImage = textSection->SizeOfRawData;
	auto patternBytes = pattern_to_byte(szSignature);
	auto scanBytes = reinterpret_cast<uint8_t*>(module) + textSection->VirtualAddress;

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	auto mbi = MEMORY_BASIC_INFORMATION{ 0 };
	uint8_t* next_check_address = 0;

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			auto current_address = scanBytes + i + j;
			if (current_address >= next_check_address) {
				if (!VirtualQuery(reinterpret_cast<void*>(current_address), &mbi, sizeof(mbi)))
					break;

				if (mbi.Protect == PAGE_NOACCESS) {
					i += ((std::uintptr_t(mbi.BaseAddress) + mbi.RegionSize) - (std::uintptr_t(scanBytes) + i));
					i--;
					found = false;
					break;
				}
				else {
					next_check_address = reinterpret_cast<uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
				}
			}

			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}
	return nullptr;
}

uint8_t* LeagueDecrypt::find_RtlDispatchExceptionAddress()
{
	auto address = find_signature("ntdll.dll", "E8 ? ? ? ? 0A C0");

	if (!address)
	{
		return nullptr;
	}
	address = address + *reinterpret_cast<uint32_t*>(address + 1) + 5;

	return address;
}