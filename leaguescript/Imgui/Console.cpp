#include "Console.h"

#include <ctype.h>
#include <malloc.h>
#include <stdio.h>

static std::vector<ConsoleItem> Items;

GameConsole::GameConsole()
{
    AutoScroll = true;
    ScrollToBottom = true;
    //AddLog(eLoggerType::Info, "Welcome to Zekware.club");
    //AddLog(eLoggerType::Info, "Press the Insert key to open/close the menu.");
}

// Portable helpers
static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

void    GameConsole::ClearLog()
{
    Items.clear();
}

#define vsnprintf   _vsnprintf

void   GameConsole::AddLog(int logType, const char* fmt, ...) IM_FMTARGS(2)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);

    if (Items.size() > 500)
    {
        Items.erase(Items.begin());
    }

    ImColor color;
    std::string message;
    switch (logType)
    {
    case (int)eLoggerType::Debug:
        color = ImColor(172, 126, 247); // purple
        message = ICON_FA_ANGLE_RIGHT "[DEBUG] " + std::string(buf);
        break;
    case (int)eLoggerType::Warn:
        color = ImColor(255, 229, 0); // yellow
        message = ICON_FA_ANGLE_RIGHT "[WARNING] " + std::string(buf);
        break;
    case (int)eLoggerType::Error:
        color = ImColor(255, 157, 0); // orange
        message = ICON_FA_ANGLE_RIGHT "[ERROR] " + std::string(buf);
        break;
    case (int)eLoggerType::Fatal:
        color = ImColor(255, 0, 0); // red
        message = ICON_FA_ANGLE_RIGHT "[FATAL ERROR] " + std::string(buf);
        break;
    case (int)eLoggerType::Info:
        color = ImColor(255, 255, 255); // white
        message = ICON_FA_ANGLE_RIGHT "[CORE] " + std::string(buf);
        break;
    default:
        color = ImColor(255, 255, 255); // white
        message = ICON_FA_ANGLE_RIGHT ""  + std::string(buf);
        break;
    }

    Items.push_back(ConsoleItem(message, color));
}

void GameConsole::AddLogFromScript(std::string message)
{
    AddLog(-1, message.c_str());
}

void    GameConsole::Draw(const char* title)
{
    ImGui::SetNextWindowSize(ImVec2(500, 350), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, NULL, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::End();
        return;
    }

    // Reserve enough left-over height for 1 separator + 1 input text
    //const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    //ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

    for (int i = 0; i < Items.size(); i++)
    {
        const ConsoleItem item = Items[i];
        if (!Filter.PassFilter(item.message.c_str()))
            continue;

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(item.color));
        ImGui::TextWrapped(item.message.c_str());
        ImGui::PopStyleColor();
    }

    if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    ScrollToBottom = false;

    ImGui::PopStyleVar();

    ImGui::End();
}

