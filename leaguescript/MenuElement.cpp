#include "MenuElement.h"

#include "imgui/imgui.h"
#include "Config.h"
#include "KeyNames.h"


/*
 *
 * TODO SAVE TO FILE IN JSON
 *
 */

void Header::Draw()
{
	ImGui::CustomHeaderWithText(m_szText.c_str(), static_cast<float>(*m_iWidth));
}

void Text::Draw()
{
	ImGui::Text(m_szText.c_str());
}

void TextPtr::Draw()
{
	ImGui::Text(m_pText->c_str());
}


void Checkbox::Draw()
{
	ImGui::Checkbox(m_szName.c_str(), m_szDisplayName.c_str(), m_bVal);
}

void Checkbox::SaveToConfig()
{
	std::ofstream file;
	file.open(CFG_FILE, std::ofstream::app);

	auto val = *m_bVal ? "true" : "false";

	file << m_szName << "=" << val << '\n';
}

void Checkbox::LoadFromConfig(std::string menuName)
{
	if (Config::GetConfigValue(menuName, m_szName) != "")
	{
		*m_bVal = Config::ReadBool(menuName, m_szName);
	}
}

void Slider::Draw()
{
	ImGui::SliderInt(m_szName.c_str(), m_szDisplayName.c_str(), m_iVal, m_iMin, m_iMax);
}

void Slider::SaveToConfig()
{
	std::ofstream file;
	file.open(CFG_FILE, std::ofstream::app);

	file << m_szName << "=" << *m_iVal << '\n';
}

void Slider::LoadFromConfig(std::string menuName)
{
	if (Config::GetConfigValue(menuName, m_szName) != "")
		*m_iVal = Config::ReadInt(menuName, m_szName);
}

static auto vector_getter = [](void* vec, int idx, const char** out_text) {
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

void Listbox::Draw()
{
	ImGui::Combo(m_szDisplayName.c_str(), m_iVal, vector_getter, static_cast<void*>(&m_Items), m_Items.size());
}

void Listbox::SaveToConfig()
{
	std::ofstream file;
	file.open(CFG_FILE, std::ofstream::app);

	file << m_szName << "=" << *m_iVal << '\n';
}

void Listbox::LoadFromConfig(std::string menuName)
{
	if (Config::GetConfigValue(menuName, m_szName) != "")
		*m_iVal = Config::ReadInt(menuName, m_szName);
}

void Tooltip::Draw()
{
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(m_szText.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void KeyBind::Draw()
{
	ImGui::Button(KeyNames[*m_iKey].text, ImVec2(75, 0));

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted("Press any key");
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();

		ImGuiIO& io = ImGui::GetIO();
		for (int i = 0; i < 256; i++)
		{
			if (io.KeysDown[i] == 1 && i != VK_PROCESSKEY)
			{
				*m_iKey = i;
			}
		}
	}

	if (!m_szDisplayName.empty())
	{
		ImGui::SameLine();
		ImGui::Text(m_szDisplayName.c_str());
	}
}

void KeyBind::SaveToConfig()
{
	std::ofstream file;
	file.open(CFG_FILE, std::ofstream::app);

	file << m_szName << "=" << *m_iKey << '\n';
}

void KeyBind::LoadFromConfig(std::string menuName)
{
	if (Config::GetConfigValue(menuName, m_szName) != "")
		*m_iKey = Config::ReadInt(menuName, m_szName);
}