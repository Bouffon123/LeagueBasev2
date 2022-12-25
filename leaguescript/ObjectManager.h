#pragma once

#include "Common.h"

#include "GameObject.h"

namespace Script {
namespace Native {
	class SCRIPT_API ObjectManager final
	{
	private:
		static ObjectManager* Instance();
		GameObject* GetFirstObject();
		GameObject* GetNextObject(GameObject* object);
	public:
		static GameObject* GetObjectByNetworkId(unsigned networkId);
		static GameObject* GetObjectById(unsigned id);
	};
}
}