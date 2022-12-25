#pragma once

namespace Script {
namespace Native {
namespace Structures {
	enum class Chat : unsigned int // TO CHECK
	{
		isOpen = 0x754,
	};

	enum class r3dRenderer : unsigned int // TO CHECK
	{
		dxVersion = 0x4,				// 3 = dx11, 0 = dx9
		rendererSize = 0xC,
		viewMatrix = 0x54,
		projectionMatrix = viewMatrix + 0x40,
	};

	enum class RenderPipelineLOL : unsigned int // TO CHECK
	{
		screenBuffer = 0x0,
		defaultMouseOverEffectData = 0x17C,
	};

	enum class MouseOverEffectData : unsigned int // TO CHECK
	{
		color = 0x0,
		size = 0x34,
		blur = 0x3C,
	};

	enum class Hud : unsigned int // TO CHECK
	{
		cursor = 0x14,
		isWindowFocused = 0x64, // yes = 1; no = 257
	};

	enum class Cursor : unsigned int // TO CHECK
	{
		position = 0x1C,
		lastClickPosition = 0x28,
		hoveredUnitId = 0x3C,
	};

	enum class Minimap : unsigned int // TO CHECK
	{
		minimapHud = 0x120,
		position = 0x44,
		size = 0x4C,
	};

	enum class GameObjectVTable : unsigned int {
		GetBoundingRadius = 34,
		IsVisible = 36,
		IsZombie = 60,
		IsTargetableToAllies = 100,
		IsTargetableToEnemies = 104,
		IsHpBarVisible = 124,				// TOCHECK
		GetPathControllerCommon = 150,		
		GetRespawnTime = 221,				
		GetRespawnTimeRemaining = 222,
		IsWard = 288,						// TOCHECK
		IsPet = 289,						// TOCHECK


		IsTargetable = 0x0, // ??
		IsTargetableToTeamFlags = 0x0, // ??
		GetGoldGivenOnDeath = 0x0, // ??
		GetGoldGivenOnDeathPreKill = 0x0, // ??
	};

	enum class GameObject : unsigned int
	{
		// TODO obfuscated class members : obj flag, id, healthbar pos etc

		BuffManager = 0x2178,			//12.12 ?

		index = 0x8,					//12.13
		team = 0x34,					//12.13
		name = 0x54,					//12.13
		networkId = 0xB4,				//12.13
		position = 0x1DC,				//12.13
		direction = 0x1BB0,				//12.13
		currentGolds = 0x1B58,			//12.13
		missileClient = 0x258,			//12.12	?
		visibility = 0x274,				//12.12 ?
		srcIndex = 0x2C4,				//12.12 ?
		mana = 0x29C,					//12.13
		maxMana = 0x2AC,				//12.13
		recallState = 0xD90,			//12.13
		health = 0xE74,					//12.13
		maxHealth = 0xE84,				//12.13
		abilityHaste = 0x1690,			//12.13
		lethality = 0x1288,				//12.13
		armor = 0x1374,					//12.13
		bonusArmor = 0x1378,			//12.13
		magicRes = 0x137C,				//12.13
		bonusMagicRes = 0x1380,			//12.13
		baseAD = 0x134C,				//12.13
		bonusAD = 0x12C4,				//12.13
		movementSpeed = 0x138C,			//12.13
		combatType = 0x20A8,			//12.13
		SpellBookInstance = 0x2330,		//12.12 ?
		championName = 0x2BA4,			//12.13
		level = 0x3384,					//12.13
		critDamageMultiplier = 0x1360,	//12.13
		crit = 0x1370,					//12.13
		baseAP = 0x1740,				//12.13
		attackSpeedMultiplier = 0x1348,	//12.13
		attackRange = 0x1394,			//12.13
		isTargetable = 0xD04,			//12.13
		isInvulnerable = 0x3D4,			//12.13
		levelUpPoints = 0x33AC,			//12.13
		itemList = 0x33D0,				//12.12 ?
		cumulatedExperience = 0x3374,	//12.12
		magicPenFlat = 0x126C,			//12.13
		armorPenMultiplier = 0x1274,	//12.13
		magicPenMultiplier = 0x1270,	//12.13
		bonusAP = 0x12D4,			//12.13
		bonusAPMultiplier = 0x12D8,//12.13
		manaRegen = 0x11E0,				//12.13
		healthRegen = 0x1388,			//12.13
	};

