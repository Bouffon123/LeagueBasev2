#include "ObjectManager.h"

#include "fp_call.h"

namespace Script {
namespace Native {

	ObjectManager* ObjectManager::Instance()
	{
		return *reinterpret_cast<ObjectManager**>(MAKE_RVA(Addresses::ObjectManager::ObjectManagerInstance));
	}

	GameObject* ObjectManager::GetFirstObject()
	{
		return base::this_call<GameObject*>(MAKE_RVA(Addresses::ObjectManager::GetFirstObject), this);
	}

	GameObject* ObjectManager::GetNextObject(GameObject* object)
	{
		return base::this_call<GameObject*>(MAKE_RVA(Addresses::ObjectManager::GetNextObject), this, object);
	}


	GameObject* ObjectManager::GetObjectById(unsigned id)
	{
		auto obj = Instance()->GetFirstObject();

		while (obj)
		{
			if (obj->index() == id)
				return obj;

			obj = Instance()->GetNextObject(obj);
		}

		return nullptr;
	}

	GameObject* ObjectManager::GetObjectByNetworkId(unsigned networkId)
	{
		auto obj = Instance()->GetFirstObject();

		while (obj)
		{
			if (obj->networkId() == networkId)
				return obj;

			obj = Instance()->GetNextObject(obj);
		}

		return nullptr;
	}
}
}