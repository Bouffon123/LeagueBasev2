#pragma once

#include "Common.h"
#include "GameObject.h"
#include "ManagerTemplate.h"

namespace Script {
namespace Native {
	class SCRIPT_API ManagerList final
	{
	private:
		MAKEGET(ManagerTemplate<GameObject>*, HeroList)
		MAKEGET(ManagerTemplate<GameObject>*, MinionList)
		MAKEGET(ManagerTemplate<GameObject>*, TurretList)
		MAKEGET(ManagerTemplate<GameObject>*, MissileList)
		MAKEGET(ManagerTemplate<GameObject>*, BuildingList)
		MAKEGET(ManagerTemplate<GameObject>*, InhibitorList)
		MAKEGET(ManagerTemplate<GameObject>*, AttackableUnitList)
	public:
		std::vector<GameObject*> GetChampionList();
		std::vector<GameObject*> GetMinionList();
		std::vector<GameObject*> GetTurretList();
		std::vector<GameObject*> GetMissileList();
		std::vector<GameObject*> GetBuildingList();
		std::vector<GameObject*> GetInhibitorList();
		std::vector<GameObject*> GetAttackableUnitList();
	};
}
}
