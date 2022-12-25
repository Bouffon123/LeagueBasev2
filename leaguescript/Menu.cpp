#include "Menu.h"

#include "imgui/imgui.h"
#include "Context.h"
#include "Config.h"

#include <iostream>
#include <fstream>

Menu* Menu::CreateMenu(std::string name, std::string displayName)
{
	Menu* newMenu = new Menu(name, displayName);
	g_ctx->mRootMenus.push_back(newMenu);

	return newMenu;
}

Menu* Menu::CreateMenu_LUA(std::string name, std::string displayName)
{
	Menu* newMenu = new Menu(name, displayName);
	g_ctx->mApiMenus.push_back(newMenu);

	return newMenu;
}

void Menu::Delete()
{
	for (auto elt : m_vElements)
	{
		if (Menu* v = dynamic_cast<Menu*>(elt)) {
			v->Delete();
		}
	}

	m_vElements.clear();
}

void Menu::DeleteLUAMenus()
{
	for (auto menu : g_ctx->mApiMenus)
	{
		menu->Delete();
	}

	g_ctx->mApiMenus.clear();
}

void EmptyCfgFile()
{
	std::ofstream file;
	file.open(CFG_FILE, std::ofstream::out | std::ofstream::trunc);
	file.close();
}

void Menu::SaveEverythingToConfig()
{
	EmptyCfgFile();

	std::vector<Menu*> menus{};

	for (auto rootMenu : g_ctx->mRootMenus)
	{
		menus.push_back(rootMenu);
		auto subMenus = rootMenu->GetSubMenus();
		menus.insert(menus.end(), subMenus.begin(), subMenus.end());
	}

	for (auto rootMenu : g_ctx->mRootMenus)
	{
		menus.push_back(rootMenu);
		auto subMenus = rootMenu->GetSubMenus();
		menus.insert(menus.end(), subMenus.begin(), subMenus.end());
	}

	for (auto menu : menus)
		menu->SaveToConfig();
}

void Menu::LoadEverythingFromConfig()
{
	// check if config file exists, return if it doesnt.
	struct stat buffer;
	if (stat(CFG_FILE, &buffer) != 0)
		return;

	for (auto rootMenu : g_ctx->mRootMenus)
	{
		rootMenu->LoadFromConfig(rootMenu->m_szName);
	}
	for (auto rootMenu : g_ctx->mApiMenus)
	{
		rootMenu->LoadFromConfig(rootMenu->m_szName);
	}
}

std::vector<Menu*> Menu::GetSubMenus()
{
	std::vector<Menu*> menus{};
	for (auto elt : m_vElements)
	{
		if (Menu* m = dynamic_cast<Menu*>(elt))
		{
			menus.push_back(m);
			auto subMenus = m->GetSubMenus();
			menus.insert(menus.begin(), subMenus.begin(), subMenus.end());
		}
	}

	return menus;
}

void Menu::SaveToConfig()
{
	std::ofstream file;
	file.open(CFG_FILE, std::ios_base::app);
	file << "[" << m_szName << "]" << '\n';
	file.close();

	for (MenuElement* elt : m_vElements)
	{
		// we dont want to save submenus to file since we handle it in the method above already.
		if (Menu* m = dynamic_cast<Menu*>(elt))
		{
		}
		else
			elt->SaveToConfig();
	}
}

void Menu::LoadFromConfig(std::string menuName)
{
	for (auto elt : m_vElements)
		elt->LoadFromConfig(m_szName);
}

void Menu::Draw()
{
	if (ImGui::BeginMenu(m_szDisplayName.c_str()))
	{
		for (MenuElement* elt : m_vElements)
			elt->Draw();

		ImGui::EndMenu();
	}
}

bool Menu::GetCheckboxValue(std::string checkboxId)
{
	for (auto checkBox : m_CheckboxValues)
	{
		if (checkBox.first == checkboxId)
		{
			return checkBox.second;
		}
	}

	return false;
}

int Menu::GetSliderValue(std::string sliderId)
{
	for (auto slider : m_SliderValues)
	{
		if (slider.first == sliderId)
		{
			return slider.second;
		}
	}

	return 0;
}

