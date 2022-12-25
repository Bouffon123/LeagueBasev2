#pragma once

namespace Script {
namespace Native {
namespace Addresses {
	enum class r3dRenderer : unsigned int
	{
		r3dRendererInstance			= 0x31415B4,//12.13 // 8B 34 90 A1 ? ? ? ? 3B 86 ? ? ? ? 0F 8F ? ? ? ? 		---> mov     eax, dword_
		RenderPipelineLOLInstance	= 0x0, // ?

		WorldToScreen				= 0xA5C670, //12.13 // FF 74 24 3C E8 ? ? ? ? 83 C4 08 83 BB ? ? ? ? ?  -> premier call en dessous de (push    dword ptr [esp+3Ch])
		ProjectToScreen				= 0xA5AB80, //12.13 // 83 EC 0C F3 0F 10 44 24 30 8D 04 24 -> direct

		RenderUsualSuspects			= 0x39B190, //12.13 // E8 ? ? ? ? F3 0F 10 44 24 ? 83 C4 20 F3 0F 11 04 24    -> call
		RenderMouseOvers			= 0x39A080, //12.13 // E8 ? ? ? ? 8B 03 89 44 24 14   (RenderUsualSuspects call is an instruction into rendermouseovers function)

		DrawCircularRangeIndicator	= 0x0 // ?
	};

	enum class ObjectManager : unsigned int
	{
		ObjectManagerInstance = 0x1870BD8, //12.13 // 8B E9 8B 0D ? ? ? ? E8 ? ? ? ? 8B F0 (mov ecx, ObjectManagerInstance)

		GetFirstObject				= 0x269980, //12.13 // C1 0C E9 C8 -> direct
		GetNextObject				= 0x26B7E0, //12.13 // 8B 44 24 04 56 8B 71 18
	};

	enum class GameObjectAddresses : unsigned int
	{

		OnCreateObject				= 0x278180, //12.13// 50 E8 ? ? ? ? 0F B6 47 1D 
		OnDeleteObject				= 0x266B00, //12.13 // E8 ? ? ? ? 8B 4E 20 85 C9 74 0F

		OnProcessSpell				= 0x5071A0, //12.13 // 56 8B 74 24 08 8B 16 85 D2 75 07 8D 42 02 5E C2 04 00 (in the function)
		OnFinishCast				= 0x0, // [actual address in first opcode] E8 ? ? ? ? E9 ? ? ? ? C6 44 24 0C 00
		OnStopCast					= 0x0, // 8B 44 24 10 57 8B F9 3B 05 ? ? ? ?
		OnNewPath					= 0x0, // E8 ? ? ? ? 8B 74 24 4C 83 C4 30 8B CE

		GetAttackDelay				= 0x284540, //12.13 // [actual address in first opcode] E8 ? ? ? ? D9 9B D4 00 00 00
		GetAttackCastDelay			= 0x284440, //12.13 // [actual address in first opcode] E8 ? ? ? ? D9 9E ? ? ? ? 57
		GetBasicAttack				= 0x142200, //12.13 // E8 ? ? ? ? 83 38 00 74 4B
		IsInhib						= 0x177040, //12.13 // 0F 45 F1 E8 ? ? ? ? FF 37 84 C0 B9 ? ? ? ? 0F 45 F1 E8 ? ? ? ? FF 37 
		IsNexus						= 0x177060, //12.13 // E8 ? ? ? ? FF 37 84 C0 B9 ? ? ? ? 0F 45 F1 E8 ? ? ? ? 84 C0 
		CompareTypeFlags			= 0x14E920, //12.13 // E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 8A 43 DD

		GetHealthBarScreenPosition	= 0x5EEEE0, //12.13	!!! DETECTED !!! E8 ?? ?? ?? ?? 8B 4E ?? 8D 54 ?? ?? 52 8B 01 FF ?? ?? 5E 83 ?? ?? C3
		BaseDrawPosition			= 0x0, // [actual address in first opcode] E8 ? ? ? ? EB 07 8B 01 8B 40
		GetBoundingRadius			= 0x142A10, //12.13 // E8 ? ? ? ? D8 44 24 0C 8B 7C 24 18 

		IsAlive						= 0x16AC50, //12.13 // E8 ? ? ? ? 84 C0 74 3B 8B 86 ? ? ? ?
		IsMissile                   = 0x1770E0, //12.13 //E8 ? ? ? ? 8B 0F 8D 54 24 2C into pointed function
		IsTurret                    = 0x1771B0, //12.13 //E8 ? ? ? ? 8B 5C 24 38 8B E8
		ShouldDrawTurretRange		= 0x0, // ?
	};

	enum class ManagerList : unsigned int
	{
		MinionList					= 0x24BFAB4, //12.13 // 8B 0D ? ? ? ? E8 ? ? ? ? EB 09			--->				 mov     ecx, dword_
		HeroList					= 0x1870C68, //12.13 // 89 44 24 18 A1 ? ? ? ? 53 55						---> first address;  mov     eax, dword_
		TurretList					= 0x3105ACC, //12.13 // 8D 44 24 0C 89 35 ? ? ? ? ---> mov     dword_3325A48, esi
		MissileList					= 0x310DC0C, //12.13 // C7 83 ? ? ? ? ? ? ? ? 8B 15 ? ? ? ? 85 D2		---> first address;  mov     edx, dword_
		BuildingList				= 0x3105AD4, //12.13 // 8B 15 ? ? ? ? 8B 7A 04 8B 42 08 8D 0C 87 3B F9 73 34
		InhibitorList				= 0x310F688, //12.13 //C7 05 ? ? ? ? ? ? ? ? 74 0B 6A 10 56 E8 ? ? ? ? 83 C4 08 8B C6 5E C2 04 00 CC CC CC CC 56 57 (mov InhibitorsCollection, 0)
		AttackableUnitList			= 0x24BFA54, //12.13 //8B 0D ? ? ? ? 8A D8 56 (mov ecx, attackableUnitsCollection)
	};

