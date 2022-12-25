#pragma once

#include "Common.h"

namespace Script {
namespace Native {
	class SCRIPT_API Chat final
	{
	private:
		Chat* Instance();
		void* MenuGui();
	public:
		void PrintChat(std::string message);
		void SendChat(std::string message, bool isAllChat);
	};
}
}