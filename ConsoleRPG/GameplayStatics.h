#pragma once

#include "RPGTypes.h"
#include "Display/ConsoleMenu.h"

class Character;
class PlayerCharacter;
class EnemyCharacter;
class SpellBook;
class SpellManager;
class CombatManager;
class MapGenerator;
class ActiveSpell;
class Item;
struct EffectParams;
struct ApplyParams;

class GameplayStatics {

public:

	static void Initialize(vector<shared_ptr<PlayerCharacter>> players, SpellManager* spell_manager, CombatManager* combat_manager, MapGenerator* map_generator, ConsoleMenu* menu);

	static void DisplayAllies();
	static void DisplayEnemies();

	static void DisplayMapMenuTitle();
	static void DisplayCombatMenuTitle();
	static void DisplaySpellMenuTitle();

	static int InteractiveDisplay(const vector<string>& options, const int right = 0, const bool clear = true);
	static void ANSI_CURSOR_DOWN_N(int n);

	static void DisplayMapMenu();
	static void HandleMapInput(int input);

	static PlayerCharacter* GetPlayer();

	static void DisplayItemMenu();
	//static void DisplaySomeSpellMenu();
	static void DisplayPlayerStats();

	static void InitiateCombatMode(vector<weak_ptr<EnemyCharacter>> enemies);
	static void ExitCombatMode();
	static void ResetCombatVariables();

	static void GiveExperience(EnemyCharacter* enemy);

	static int DisplayCombatMenu();
	static void HandleCombatInput(PlayerCharacter* character, int input);
	
	static void CombatMove();
	static void EnemyCombatMove(Character* character, OUT map<int, EDirection>& direction_map);
	static void MoveCharacterOnGrid(Character* character, EDirection direction);

	static int GetPlayerIdx(char c);
	static int GetEnemyIdx(char c);

	static void DisplayMeleeMenu();
	static void DisplayRangedMenu();
	static void DisplaySpellMenu();

	static void HandleTarget(ActiveSpell* spell);

	static void DisplayInfoMenu();
	static void HandleInfoInput(int input);
	//static void HandleEffectInfo(int spell_idx, ESpellType spell_type, int effect_idx);

	static void RedrawGameScreen();

	static stringstream& GetCombatLogStream() { return _combat_log; }
	static void DisplayCombatLog();
	static void MoveCursorToCombatLog();
	static void ExtractLinesFromStringtream(OUT vector<string>& lines, const int max_lines, stringstream& ss, OUT int& start_index);

	static vector<weak_ptr<Character>> GetPlayerCharacters();
	static vector<weak_ptr<Character>> GetEnemyCharacters();

	static float ApplyDamage(Character* instigator, Character* target, float damage, unique_ptr<ActiveSpell>& spell, bool isOnApply);
	static void ApplyEffect(Character* instigator, vector<weak_ptr<Character>>& targets, unique_ptr<ActiveSpell> spell, ApplyParams& apply_params, EffectParams& effect_params);

	static void KillEnemy(int idx);

	static void EndTurn(Character* character);

	static string GetAliasColor(char alias);

	static string string2(float f);
	static float float2(float f);

	static int GetRandInt(int a, int b);
	static float GetRandFloat(float a, float b);

private:
	static weak_ptr<PlayerCharacter>	_player;
	static vector<PlayerCharacter*>		_player_characters;
	static vector<EnemyCharacter*>		_enemy_characters;
	static SpellManager*				_sm;
	static CombatManager*				_cm;
	static MapGenerator*				_map_gen;
	static ConsoleMenu*					_menu;

	static vector<weak_ptr<EnemyCharacter>>		_enemies;
	static vector<weak_ptr<PlayerCharacter>>	_players;
	static stringstream							_combat_log;

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
};