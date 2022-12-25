#pragma once
#define LOG_MSG_IMPL(format, ...)Log::Msg(format, __VA_ARGS__)
#define LOG_MSG(format, ...) LOG_MSG_IMPL(format, __VA_ARGS__)
#define LOG_WARN_IMPL(format, ...)Log::Warning(format, __VA_ARGS__)
#define LOG_WARN(format, ...) LOG_WARN_IMPL(format, __VA_ARGS__)
#define LOG_DEBUG_IMPL(format, ...)Log::Debug(format, __VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG_DEBUG_IMPL(format, __VA_ARGS__)
#define LOG_ERROR_IMPL(format, ...)Log::Error(format, __VA_ARGS__)
#define LOG_ERROR(format, ...) LOG_ERROR_IMPL(format, __VA_ARGS__)
#define LOG_FATAL_IMPL(format, ...)Log::Fatal(format, __VA_ARGS__)
#define LOG_FATAL(format, ...) LOG_FATAL_IMPL(format, __VA_ARGS__)

class Log
{
public:
	static void Init();
	static void UnInit();
	static void Console();
	static void Msg(const char* fmt, ...);
	static void Error(const char* fmt, ...);
	static void Fatal(const char* fmt, ...);
	static void Debug(const char* fmt, ...);
	static void Warning(const char* fmt, ...);
private:
	static void InitLog();
	static void WriteLog(const char* Section, const char* Data, bool Error, bool Fatal);
};
