#include "NetClient.h"

#include "fp_call.h"

namespace Script {
namespace Native {
	NetClient* NetClient::Instance()
	{
		return *reinterpret_cast<NetClient**>(MAKE_RVA(Addresses::NetClient::GetNetClientInstance));
	}

	int NetClient::GetPing()
	{
		return base::this_call<int>(MAKE_RVA(Addresses::NetClient::GetPing), this);
	}
}
}