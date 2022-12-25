#include "GameObject.h"

#include "fp_call.h"
#include "ProcessedSpell.h"
#include "Hook.h"
#include "Decrypt.h"
#include "Utils.h"

namespace Script {
namespace Native {
#define FOCUS_CLASS GameObject
	MAKEGET_C(unsigned, index)
	MAKEGET_C(GameObjectTeam, team)
	MAKEGET_C(std::string, name)
	MAKEGET_C(unsigned, networkId)
	MAKEGET_C(Vec3, position)
	MAKEGET_C(Vec3, direction)
	MAKEGET_C(unsigned, currentGolds)
	MAKEGET_C(MissileClient*, missileClient)
	MAKEGET_C(unsigned, visibility)
	MAKEGET_C(unsigned, srcIndex)
	MAKEGET_C(float, mana)
	MAKEGET_C(float, maxMana)
	MAKEGET_C(RecallState, recallState)
	MAKEGET_C(float, health)
	MAKEGET_C(float, maxHealth)
	MAKEGET_C(float, abilityHaste)
	MAKEGET_C(float, lethality)
	MAKEGET_C(float, armor)
	MAKEGET_C(float, bonusArmor)
	MAKEGET_C(float, magicRes)
	MAKEGET_C(float, bonusMagicRes)
	MAKEGET_C(float, baseAD)
	MAKEGET_C(float, bonusAD)
	MAKEGET_C(float, movementSpeed)
	MAKEGET_C(CombatType, combatType)
	MAKEGET_C(SpellBook*, SpellBookInstance)
	MAKEGET_C(std::string, championName)
	MAKEGET_C(unsigned, level)
	MAKEGET_C(float, critDamageMultiplier)
	MAKEGET_C(float, crit)
	MAKEGET_C(float, baseAP)
	MAKEGET_C(float, bonusAP)
	MAKEGET_C(float, bonusAPMultiplier)
	MAKEGET_C(float, attackSpeedMultiplier)
	MAKEGET_C(float, attackRange)
	MAKEGET_C(float, isTargetable)
	MAKEGET_C(bool, isInvulnerable)
	MAKEGET_C(unsigned, levelUpPoints)
	MAKEGET_C(std::vector<ItemSlot*>, itemList)
	MAKEGET_C(float, cumulatedExperience)
	MAKEGET_C(float, magicPenFlat)
	MAKEGET_C(float, armorPenMultiplier)
	MAKEGET_C(float, magicPenMultiplier)
	MAKEGET_C(float, manaRegen)
	MAKEGET_C(float, healthRegen)
#undef FOCUS_CLASS

	Hook< convention_type::thiscall_t, int, GameObject*, unsigned > HookOnCreateObject;
	Hook< convention_type::thiscall_t, int, void*, GameObject* > HookOnDeleteObject;
	Hook< convention_type::thiscall_t, int, SpellBook*, ProcessedSpell* > HookOnProcessSpell;

	class Wrapper
	{
	public:
		static int __fastcall OnCreateObjectCallee(GameObject* thisPtr, void*, unsigned networkId)
		{
			__dbg("[OnCreate Hook] - Created object %s", thisPtr->name().c_str());

			return HookOnCreateObject.CallOriginal(thisPtr, networkId);
		}

		static int __fastcall OnDeleteObjectCallee(void* thisPtr, void*, GameObject* sender)
		{
			__dbg("[OnDelete Hook] - Deleted object %s", sender->name().c_str());

			return HookOnDeleteObject.CallOriginal(thisPtr, sender);
		}

		static int __fastcall OnProcessSpellCallee(SpellBook* thisPtr, void*, ProcessedSpell* castInfo)
		{
			__dbg("[OnProcessSpell Hook] - Detected spell: %s", castInfo->spellInfo()->spellName().c_str());

			return HookOnProcessSpell.CallOriginal(thisPtr, castInfo);
		}
	};


	bool GameObject::operator==(GameObject* other)
	{
		return this->networkId() == other->networkId();
	}

	bool GameObject::operator!=(GameObject* other)
	{
		return !(this == other);
	}

	/*
	 *		VTABLE FUNCTIONS
	 */

	DWORD* GameObject::VTable()
	{
		return *reinterpret_cast<DWORD**>(this);
	}

	int GameObject::GetBoundingRadius()
	{
		typedef int(__thiscall* vtableFunc)(PVOID);
		return base::call_virtual<vtableFunc>(this, static_cast<DWORD>(Structures::GameObjectVTable::GetBoundingRadius))(this);
	}

	bool GameObject::IsVisible()
	{
		typedef bool(__thiscall* vtableFunc)(PVOID);
		return base::call_virtual<vtableFunc>(this, static_cast<DWORD>(Structures::GameObjectVTable::IsVisible))(this);
	}

	bool GameObject::IsZombie()
	{
		typedef bool(__thiscall* vtableFunc)(PVOID);
		return base::call_virtual<vtableFunc>(this, static_cast<DWORD>(Structures::GameObjectVTable::IsZombie))(this);
	}

	bool GameObject::IsTargetableToAllies()
	{
		typedef bool(__thiscall* vtableFunc)(PVOID);
		return base::call_virtual<vtableFunc>(this, static_cast<DWORD>(Structures::GameObjectVTable::IsTargetableToAllies))(this);
	}

	bool GameObject::IsTargetableToEnemies()
	{
		typedef bool(__thiscall* vtableFunc)(PVOID);
		return base::call_virtual<vtableFunc>(this, static_cast<DWORD>(Structures::GameObjectVTable::IsTargetableToEnemies))(this);
	}

