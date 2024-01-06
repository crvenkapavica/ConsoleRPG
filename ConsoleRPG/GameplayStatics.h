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
	static void Initialize(vector<shared_ptr<Character>>&& players, SpellManager& spell_manager, CombatManager& combat_manager, MapGenerator&& map_generator, ConsoleMenu& menu);

	static void DisplayAllies();
	static void DisplayEnemies();
	//static void DisplaySummons();

	static void DisplayMapMenuTitle();
	static void DisplayCombatMenuTitle();
	static void DisplaySpellMenuTitle();

	static int InteractiveDisplay(const vector<string>& options, const int right = 0, const bool bClear = true, const bool bIsItem = false);
	static void ANSI_CURSOR_DOWN_N(int n);

	static void DisplayMapMenu();
	static void HandleMapInput(int input);

	static PlayerCharacter* GetPlayer();

	static void DisplayItemMenu();
	//static void DisplaySomeSpellMenu();
	static void DisplayPlayerStats();

	static void InitiateCombatMode(vector<weak_ptr<Character>>&& enemies);
	static void ResetCombatVariables();

	static void GiveExperience(EnemyCharacter* enemy);

	static int DisplayCombatMenu(Character* character);
	static void HandleCombatInput(PlayerCharacter* character, int input);
	static void HandleCombatInput(SummonCharacter* character, int input);
	
	static void CombatMove();
	static void EnemyCombatMove(Character* character, OUT map<int, EDirection>& direction_map);
	static void MoveCharacterOnGrid(Character* character, EDirection direction);

	static int GetPlayerIdx(char c);
	static int GetEnemyIdx(char c);

	static int GetSpellIdx(ActiveSpell* spell, shared_ptr<Character>& character);

	static void DisplayMeleeMenu();
	static void DisplayRangedMenu();
	static void DisplaySpellMenu();

	static void HandleTarget(ActiveSpell* spell);
	static void HandleMeleeTarget(ActiveSpell* spell);

	static void DisplayInfoMenu();
	static void HandleInfoInput(int input);
	//static void HandleEffectInfo(int spell_idx, ESpellType spell_type, int effect_idx);

	static void RedrawGameScreen();

	static stringstream& GetCombatLogStream() { return _combat_log; }
	static void DisplayCombatLog();
	static void ExtractLinesFromStringstream(OUT std::vector<string>& lines, const int max_lines, stringstream& ss, OUT int& start_index);

	static std::weak_ptr<Character> GetWeakCharacter(Character* character);
	static std::shared_ptr<Character> GetSharedCharacter(Character* character);

	static std::vector<weak_ptr<Character>> GetPlayerCharacters();
	static std::vector<weak_ptr<Character>> GetEnemyCharacters();

	static float ApplyDamage(std::weak_ptr<Character> instigator, Character* target, float damage, std::unique_ptr<ActiveSpell>& spell, bool isOnApply);
	static void ApplyEffect(std::shared_ptr<Character>& instigator, std::vector<weak_ptr<Character>>&targets, std::unique_ptr<ActiveSpell> spell, std::optional<ApplyParams> apply_params, std::optional<EffectParams> effect_params);

	static void KillEnemy(int idx);

	static void EndTurn(Character* character);

	static bool AddCharacterToCharGrid(const shared_ptr<Character>& instigator, std::weak_ptr<Character> summon);

	static void RollLoot();
	
	static void DisplayLoot(weak_ptr<PlayerCharacter> character, std::vector<std::unique_ptr<Item>> loot);

	static string GetAliasColor(char alias);

	static string string2(float f);
	static float float2(float f);

	static int GetRandInt(int a, int b);
	static float GetRandFloat(float a, float b);

public:
	static std::string GetEnumString(ESpellID _enum);
	static std::string GetEnumString(ESpellType _enum);
	static std::string GetEnumString(ESpellActivity _enum);
	static std::string GetEnumString(EDamageType _enum);
	static std::string GetTypeString(int _enum);
	static std::string GetEnumString(ECharacterClass _enum);
	static std::string GetEnumString(EItemSlot _enum);
	static std::string GetEnumString(EWeaponType _enum);
	static std::string GetEnumString(EItemRarity _enum);
	static std::string GetEnumString(EItemType _enum);

private:
	GameplayStatics() = delete;

private:
	static weak_ptr<Character>				_player;
	static vector<shared_ptr<Character>>	_player_characters;
	static SpellManager*	_sm;
	static CombatManager*	_cm;
	static MapGenerator*	_map_gen;
	static ConsoleMenu*		_menu;

	static vector<weak_ptr<Character>>		_enemies;
	static vector<weak_ptr<Character>>		_players;
	static stringstream						_combat_log;
};