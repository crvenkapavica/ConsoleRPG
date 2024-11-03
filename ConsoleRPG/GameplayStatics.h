#pragma once

#include "RPGTypes.h"
#include "Display/ConsoleMenu.h"

class Character;
class PlayerCharacter;
class EnemyCharacter;
class SummonCharacter;
class SpellManager;
class CombatManager;
class MapGenerator;
class ActiveSpell;
class Item;
struct EffectParams;
struct ApplyParams;

class GameplayStatics {

public:
	GameplayStatics() = delete;
	
	static void Initialize(std::vector<std::shared_ptr<Character>>&& InPlayerCharacters, ConsoleMenu& Menu);

	static void DisplayAllies();
	static void DisplayEnemies();
	//static void DisplaySummons();

	static void DisplayMapMenuTitle();
	static void DisplayCombatMenuTitle();
	static void DisplaySpellMenuTitle();

	static int InteractiveDisplay(const std::vector<std::string>& Options, const int Right = 0, const bool bClear = true, const std::vector<Item*>& Items = std::vector<Item*>());
	static void ANSI_CURSOR_DOWN_N(int nDown);

	static void DisplayMapMenu();
	static void HandleMapInput(int Input);

	static PlayerCharacter* GetTurnCharacter();

	static void DisplayItemMenu();
	//static void DisplaySomeSpellMenu();
	static void DisplayPlayerStats();

	static void InitiateCombatMode(std::vector<std::weak_ptr<Character>>&& enemies);
	static void ResetCombatVariables();

	static void GiveExperience(EnemyCharacter* enemy);

	static int DisplayCombatMenu(Character* character);
	static void HandleCombatInput(PlayerCharacter* character, int Input);
	static void HandleCombatInput(SummonCharacter* character, int Input);
	
	static void CombatMove();
	static void EnemyCombatMove(Character* Enemy, OUT std::map<int, EDirection>& DirectionMap) {}
	static void MoveCharacterOnGrid(const Character& InCharacter, const EDirection Direction);
	 
	static int GetPlayerIdx(char c);
	static int GetEnemyIdx(char c);

	static int GetSpellIndex(ActiveSpell* spell, std::shared_ptr<Character>& character);

	static void DisplayMeleeMenu();
	static void DisplayRangedMenu();
	static void DisplaySpellMenu();

	static void HandleTarget(const ActiveSpell* TargetSpell);
	static void HandleMeleeTarget(ActiveSpell* spell);

	static void DisplayInfoMenu();
	static void HandleInfoInput(int input);
	//static void HandleEffectInfo(int spell_idx, ESpellType spell_type, int effect_idx);

	static void RedrawGameScreen();

	static std::stringstream& GetCombatLogStream() { return CombatLog; }
	static void DisplayCombatLog();
	static void ExtractLinesFromStringStream(OUT std::vector<std::string>& Lines, const int MaxLines, std::stringstream& Buffer, OUT int& StartIndex);

	static std::weak_ptr<Character> GetWeakCharacter(const Character& InCharacter);
	static std::shared_ptr<Character> GetSharedCharacter(const Character& InCharacter);

	static std::vector<std::weak_ptr<Character>> GetPlayerCharacters();
	static std::vector<std::weak_ptr<Character>> GetEnemyCharacters();

	static float ApplyDamage(const std::weak_ptr<Character>& Instigator, Character* Target, float Damage, const std::unique_ptr<ActiveSpell>& Spell, bool bIsOnApply);
	static void ApplyEffect(std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets, std::unique_ptr<ActiveSpell> Spell,
							const std::optional<ApplyParams>& ApplyParams, const std::optional<EffectParams>& EffectParams);

	static void KillEnemy(int Idx);

	static void EndTurn(Character* character);

	static bool AddCharacterToCharGrid(const std::shared_ptr<Character>& Instigator, const std::weak_ptr<Character>& Summon);

	static void RollLoot();
	
	static void DisplayLoot(const std::weak_ptr<PlayerCharacter>& Character, std::vector<std::unique_ptr<Item>> Loot);

	static std::string GetAliasColor(char Alias);

	static std::string String2(float F);
	static float Float2(float F);

	static int GetRandInt(int A, int B);
	static float GetRandFloat(float A, float B);

public:
	static std::string GetEnumString(ESpellID Enum);
	static std::string GetEnumString(ESpellType Enum);
	static std::string GetEnumString(ESpellActivity Enum);
	static std::string GetEnumString(EDamageType Enum);
	static std::string GetTypeString(int Enum);
	static std::string GetEnumString(ECharacterClass Enum);
	static std::string GetEnumString(EItemSlot Enum);
	static std::string GetEnumString(EWeaponType Enum);
	static std::string GetEnumString(EItemRarity Enum);
	static std::string GetEnumString(EItemType Enum);

private:
	static std::weak_ptr<Character>					PlayerAvatar;
	static std::vector<std::shared_ptr<Character>>	PlayerCharacters;
	static std::stringstream						CombatLog;
	static ConsoleMenu*								DisplayMenu;
};