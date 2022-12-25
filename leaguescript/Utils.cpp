#include "Utils.h"

#include <algorithm>

namespace Script {
namespace Native {
	std::string Utils::ToLowerCase(std::string str)
	{
		std::string strLower;
		strLower.resize(str.size());

		std::transform(str.begin(),
			str.end(),
			strLower.begin(),
			::tolower);

		return strLower;
	}
}
}
