#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define DELIMITER '='
#define CFG_FILE "config.cfg"

namespace Config
{
	inline std::vector<std::string> Split(const std::string& s, char delim) {
		std::stringstream ss(s);
		std::string item;
		std::vector<std::string> elems;
		while (std::getline(ss, item, delim)) {
			elems.push_back(std::move(item));
		}
		return elems;
	}

	inline std::string GetConfigValue(std::string menuName, std::string key)
	{
		std::string line;
		std::string currMenu;

		std::ifstream file(CFG_FILE);
		while (std::getline(file, line))
		{
			if (line.front() == '[' && line.back() == ']')
				currMenu = line.substr(1, line.size() - 2); // get only the menu name from the cfg file

			auto split = Split(line, DELIMITER);
			if (split[0] == key && currMenu == menuName)
			{
				return split[1];
			}
		}

		return "";
	}

	inline bool ReadBool(std::string menuName, std::string key)
	{
		return GetConfigValue(menuName, key) == "true";
	}

	inline int ReadInt(std::string menuName, std::string key)
	{
		return std::stoi(GetConfigValue(menuName, key));
	}
}