	enum class MissileClient : unsigned int	// TO CHECK
	{
		spellInfo = 0x8,

		slot = 0x10,
		missileIndex = 0x14,
		casterName = 0x20,
		casterIndex = 0x6C,
		targetIndex = 0xC0,

		startPosition = 0x84,
		endPosition = 0x90,
		castPosition = 0x9C,

		windupTime = 0x4C0,
		cooldown = 0x4D4,
		manaCost = 0x4F0,

		castStartTime = 0x544,
		castEndTime = 0x548,
	};

	enum class PathControllerCommon : unsigned int
	{
		navigationMesh = 0x1B0,
		isMoving = 0x1C0,
		navigationPath = 0x1C4,
		isDashing = 0x214,
		serverPos = 0x2E4,
	};

	enum class NavigationPath : unsigned int
	{
		currSegment = 0x0,
		navStart = 0x8,
		navEnd = 0x14,
		navArray = 0x20,
		navArrayEnd = 0x24
	};

	enum class SpellBook : unsigned int //ok
	{
		activeSpell = 0x20,
		firstSlot = 0x488,			
	};

	enum class SpellBookVTable : unsigned int		// TO CHECK
	{
		GetSpellState			= 1,
		GetSpellDataInstance	= 13
	};

	enum class ActiveSpell : unsigned int //ok
	{
		srcIndex = 0x6C,
		targetsArray = 0xC0,
		targetsArraySize = 0xC4,

		spellInfo = 0x8,

		startPos = 0x84,
		endPos = 0x90,

		
		isSpell = 0xE8,
		isAuto = 0xEC,

		slot = 0xF4,
	};

	// struct passed to OnProcessSpell only
	enum class ProcessedSpell : unsigned int
	{
		spellInfo = 0x0,

		srcIndex = 0x64,
		targetsArray = 0xB8,
		targetsArraySize = 0xC0,

		startPos = 0x7C,
		endPos = 0x88,

		
		isSpell = 0xE0,
		isAuto = 0xE4,

		slot = 0xEC,
	};

	enum class SpellSlot : unsigned int //ok
	{
		chargeTime = 0x74,
		level = 0x1C,
		readyAt = 0x24,
		charges = 0x54,
		damage = 0x94,

		targetClient = 0x11C,
		spellInfo = 0x120,
	};

	enum class SpellInfo : unsigned int //ok
	{
		spellName = 0x18,

		spellData = 0x40,
	};

	enum class SpellData : unsigned int //ok
	{
		name = 0x6C,
		missileSpeed = 0x460,
	};

	enum class ItemSlot : unsigned int // TO CHECK
	{
		itemInfo = 0xC,
	};

	enum class ItemInfo : unsigned int // TO CHECK
	{
		itemData = 0x20,

		ammo = 0x24,
		price = 0x28,
	};

	enum class ItemData : unsigned int // TO CHECK
	{
		itemId = 0x68,
		abilityHaste = 0xF8,
		health = 0xFC,
		movementSpeed = 0x128,
		armor = 0x134,
		magicResistance = 0x154,
		ad = 0x170,
		ap = 0x178,
		attackSpeed = 0x1A4,
	};

	enum class BuffManager : unsigned int //ok
	{
		buffEntriesArray = 0x10,
		buffEntriesArrayEnd = 0x14,
	};

	enum class BuffEntry : unsigned int //ok
	{
		BuffData = 0x8,

		type = 0x4,
		startTime = 0xC,
		endTime = 0x10,
		countAlt = 0x24,
		countInt = 0x74,
		countFloat = 0x130,
	};

	enum class BuffData : unsigned int //ok
	{
		name = 0x4,
	};

	enum class NavGrid : unsigned int
	{
		grid = 0x4,
	};
			
	enum class TargetClient : unsigned int //ok
	{
		CasterGameObjectIndex = 0x8,
		TargetGameObjectIndex = 0xC,
		SpellStartPosition = 0x10,
		TargetPosition = 0x1C,
		CursorWorldPosition = 0x28,
	};
}
}
}