int Menu::GetListValue(std::string listId)
{
	for (auto list : m_ListValues)
	{
		if (list.first == listId)
		{
			return list.second;
		}
	}

	return 0;
}

int Menu::GetKeybindValue(std::string keybindId)
{
	for (auto key : m_KeyValues)
	{
		if (key.first == keybindId)
		{
			return key.second;
		}
	}

	return 0;
}

Menu* Menu::AddMenu(std::string name, std::string displayName)
{
	Menu* newMenu = new Menu(name, displayName);
	this->m_vElements.push_back(newMenu);

	auto size = ImGui::CalcTextSize(displayName.c_str(), NULL, true, 300);
	if (m_iMenuWidth < static_cast<int>(size.x) + 20)
		m_iMenuWidth = static_cast<int>(size.x) + 20;

	return newMenu;
}

void Menu::AddCheckbox(std::string name, std::string displayName, bool* val)
{
	Checkbox* newCb = new Checkbox(name, displayName, val);
	this->m_vElements.push_back(newCb);
	auto size = ImGui::CalcTextSize(displayName.c_str(), NULL, true, 300);
	if (m_iMenuWidth < static_cast<int>(size.x) + 20)
		m_iMenuWidth = static_cast<int>(size.x) + 20;
}

void Menu::AddText2(std::string* text)
{
	TextPtr* newText = new TextPtr(text);
	this->m_vElements.push_back(newText);
	auto size = ImGui::CalcTextSize(text->c_str(), NULL, true, 300);
	if (m_iMenuWidth < static_cast<int>(size.x))
		m_iMenuWidth = static_cast<int>(size.x);
}

void Menu::AddText(std::string text)
{
	Text* newText = new Text(text);
	this->m_vElements.push_back(newText);
	auto size = ImGui::CalcTextSize(text.c_str(), NULL, true, 300);
	if (m_iMenuWidth < static_cast<int>(size.x))
		m_iMenuWidth = static_cast<int>(size.x);
}

void Menu::AddSlider(std::string name, std::string displayName, int min, int max, int* val)
{
	Slider* newSlider = new Slider(name, displayName, min, max, val);
	this->m_vElements.push_back(newSlider);
	auto size = ImGui::CalcTextSize(displayName.c_str(), NULL, true, 300);
	if (m_iMenuWidth < static_cast<int>(size.x) + 180)
		m_iMenuWidth = static_cast<int>(size.x) + 180;
}

void Menu::AddListbox(std::string name, std::string displayName, std::vector<std::string> values, int* selectedIdx)
{
	Listbox* listbox = new Listbox(name, displayName, values, selectedIdx);
	this->m_vElements.push_back(listbox);
	auto size = ImGui::CalcTextSize(displayName.c_str(), NULL, true, 300);
	if (m_iMenuWidth < static_cast<int>(size.x) + 250)
		m_iMenuWidth = static_cast<int>(size.x) + 250;
}

void Menu::AddTooltip(std::string text)
{
	Tooltip* tooltip = new Tooltip(text);
	this->m_vElements.push_back(tooltip);
}

void Menu::AddKeybind(std::string name, std::string displayName, int* key)
{
	KeyBind* newKb = new KeyBind(name, displayName, key);
	this->m_vElements.push_back(newKb);

	auto size = ImGui::CalcTextSize(displayName.c_str(), NULL, true, 300);
	if (m_iMenuWidth < static_cast<int>(size.x) + 50)
		m_iMenuWidth = static_cast<int>(size.x) + 50;
}

void Menu::AddHeader(std::string text)
{
	auto size = ImGui::CalcTextSize(text.c_str(), NULL, true, 300);
	if (m_iMenuWidth < static_cast<int>(size.x) + 10)
		m_iMenuWidth = static_cast<int>(size.x) + 10;

	Header* header = new Header(text, &m_iMenuWidth);

	this->m_vElements.push_back(header);
}

void Menu::AddCheckbox_LUA(std::string checkboxId, std::string text, bool defaultVal)
{
	auto it = m_CheckboxValues.find(checkboxId);

	if (it == m_CheckboxValues.end())
	{
		m_CheckboxValues.emplace(checkboxId, defaultVal);
		it = m_CheckboxValues.find(checkboxId);
	}

	this->AddCheckbox(checkboxId, text, &it->second);
}

