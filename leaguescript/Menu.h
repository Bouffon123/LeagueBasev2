#pragma once

#include "MenuElement.h"

#include <map>

class Menu : MenuElement
{
public:
	static Menu* CreateMenu(std::string name, std::string displayName);
	static Menu* CreateMenu_LUA(std::string name, std::string displayName);
	static void  DeleteLUAMenus();
	static void	 DrawMenus();

	void Delete();

	static void DrawCodeEditor();

	void  AddCheckbox(std::string name, std::string displayName, bool* val);
	void  AddSlider(std::string name, std::string displayName, int min, int max, int* val);
	void  AddListbox(std::string name, std::string displayName, std::vector<std::string> values, int* selectedIdx);
	void  AddTooltip(std::string text);
	void  AddKeybind(std::string name, std::string displayName, int* key);
	void  AddHeader(std::string text);
	void  AddText(std::string text);
	void  AddText2(std::string* text);
	Menu* AddMenu(std::string name, std::string displayName);

	void  AddCheckbox_LUA(std::string checkboxId, std::string text, bool defaultVal);
	void  AddSlider_LUA(std::string sliderId, std::string text, int min, int max, int defaultVal);
	void  AddListbox_LUA(std::string listId, std::string text, std::vector<std::string> values, int defaultIndex);
	void  AddTooltip_LUA(std::string text);
	void  AddKeybind_LUA(std::string keybindId, std::string text, int defaultVal);
	void  AddHeader_LUA(std::string text);
	Menu* AddMenu_LUA(std::string menuId, std::string text);

	void Draw();

	bool GetCheckboxValue(std::string checkboxId);
	int  GetSliderValue(std::string sliderId);
	int  GetListValue(std::string listId);
	int  GetKeybindValue(std::string keybindId);

	static void			SaveEverythingToConfig();
	static void			LoadEverythingFromConfig();
	std::vector<Menu*>  GetSubMenus();
	void				SaveToConfig();
	void				LoadFromConfig(std::string menuName);

	int m_iMenuWidth{ 0 };

private:
	Menu(std::string name, std::string displayName) :
		m_szName(name),
		m_szDisplayName(displayName)
	{}

	std::map<std::string, bool>  m_CheckboxValues{};
	std::map<std::string, int>   m_SliderValues{};
	std::map<std::string, int>   m_KeyValues{};
	std::map<std::string, int>   m_ListValues{};

	std::string				  m_szName;
	std::string				  m_szDisplayName;
	std::vector<MenuElement*> m_vElements;
};