	enum class LocalPlayer : unsigned int
	{
		LocalPlayerInstance			= 0x310ED68, //12.13 // A1 ? ? ? ? 8B 54 24 28

		IssueOrder					= 0x14F7E0, // C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 8B 44 24 20 C7 05 ? ? ? ? ? ? ? ? ---> first call under
		TryRightClick				= 0x607260, // [actual address in first opcode]E8 ? ? ? ? C2 04 00 CC CC CC CC CC 8B 44 24 04 6A 01
	};

	enum class Spellbook : unsigned int
	{
		CastSpell					= 0x503F00, //E8 ? ? ? ? 85 DB 74 2A 
		NewCastSpell				= 0x5F1DF0, //E8 ? ? ? ? 5E C6 87 ? ? ? ? ? 
		UpdateChargeableSpell		= 0x0, // ?
		GetSpellState				= 0x4EFA00, // 83 EC 14 8B 44 24 1C 55
	};

	enum class Game : unsigned int
	{
		GameVersion					= 0x3123020, //12.13 // 8B 44 24 04 BA ? ? ? ? 2B D0

		OnMainLoop					= 0x0, // 83 EC 1C 53 55 56 8B F1 57 8B AE 98 00 00 00 		// ObjManager class?
		OnWorldDraw					= 0x0, // F3 0F 10 6C 24 04 F3 0F 5C 6E 04		// hud class?

		GameTime					= 0x31081A8, //12.13 // 0F 57 C0 F3 0F 5C 0D ? ? ? ? 0F 2F C1			---> subss	 xmm1, dword_

		PlaySound					= 0x2017F0, //12.13 // 56 8B 74 24 14 83 7E 10 00
	};

	enum class NetClient : unsigned int
	{
		GetNetClientInstance		= 0x3116F58, //12.13 // A1 ? ? ? ? 88 4C 24 13
		GetPing						= 0x3317A0, //12.13 // E8 ? ? ? ? 8B E8 39 2B
	};

	enum class Minimap : unsigned int
	{
		MinimapInstance				= 0x31081E8, //8B 0D ? ? ? ? 85 C9 74 18 80 79 38 00  (mov ecx, minimapinstance)
	};

	enum class Logger : unsigned int
	{
		LoggerInstance				= 0x1122C0, //12.13 // 8D 44 24 14 50 FF 74 24 14 FF 74 24 14 FF 74 24 14 FF 74 24 14 E8 ?? ?? ?? ?? 83 C4 14 C3

		LogUiEvents					= 0x1195C0, //12.13 // 53 55 56 8B F1 57 8B 7C 24 18 8B 6E 14 3B FD 77 27 8B DE 83 FD 10 72 02 8B 1E 57 FF 74 24 18 89 7E 10 53 E8 ? ? ? ? 83 C4 0C C6 04 3B 00 8B C6 5F 5E 5D 5B C2 08 00
	};

	enum class Chat : unsigned int
	{
		ChatInstance				= 0x24BFC48, //12.13 // A3 ? ? ? ? E8 ? ? ? ? 6A 00 E8 ? ? ? ? ----> mov     dword ptr ChatInstance, eax ///////// FF 74 24 14 E8 ? ? ? ? 8D 4C 24 10 passe dans la fonction printchat en dur le chatinstance
		ChatClient					= 0x310F5E8, //12.13 // 8B 0D ? ? ? ? 8A D8 85 C9 -----> mov     ecx, ChatClient
		MenuGui						= 0x310f52c, //12.13  // 8B 0D ?? ?? ?? ?? 6A 00 FF 74 24 0C

		PrintChat					= 0x5D4EC0, //12.13 // FF 74 24 14 E8 ? ? ? ? 8D 4C 24 10 
		SendChat					= 0x63E840, //12.13 // E8 ? ? ? ? 8B E8 39 2B ----> OLD --> FF 74 24 0C E8 ? ? ? ? 8D 4C 24 08  ((void (__thiscall *)(int, char *, _DWORD))dword_A84F30[0])(dword_3557324, v2, 0);
	};

	enum class Hud : unsigned int
	{	
		HudInstance					= 0x1870C8C, //12.13 //8B ? ? ? ? ? 6A 00 8B ? ? E8 ? ? ? ? B0 01 ? ------> mov     ecx, offsetHUDINSTANCE
	};

	enum class NavGrid : unsigned int
	{
		NavGridInstance				= 0x9D93B0, //12.13 // 50 E8 ? ? ? ? F3 0F 10 0D ? ? ? ? 8D 44 24 30 
		IsNotWall					= 0x9E2240, //12.13 //E8 ? ? ? ? 83 C4 10 84 C0 75 39
		GetHeightForPosition		= 0x9DD730, //12.13 // E8 ? ? ? ? 8B 44 24 0C D9 18 
	};

	enum class PathControllerCommon : unsigned int
	{
		CalculatePath				= 0x0, // 83 EC 50 8B 44 24 58
	};
}
}
}