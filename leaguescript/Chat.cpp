#include "Chat.h"

#include "fp_call.h"	

namespace Script {
namespace Native {
	Chat* Chat::Instance()
	{
		return *reinterpret_cast<Chat**>(MAKE_RVA(Addresses::Chat::ChatInstance));
	}

	void* Chat::MenuGui()
	{
		return *reinterpret_cast<void**>(MAKE_RVA(Addresses::Chat::MenuGui));
	}

	void Chat::PrintChat(std::string message)
	{
		base::this_call<void>(MAKE_RVA(Addresses::Chat::PrintChat), this, message.c_str(), 64);
	}

	void Chat::SendChat(std::string message, bool isAllChat)
	{
		if (isAllChat)
			message = "/all " + message;

		base::this_call<void>(MAKE_RVA(Addresses::Chat::SendChat), MenuGui(), message.c_str(), 0);
	}
}
}