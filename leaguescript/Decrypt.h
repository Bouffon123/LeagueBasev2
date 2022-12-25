#pragma once

#include <cstdint>
#include <Windows.h>
#include <string>
#include <vector>
#include <process.h>
#include <DbgHelp.h>

struct LeagueDecryptData
{
	int totalSuccessDecrypted;
	int totalSuccess_PAGE_NOACCESS;
	int totalSuccess_EXCEPTION_CONTINUE_EXECUTION;
	int totalFailedDecrypted;
};


struct ImageSectionInfo
{
	char SectionName[IMAGE_SIZEOF_SHORT_NAME];//the macro is defined WinNT.h
	DWORD SectionAddress;
	DWORD SectionSize;
	ImageSectionInfo(const char* name)
	{
		strcpy_s(SectionName, name);
	}
};


class LeagueDecrypt
{
public:
	static uint8_t* _RtlDispatchExceptionAddress;
	static int IsMemoryDecrypted(PVOID Address);
	static LeagueDecryptData Decrypt(const wchar_t* szModule);

	static uint8_t* find_RtlDispatchExceptionAddress();
};