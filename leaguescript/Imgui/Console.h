#pragma once

#include "imgui.h"

#include <string>
#include <vector>

#define __DEV

#ifdef __DEV
#define __dbg(OUT_MSG, ...) g_ctx->mConsole->AddLog(0, OUT_MSG, __VA_ARGS__)
#else
#define __dbg(OUT_MSG, ...) 
#endif
                                                           
#define __err(OUT_MSG, ...)   g_ctx->mConsole->AddLog(3,  OUT_MSG, __VA_ARGS__)
#define __warn(OUT_MSG, ...)  g_ctx->mConsole->AddLog(2,  OUT_MSG, __VA_ARGS__)
#define __info(OUT_MSG, ...)  g_ctx->mConsole->AddLog(1,  OUT_MSG, __VA_ARGS__)
#define __fatal(OUT_MSG, ...) g_ctx->mConsole->AddLog(4,  OUT_MSG, __VA_ARGS__)
#define __print(OUT_MSG, ...) g_ctx->mConsole->AddLog(-1, OUT_MSG, __VA_ARGS__)


struct ConsoleItem {
    std::string message;
    ImColor color;

    ConsoleItem(std::string msg, ImColor color) : message(msg), color(color) {}
};

enum class eLoggerType : int
{
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

class GameConsole
{
private:
    char                      InputBuf[256];
    ImVector<const char*>     Commands;
    ImVector<ConsoleItem>     History;
    ImGuiTextFilter           Filter;
    bool                      AutoScroll;
    bool                      ScrollToBottom;

public:
    GameConsole();

    void ClearLog();
    void AddLog(int, const char* fmt, ...) IM_FMTARGS(2);
    void AddLogFromScript(std::string message);
    void Draw(const char* title);
};