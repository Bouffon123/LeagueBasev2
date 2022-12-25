#pragma once

#include <string>
#include <vector>

class MenuElement {
public:
	virtual void Draw() = 0;
	virtual void SaveToConfig() = 0;
	virtual void LoadFromConfig(std::string menuName) = 0;

	std::string GetName() { return m_szName; }

protected:
	std::string m_szName;
};

class Header : public MenuElement {
public:
	Header(std::string text, int* width) :
		m_szText(text),
		m_iWidth(width)
	{}

	void Draw();
	void SaveToConfig() {};
	void LoadFromConfig(std::string menuName) {};
private:
	std::string m_szText;
	int* m_iWidth;
};

class Checkbox : public MenuElement {
public:
	Checkbox(std::string name, std::string displayName, bool* val) :
		m_szDisplayName(displayName),
		m_bVal(val)
	{
		m_szName = name;
	}

	void Draw();
	void SaveToConfig();
	void LoadFromConfig(std::string menuName);

	bool GetValue() { return *m_bVal; }

private:
	std::string m_szDisplayName;
	bool* m_bVal;
};

class TextPtr : public MenuElement
{
public:
	TextPtr(std::string* text) :
		m_pText(text) {}

	void Draw();
	void SaveToConfig() {};
	void LoadFromConfig(std::string menuName) {};

private:
	std::string* m_pText;
};

class Text : public MenuElement
{
public:
	Text(std::string text) :
		m_szText(text) {}

	void Draw();
	void SaveToConfig() {};
	void LoadFromConfig(std::string menuName) {};

private:
	std::string m_szText;
};

class Slider : public MenuElement {
public:
	Slider(std::string name, std::string displayName, int min, int max, int* val) :
		m_szDisplayName(displayName),
		m_iMin(min), m_iMax(max),
		m_iVal(val)
	{
		m_szName = name;
	}

	void Draw();
	void SaveToConfig();
	void LoadFromConfig(std::string menuName);

	int GetValue() { return *m_iVal; }

private:
	std::string m_szDisplayName;
	int			m_iMin;
	int			m_iMax;
	int* m_iVal;
};

class Listbox : public MenuElement {
public:
	Listbox(std::string name, std::string displayName, std::vector<std::string> items, int* val) :
		m_szDisplayName(displayName),
		m_Items(items),
		m_iVal(val)
	{
		m_szName = name;
	}

	void Draw();
	void SaveToConfig();
	void LoadFromConfig(std::string menuName);

	int GetValue() { return *m_iVal; }

private:
	std::string				 m_szDisplayName;
	std::vector<std::string> m_Items;
	int* m_iVal;
};

class Tooltip : public MenuElement {
public:
	Tooltip(std::string tooltip) :
		m_szText(tooltip)
	{
		m_szName = "invalid";
	}

	void Draw();
	void SaveToConfig() {};
	void LoadFromConfig(std::string menuName) {};
private:
	std::string m_szText;
};

class KeyBind : public MenuElement {
public:
	KeyBind(std::string name, std::string displayName, int* key) :
		m_szDisplayName(displayName),
		m_iKey(key)
	{
		m_szName = name;
	}

	void Draw();
	void SaveToConfig();
	void LoadFromConfig(std::string menuName);

	int GetValue() { return *m_iKey; }

private:
	std::string m_szDisplayName;
	int* m_iKey;
};