	PathControllerCommon* GameObject::GetPathControllerCommon()
	{
		typedef PathControllerCommon*(__thiscall* vtableFunc)(PVOID);
		return base::call_virtual<vtableFunc>(this, static_cast<DWORD>(Structures::GameObjectVTable::GetPathControllerCommon))(this);
	}

	double GameObject::GetRespawnTimeRemaining()
	{
		typedef double(__thiscall* vtableFunc)(PVOID);
		return base::call_virtual<vtableFunc>(this, static_cast<DWORD>(Structures::GameObjectVTable::GetRespawnTimeRemaining))(this);
	}

	/*
	 *		GAME FUNCTIONS
	 */

	bool GameObject::CompareObjectTypeFlag(TypeFlag flag)
	{
		return base::this_call<bool>(MAKE_RVA(Addresses::GameObjectAddresses::CompareTypeFlags), this, static_cast<int>(flag));
	}

	float GameObject::GetAttackDelay()
	{
		return base::this_call<float>(MAKE_RVA(Addresses::GameObjectAddresses::GetAttackDelay), this);
	}

	float GameObject::GetAttackCastDelay()
	{
		return base::this_call<float>(MAKE_RVA(Addresses::GameObjectAddresses::GetAttackCastDelay), this);
	}

	SpellInfo* GameObject::GetBasicAttack()
	{
		return base::this_call<SpellInfo*>(MAKE_RVA(Addresses::GameObjectAddresses::GetBasicAttack), this);
	}

	Vec2 GameObject::GetHealthBarScreenPosition()
	{
		return Vec2{}; // TODO TO IMPLEMENT CF UC POST ABOUT ENCRYPTED HP BAR POS
	}

	bool GameObject::IsAlive()
	{
		// At the end of OG IsAlive function.
		constexpr uint32_t mods[2]{ 0x94, 0x8 };
		return !(*reinterpret_cast<bool(__thiscall**)(DWORD*)>(*(reinterpret_cast<DWORD*>(this) + mods[0]) + mods[1]))(reinterpret_cast<DWORD*>(this) + mods[0]);
	}

	/*
	 *		CUSTOM IMPLEMENTATION FUNCTIONS
	 */

	bool GameObject::IsAlly()
	{
		return team() == LocalPlayer()->team();
	}

	bool GameObject::IsAlly(GameObject* other)
	{
		return team() == other->team();
	}

	bool GameObject::IsEnemy()
	{
		return !IsAlly();
	}

	bool GameObject::IsEnemy(GameObject* other)
	{
		return !IsAlly(other);
	}

	bool GameObject::IsNeutral()
	{
		return team() == GameObjectTeam::Neutral;
	}

	bool GameObject::IsMinion()
	{
		return CompareObjectTypeFlag(TypeFlag::Minion);
	}

	bool GameObject::IsChampion()
	{
		return CompareObjectTypeFlag(TypeFlag::Hero);
	}

	bool GameObject::IsTurret()
	{
		return CompareObjectTypeFlag(TypeFlag::Turret);
	}

	bool GameObject::IsMissile()
	{
		return CompareObjectTypeFlag(TypeFlag::Missile);
	}

	bool GameObject::IsWard()
	{
		return !IsPlant() && maxHealth() <= 5;
	}

	bool GameObject::IsPlant()
	{
		return name().find("Plant") != std::string::npos;
	}


	bool GameObject::IsClone()
	{
		if (!IsMinion())
			return false;

		static std::string championClones[] =
		{
			"shaco",
			"leblanc",
			"monkeyking",
			"neeko",
			"fiddlesticks"
		};

		for (const std::string& s : championClones)
		{
			if (s == Utils::ToLowerCase(championName()))
				return true;
		}

		return false;
	}

	bool GameObject::IsPracticeToolDummy()
	{
		if (!IsChampion())
			return false;

		return Utils::ToLowerCase(championName()) == "practicetool_targetdummy";
	}

	bool GameObject::IsMelee()
	{
		return combatType() == CombatType::Melee;
	}

	bool GameObject::IsRanged()
	{
		return !IsMelee();
	}

	bool GameObject::IsRecalling()
	{
		return recallState() == RecallState::Recall || recallState() == RecallState::EmpoweredRecall;
	}

	bool GameObject::IsTeleporting()
	{
		return recallState() == RecallState::Teleport;
	}

	ItemSlot* GameObject::GetItemBySlot(int id)
	{
		return *reinterpret_cast<ItemSlot**>(this + static_cast<unsigned int>(Structures::GameObject::itemList) + 0x4 * id);
	}

	GameObject* GameObject::LocalPlayer() {
		return *reinterpret_cast<GameObject**>(MAKE_RVA(Addresses::LocalPlayer::LocalPlayerInstance));
	}


	bool GameObject::ApplyHooks()
	{
		HookOnCreateObject.ApplyHWBP(MAKE_RVA(Addresses::GameObjectAddresses::OnCreateObject), (DWORD)Wrapper::OnCreateObjectCallee, 0, 0xAE);
		HookOnDeleteObject.ApplyHWBP(MAKE_RVA(Addresses::GameObjectAddresses::OnDeleteObject), (DWORD)Wrapper::OnDeleteObjectCallee, 1, 0x158);
		HookOnProcessSpell.ApplyVEH(MAKE_RVA(Addresses::GameObjectAddresses::OnProcessSpell), (DWORD)Wrapper::OnProcessSpellCallee, 0,  0x5C);

		return HookOnCreateObject.IsApplied()
			&& HookOnDeleteObject.IsApplied()
			&& HookOnProcessSpell.IsApplied();
	}
}
}