void Menu::AddSlider_LUA(std::string sliderId, std::string text, int min, int max, int defaultVal)
{
	auto it = m_SliderValues.find(sliderId);

	if (it == m_SliderValues.end())
	{
		m_SliderValues.emplace(sliderId, defaultVal);
		it = m_SliderValues.find(sliderId);
	}

	this->AddSlider(sliderId, text, min, max, &it->second);
}

void Menu::AddListbox_LUA(std::string listId, std::string text, std::vector<std::string> values, int defaultIndex)
{
	auto it = m_ListValues.find(listId);

	if (it == m_ListValues.end())
	{
		m_ListValues.emplace(listId, defaultIndex);
		it = m_ListValues.find(listId);
	}

	this->AddListbox(listId, text, values, &it->second);
}

void Menu::AddKeybind_LUA(std::string keybindId, std::string text, int defaultVal)
{
	auto it = m_KeyValues.find(keybindId);

	if (it == m_KeyValues.end())
	{
		m_KeyValues.emplace(keybindId, defaultVal);
		it = m_KeyValues.find(keybindId);
	}

	this->AddKeybind(keybindId, text, &it->second);
}

void Menu::AddTooltip_LUA(std::string text)
{
	this->AddTooltip(text);
}

void Menu::AddHeader_LUA(std::string text)
{
	this->AddHeader(text);
}

Menu* Menu::AddMenu_LUA(std::string menuId, std::string text)
{
	return this->AddMenu(menuId, text);
}

