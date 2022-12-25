#pragma once

#include "Common.h"
#include "PathControllerCommon.h"
#include "Vector.h"
#include "GameObjectEnums.h"
#include "SpellEnums.h"
#include "ItemSlot.h"
#include "SpellBook.h"
#include "MissileClient.h"

namespace Script {
namespace Native {

	// TODO: test VTable funcs for IsWard, IsPlant etc.

	class SCRIPT_API GameObject final
	{
	private:
		DWORD* VTable();
	public:
		MAKEGET(unsigned, index)
		MAKEGET(GameObjectTeam, team)
		MAKEGET(std::string, name)
		MAKEGET(unsigned, networkId)
		MAKEGET(Vec3, position)
		MAKEGET(Vec3, direction)
		MAKEGET(unsigned, currentGolds)
		MAKEGET(MissileClient*, missileClient)
		MAKEGET(unsigned, visibility)
		MAKEGET(unsigned, srcIndex)
		MAKEGET(float, mana)
		MAKEGET(float, maxMana)
		MAKEGET(RecallState, recallState)
		MAKEGET(float, health)
		MAKEGET(float, maxHealth)
		MAKEGET(float, abilityHaste)
		MAKEGET(float, lethality)
		MAKEGET(float, armor)
		MAKEGET(float, bonusArmor)
		MAKEGET(float, magicRes)
		MAKEGET(float, bonusMagicRes)
		MAKEGET(float, baseAD)
		MAKEGET(float, bonusAD)
		MAKEGET(float, movementSpeed)
		MAKEGET(CombatType, combatType)
		MAKEGET(SpellBook*, SpellBookInstance)
		MAKEGET(std::string, championName)
		MAKEGET(unsigned, level)
		MAKEGET(float, critDamageMultiplier)
		MAKEGET(float, crit)
		MAKEGET(float, baseAP)
		MAKEGET(float, bonusAP)
		MAKEGET(float, bonusAPMultiplier)
		MAKEGET(float, attackSpeedMultiplier)
		MAKEGET(float, attackRange)
		MAKEGET(float, isTargetable)
		MAKEGET(bool, isInvulnerable)
		MAKEGET(unsigned, levelUpPoints)
		MAKEGET(std::vector<ItemSlot*>, itemList)
		MAKEGET(float, cumulatedExperience)
		MAKEGET(float, magicPenFlat)
		MAKEGET(float, armorPenMultiplier)
		MAKEGET(float, magicPenMultiplier)
		MAKEGET(float, manaRegen)
		MAKEGET(float, healthRegen)


		/*
		 *		VTABLE FUNCTIONS
		 */
		int GetBoundingRadius();
		bool IsVisible();
		bool IsZombie();
		bool IsTargetableToAllies();
		bool IsTargetableToEnemies();
		PathControllerCommon* GetPathControllerCommon();
		double GetRespawnTimeRemaining();

		float GetAttackDelay();
		float GetAttackCastDelay();
		SpellInfo* GetBasicAttack();
		Vec2 GetHealthBarScreenPosition();
		bool IsAlive();
		bool CompareObjectTypeFlag(TypeFlag flag);

		bool IsMelee();
		bool IsRanged();

		bool IsAlly();
		bool IsAlly(GameObject* other);
		bool IsEnemy();
		bool IsEnemy(GameObject* other);
		bool IsNeutral();

		bool IsMinion();
		bool IsChampion();
		bool IsTurret();
		bool IsMissile();
		bool IsWard();
		bool IsBaron();

		bool IsPlant();
		bool IsPracticeToolDummy();
		bool IsClone();

		bool IsRecalling();
		bool IsTeleporting();

		ItemSlot* GetItemBySlot(int id);
		eSpellSlot* GetSpellSlotById(eSpellSlot id);

		bool operator==(GameObject* other);
		bool operator!=(GameObject* other);

		static bool ApplyHooks();

		static GameObject* LocalPlayer();
	};
}
}
