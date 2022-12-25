#include "Logger.h"
#include <Windows.h>
#include <fstream>
#include <sstream>

#include "Context.h"

#define LOGFILE_NAME "internal.log"

#define CHARS_FOR_BUFF 4096
#define CHARS_FOR_PARAMS 3500
char g_LogFile[MAX_PATH];
bool IsConsole = false;
std::ofstream m_Console;

void Log::Init()
{
	std::ofstream file;
	file.open(LOGFILE_NAME, std::ios_base::app);
	file.close();
}

void Log::UnInit()
{
	FreeConsole();
	m_Console.close();
}

void Log::Console()
{
	if (!AttachConsole(GetCurrentProcessId())) AllocConsole();
	SetConsoleTitleW(L"Internal");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	m_Console.open("CONOUT$");
}

void Log::Msg(const char* fmt, ...)
{
	va_list va_alist;
	char chParameters[CHARS_FOR_PARAMS];
	va_start(va_alist, fmt);
	sprintf(chParameters, fmt, va_alist);
	va_end(va_alist);
	WriteLog("+", chParameters, false, false);
}

void Log::Error(const char* fmt, ...)
{
	va_list va_alist;
	char chParameters[CHARS_FOR_PARAMS];
	va_start(va_alist, fmt);
	sprintf(chParameters, fmt, va_alist);
	va_end(va_alist);
	WriteLog("-", chParameters, true, false);
}

void Log::Fatal(const char* fmt, ...)
{
	va_list va_alist;
	char chParameters[CHARS_FOR_PARAMS];
	va_start(va_alist, fmt);
	sprintf(chParameters, fmt, va_alist);
	va_end(va_alist);
	WriteLog("#", chParameters, false, true);
	ExitProcess(0);
}

void Log::Debug(const char* fmt, ...)
{
	va_list va_alist;
	char chParameters[CHARS_FOR_PARAMS];
	va_start(va_alist, fmt);
	sprintf(chParameters, fmt, va_alist);
	va_end(va_alist);
	WriteLog("@", chParameters, false, false);
}

void Log::Warning(const char* fmt, ...)
{
	va_list va_alist;
	char chParameters[CHARS_FOR_PARAMS];
	va_start(va_alist, fmt);
	sprintf(chParameters, fmt, va_alist);
	va_end(va_alist);
	WriteLog("!", chParameters, false, false);
}

void Log::InitLog()
{
	char chLogBuff[CHARS_FOR_BUFF];
	char szTimestamp[30];
	tm current_tm;
	time_t current_time = time(NULL);
	localtime_s(&current_tm, &current_time);
	strftime(szTimestamp, sizeof(szTimestamp), "[%Y-%m-%d-%H:%M:%S] %s", &current_tm);
	sprintf_s(chLogBuff, szTimestamp);
	FILE* file;
	if ((fopen_s(&file, g_LogFile, "w")) == 0)
	{
		fprintf_s(file, "%s\n", chLogBuff);
		fclose(file);
	}
	if (IsConsole) Console();
	m_Console << chLogBuff << std::endl;
}

void Log::WriteLog(const char* Section, const char* Data, bool Error, bool Fatal)
{
	char chLogBuff[CHARS_FOR_BUFF];
	char szTimestamp[30];
	struct tm current_tm;
	time_t current_time = time(NULL);
	localtime_s(&current_tm, &current_time);
	strftime(szTimestamp, sizeof(szTimestamp), "%Y-%m-%d-%H:%M:%S", &current_tm);
	sprintf_s(chLogBuff, "[%s][%s] %s", szTimestamp, Section, Data);
	std::ofstream file;
	file.open(LOGFILE_NAME, std::ios_base::app);

	file << chLogBuff << "\n";

	if (Error) MessageBoxA(NULL, chLogBuff, "ERROR", MB_ICONERROR);
	if (Fatal) MessageBoxA(NULL, chLogBuff, "FATAL ERROR", MB_ICONERROR);
	if (IsConsole) m_Console << chLogBuff << std::endl;
}