void DrawColorEditor()
{
	static ImGuiStyle& style = ImGui::GetStyle();
	static float text[4]{ style.Colors[ImGuiCol_::ImGuiCol_Text].x, style.Colors[ImGuiCol_::ImGuiCol_Text].y, style.Colors[ImGuiCol_::ImGuiCol_Text].z, style.Colors[ImGuiCol_::ImGuiCol_Text].w };
	static float textDisabled[4]{ style.Colors[ImGuiCol_::ImGuiCol_TextDisabled].x, style.Colors[ImGuiCol_::ImGuiCol_TextDisabled].y, style.Colors[ImGuiCol_::ImGuiCol_TextDisabled].z, style.Colors[ImGuiCol_::ImGuiCol_TextDisabled].w };
	static float windowbg[4]{ style.Colors[ImGuiCol_::ImGuiCol_WindowBg].x, style.Colors[ImGuiCol_::ImGuiCol_WindowBg].y, style.Colors[ImGuiCol_::ImGuiCol_WindowBg].z, style.Colors[ImGuiCol_::ImGuiCol_WindowBg].w };
	static float childbg[4]{ style.Colors[ImGuiCol_::ImGuiCol_ChildBg].x, style.Colors[ImGuiCol_::ImGuiCol_ChildBg].y, style.Colors[ImGuiCol_::ImGuiCol_ChildBg].z, style.Colors[ImGuiCol_::ImGuiCol_ChildBg].w };
	static float ImGuiCol_PopupBg[4]{ style.Colors[ImGuiCol_::ImGuiCol_PopupBg].x, style.Colors[ImGuiCol_::ImGuiCol_PopupBg].y, style.Colors[ImGuiCol_::ImGuiCol_PopupBg].z, style.Colors[ImGuiCol_::ImGuiCol_PopupBg].w };
	static float ImGuiCol_Border[4]{ style.Colors[ImGuiCol_::ImGuiCol_Border].x, style.Colors[ImGuiCol_::ImGuiCol_Border].y, style.Colors[ImGuiCol_::ImGuiCol_Border].z, style.Colors[ImGuiCol_::ImGuiCol_Border].w };
	static float ImGuiCol_BorderShadow[4]{ style.Colors[ImGuiCol_::ImGuiCol_BorderShadow].x, style.Colors[ImGuiCol_::ImGuiCol_BorderShadow].y, style.Colors[ImGuiCol_::ImGuiCol_BorderShadow].z, style.Colors[ImGuiCol_::ImGuiCol_BorderShadow].w };
	static float ImGuiCol_FrameBg[4]{ style.Colors[ImGuiCol_::ImGuiCol_FrameBg].x, style.Colors[ImGuiCol_::ImGuiCol_FrameBg].y, style.Colors[ImGuiCol_::ImGuiCol_FrameBg].z, style.Colors[ImGuiCol_::ImGuiCol_FrameBg].w };
	static float ImGuiCol_FrameBgHovered[4]{ style.Colors[ImGuiCol_::ImGuiCol_FrameBgHovered].x, style.Colors[ImGuiCol_::ImGuiCol_FrameBgHovered].y, style.Colors[ImGuiCol_::ImGuiCol_FrameBgHovered].z, style.Colors[ImGuiCol_::ImGuiCol_FrameBgHovered].w };
	static float ImGuiCol_FrameBgActive[4]{ style.Colors[ImGuiCol_::ImGuiCol_FrameBgActive].x, style.Colors[ImGuiCol_::ImGuiCol_FrameBgActive].y, style.Colors[ImGuiCol_::ImGuiCol_FrameBgActive].z, style.Colors[ImGuiCol_::ImGuiCol_FrameBgActive].w };
	static float ImGuiCol_TitleBg[4]{ style.Colors[ImGuiCol_::ImGuiCol_TitleBg].x, style.Colors[ImGuiCol_::ImGuiCol_TitleBg].y, style.Colors[ImGuiCol_::ImGuiCol_TitleBg].z, style.Colors[ImGuiCol_::ImGuiCol_TitleBg].w };
	static float ImGuiCol_TitleBgActive[4]{ style.Colors[ImGuiCol_::ImGuiCol_TitleBgActive].x, style.Colors[ImGuiCol_::ImGuiCol_TitleBgActive].y, style.Colors[ImGuiCol_::ImGuiCol_TitleBgActive].z, style.Colors[ImGuiCol_::ImGuiCol_TitleBgActive].w };
	static float ImGuiCol_MenuBarBg[4]{ style.Colors[ImGuiCol_::ImGuiCol_MenuBarBg].x, style.Colors[ImGuiCol_::ImGuiCol_MenuBarBg].y, style.Colors[ImGuiCol_::ImGuiCol_MenuBarBg].z, style.Colors[ImGuiCol_::ImGuiCol_MenuBarBg].w };
	static float ImGuiCol_Button[4]{ style.Colors[ImGuiCol_::ImGuiCol_Button].x, style.Colors[ImGuiCol_::ImGuiCol_Button].y, style.Colors[ImGuiCol_::ImGuiCol_Button].z, style.Colors[ImGuiCol_::ImGuiCol_Button].w };
	static float ImGuiCol_ButtonHovered[4]{ style.Colors[ImGuiCol_::ImGuiCol_ButtonHovered].x, style.Colors[ImGuiCol_::ImGuiCol_ButtonHovered].y, style.Colors[ImGuiCol_::ImGuiCol_ButtonHovered].z, style.Colors[ImGuiCol_::ImGuiCol_ButtonHovered].w };
	static float ImGuiCol_ButtonActive[4]{ style.Colors[ImGuiCol_::ImGuiCol_ButtonActive].x, style.Colors[ImGuiCol_::ImGuiCol_ButtonActive].y, style.Colors[ImGuiCol_::ImGuiCol_ButtonActive].z, style.Colors[ImGuiCol_::ImGuiCol_ButtonActive].w };
	static float ImGuiCol_Header[4]{ style.Colors[ImGuiCol_::ImGuiCol_Header].x, style.Colors[ImGuiCol_::ImGuiCol_Header].y, style.Colors[ImGuiCol_::ImGuiCol_Header].z, style.Colors[ImGuiCol_::ImGuiCol_Header].w };
	static float ImGuiCol_HeaderActive[4]{ style.Colors[ImGuiCol_::ImGuiCol_HeaderActive].x, style.Colors[ImGuiCol_::ImGuiCol_HeaderActive].y, style.Colors[ImGuiCol_::ImGuiCol_HeaderActive].z, style.Colors[ImGuiCol_::ImGuiCol_HeaderActive].w };
	ImGui::Begin("Color editor", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	ImGui::ColorEdit4("text", text);
	ImGui::ColorEdit4("text disabled", textDisabled);
	ImGui::ColorEdit4("windowbg", windowbg);
	ImGui::ColorEdit4("childbg", childbg);
	ImGui::ColorEdit4("ImGuiCol_PopupBg", ImGuiCol_PopupBg);
	ImGui::ColorEdit4("ImGuiCol_Border", ImGuiCol_Border);
	ImGui::ColorEdit4("ImGuiCol_BorderShadow", ImGuiCol_BorderShadow);
	ImGui::ColorEdit4("ImGuiCol_FrameBg", ImGuiCol_FrameBg);
	ImGui::ColorEdit4("ImGuiCol_FrameBgHovered", ImGuiCol_FrameBgHovered);
	ImGui::ColorEdit4("ImGuiCol_FrameBgActive", ImGuiCol_FrameBgActive);
	ImGui::ColorEdit4("ImGuiCol_TitleBg", ImGuiCol_TitleBg);
	ImGui::ColorEdit4("ImGuiCol_TitleBgActive", ImGuiCol_TitleBgActive);
	ImGui::ColorEdit4("ImGuiCol_MenuBarBg", ImGuiCol_MenuBarBg);
	ImGui::ColorEdit4("ImGuiCol_Button", ImGuiCol_Button);
	ImGui::ColorEdit4("ImGuiCol_ButtonHovered", ImGuiCol_ButtonHovered);
	ImGui::ColorEdit4("ImGuiCol_ButtonActive", ImGuiCol_ButtonActive);
	ImGui::ColorEdit4("ImGuiCol_Header", ImGuiCol_Header);
	ImGui::ColorEdit4("ImGuiCol_HeaderActive", ImGuiCol_HeaderActive);
	ImGui::End();

	style.Colors[ImGuiCol_::ImGuiCol_Text] = ImVec4(text);
	style.Colors[ImGuiCol_::ImGuiCol_TextDisabled] = ImVec4(textDisabled);
	style.Colors[ImGuiCol_::ImGuiCol_WindowBg] = ImVec4(windowbg);
	style.Colors[ImGuiCol_::ImGuiCol_ChildBg] = ImVec4(childbg);
	style.Colors[ImGuiCol_::ImGuiCol_PopupBg] = ImVec4(ImGuiCol_PopupBg);
	style.Colors[ImGuiCol_::ImGuiCol_Border] = ImVec4(ImGuiCol_Border);
	style.Colors[ImGuiCol_::ImGuiCol_BorderShadow] = ImVec4(ImGuiCol_BorderShadow);
	style.Colors[ImGuiCol_::ImGuiCol_FrameBg] = ImVec4(ImGuiCol_FrameBg);
	style.Colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = ImVec4(ImGuiCol_FrameBgHovered);
	style.Colors[ImGuiCol_::ImGuiCol_FrameBgActive] = ImVec4(ImGuiCol_FrameBgActive);
	style.Colors[ImGuiCol_::ImGuiCol_TitleBg] = ImVec4(ImGuiCol_TitleBg);
	style.Colors[ImGuiCol_::ImGuiCol_TitleBgActive] = ImVec4(ImGuiCol_TitleBgActive);
	style.Colors[ImGuiCol_::ImGuiCol_MenuBarBg] = ImVec4(ImGuiCol_MenuBarBg);
	style.Colors[ImGuiCol_::ImGuiCol_Button] = ImVec4(ImGuiCol_Button);
	style.Colors[ImGuiCol_::ImGuiCol_ButtonHovered] = ImVec4(ImGuiCol_ButtonHovered);
	style.Colors[ImGuiCol_::ImGuiCol_ButtonActive] = ImVec4(ImGuiCol_ButtonActive);
	style.Colors[ImGuiCol_::ImGuiCol_Header] = ImVec4(ImGuiCol_Header);
	style.Colors[ImGuiCol_::ImGuiCol_HeaderActive] = ImVec4(ImGuiCol_HeaderActive);
}

void Menu::DrawMenus()
{
	ImGui::SetNextWindowPos(ImVec2(904, 349), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(160, 40), ImVec2(99999, 99999));
	ImGui::Begin(ICON_FA_BUG "Zekware internal", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	{
		for (Menu* menu : g_ctx->mRootMenus)
			menu->Draw();
		for (Menu* menu : g_ctx->mApiMenus)
			menu->Draw();
	}ImGui::End();
}