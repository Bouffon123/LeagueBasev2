#include "ManagerList.h"

namespace Script {
namespace Native {
	ManagerTemplate<GameObject>* ManagerList::HeroList()
	{
		return *reinterpret_cast<ManagerTemplate<GameObject>**>(MAKE_RVA(Addresses::ManagerList::HeroList));
	}

	ManagerTemplate<GameObject>* ManagerList::MinionList()
	{
		return *reinterpret_cast<ManagerTemplate<GameObject>**>(MAKE_RVA(Addresses::ManagerList::MinionList));
	}

	ManagerTemplate<GameObject>* ManagerList::TurretList()
	{
		return *reinterpret_cast<ManagerTemplate<GameObject>**>(MAKE_RVA(Addresses::ManagerList::TurretList));
	}

	ManagerTemplate<GameObject>* ManagerList::MissileList()
	{
		return *reinterpret_cast<ManagerTemplate<GameObject>**>(MAKE_RVA(Addresses::ManagerList::MissileList));
	}

	ManagerTemplate<GameObject>* ManagerList::BuildingList()
	{
		return *reinterpret_cast<ManagerTemplate<GameObject>**>(MAKE_RVA(Addresses::ManagerList::BuildingList));
	}

	ManagerTemplate<GameObject>* ManagerList::InhibitorList()
	{
		return *reinterpret_cast<ManagerTemplate<GameObject>**>(MAKE_RVA(Addresses::ManagerList::InhibitorList));
	}

	ManagerTemplate<GameObject>* ManagerList::AttackableUnitList()
	{
		return *reinterpret_cast<ManagerTemplate<GameObject>**>(MAKE_RVA(Addresses::ManagerList::AttackableUnitList));
	}

	std::vector<GameObject*> ManagerList::GetChampionList()
	{
		std::vector<GameObject*> res{};
		auto heros = HeroList();

		for (auto i = 0; i < heros->size; i++)
		{
			auto hero = heros->entities[i];

			res.push_back(hero);
		}

		return res;
	}

	std::vector<GameObject*> ManagerList::GetTurretList()
	{
		std::vector<GameObject*> res{};
		auto heros = TurretList();

		for (auto i = 0; i < heros->size; i++)
		{
			auto hero = heros->entities[i];

			res.push_back(hero);
		}

		return res;
	}

	std::vector<GameObject*> ManagerList::GetMinionList()
	{
		std::vector<GameObject*> res{};
		auto heros = MinionList();

		for (auto i = 0; i < heros->size; i++)
		{
			auto hero = heros->entities[i];

			res.push_back(hero);
		}

		return res;
	}

	std::vector<GameObject*> ManagerList::GetMissileList()
	{
		std::vector<GameObject*> res{};
		auto heros = MissileList();

		for (auto i = 0; i < heros->size; i++)
		{
			auto hero = heros->entities[i];

			res.push_back(hero);
		}

		return res;
	}

	std::vector<GameObject*> ManagerList::GetBuildingList()
	{
		std::vector<GameObject*> res{};
		auto heros = BuildingList();

		for (auto i = 0; i < heros->size; i++)
		{
			auto hero = heros->entities[i];

			res.push_back(hero);
		}

		return res;
	}

	std::vector<GameObject*> ManagerList::GetInhibitorList()
	{
		std::vector<GameObject*> res{};
		auto heros = InhibitorList();

		for (auto i = 0; i < heros->size; i++)
		{
			auto hero = heros->entities[i];

			res.push_back(hero);
		}

		return res;
	}

	std::vector<GameObject*> ManagerList::GetAttackableUnitList()
	{
		std::vector<GameObject*> res{};
		auto heros = AttackableUnitList();

		for (auto i = 0; i < heros->size; i++)
		{
			auto hero = heros->entities[i];

			res.push_back(hero);
		}

		return res;
	}
}
}