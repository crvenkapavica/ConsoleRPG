#pragma once
#include "../GameplayStatics.h"
#include "../RPGTypes.h"
#include "../Spells/EffectStructs.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Characters/SummonCharacter.h"

class Character;
class SpellManager;

class CombatManager {

public:
	static CombatManager& GetInstance();

	// Set turns for each side and add them to the turn table
	void SetTurns(vector<weak_ptr<PlayerCharacter>> characters_1, vector<weak_ptr<EnemyCharacter>> characters_2);

	void StartCombat(weak_ptr<PlayerCharacter> player);
	
	// Add the effect of the spell to the effect pool until it expires
	void AddCombatEffect(unique_ptr<CombatEffect> effect);

	void BeginTurn(weak_ptr<Character> character);
	void EndTurn(Character* character);
	
	// Add a summoned unit (from spell or item) to the combat and turn order
	void AddSummonToCombat(shared_ptr<SummonCharacter> character);

	// Gets the turn table aliases for printing turn order
	void DisplayTurnOrder();

	// Gets the Character currently on turn
	inline const weak_ptr<Character> GetTurnCharacter() const { return _turn_table[_turn_index]; }

	// Gets the alias of the character currently on turn
	inline char GetTurnAlias() const { return _turn_table[_turn_index].lock()->GetAlias(); }

	inline const int GetTurn() const { return _turn; }

	inline const vector<weak_ptr<Character>> GetPlayers() const { return _players; }

	inline const vector<weak_ptr<Character>> GetEnemies() const { return _enemies; }

	inline const vector<shared_ptr<Character>> GetSummons() const { return _summons; }

	// Flaggs all characters with HP < 0 with bIsAlive = false
	void FlagDeadCharacters();

	// Reset the pointer pointing to a dead character
	void KillFlaggedCharacters();

	// Remove dead characters from the turn table
	void RemoveDeadCharacters();

	void ExitCombatMode();

	void ResetCombatVariables();

	//===== PUBLIC EVENTS ===== //
	//////////////////////////////////////////////////////////////////////////////////

	void OnMagicBegin(Character* instigator, vector<weak_ptr<Character>> targets);
	void OnMagicEnd(Character* instigator, vector<weak_ptr<Character>> targets);

	void OnMagicReceivedBegin(Character* instigator, Character* target);
	void OnMagicReceivedEnd(Character* instigator, Character* target);

	void OnMeleeBegin(Character* instigator, vector<weak_ptr<Character>> targets);
	void OnMeleeEnd(Character* instigator, vector<weak_ptr<Character>> targets);

	void OnMeleeReceivedBegin(Character* instigator, Character* target);
	void OnMeleeReceivedEnd(Character* instigator, Character* target);

	void OnRangedBegin(Character* instigator, vector<weak_ptr<Character>> targets);
	void OnRangedEnd(Character* instigator, vector<weak_ptr<Character>> targets);

	void OnRangedReceivedBegin(Character* instigator, Character* target);
	void OnRangedReceivedEnd(Character* instigator, Character* target);

	//////////////////////////////////////////////////////////////////////////////////
private:
	// Apply the value of the effect to the related targets' stat
	void ApplyStat(CombatEffect* effect, Character* target, CharacterStat& character_stat, float& _total, bool isOnApply);

	void HandleCombatEffect(CombatEffect* effect, Character* target = nullptr);

	// Calls Apply Stat for each character passed in the effect on effect application
	void HandleApplyStat(CombatEffect* effect, Character* target);

	// Calls Apply Stat for each character passed in the effect with per tick value
	void HandleEffectStat(CombatEffect* effect, Character* target);

	// Gets the characters' base for resetting values after each turn
	void GetCharactersBase();

	// Resets the values so they are not re-applied if they're expired
	void ResetCharacterValues();

	void RemoveExpiredCombatEffects();

	void ApplyEffectsOnEvent(ECombatEvent on_event);

	// Called when the passive effect is being instigated, which means its the instigators turn
	void InstigatePassiveEffects(Character* instigator, vector<weak_ptr<Character>> targets, ECombatEvent on_event);

	// Called when the character with the passive was being instigated on
	void TriggerPassiveEffects(Character* character, Character* instigator, ECombatEvent on_event);

	//=====  EVENTS ===== //
	///////////////////////

	void OnApplyEffect();

	void OnCombatBegin();

	void OnCombatEnd();

	void OnTurnBegin();

	void OnTurnEnd();

	void OnCycleBegin();

	void OnCycleEnd();

	//////////////////////

private:
	CombatManager() {}
	CombatManager(const CombatManager&) = delete;
	CombatManager& operator=(const CombatManager&) = delete;

	weak_ptr<Character> _player;

	vector<weak_ptr<Character>> _players;
	vector<PlayerCharacter> _players_base;

	vector<weak_ptr<Character>> _enemies;
	vector<EnemyCharacter> _enemies_base;

	vector<shared_ptr<Character>> _summons;
	vector<SummonCharacter> _summons_base;

	vector<pair<int, unique_ptr<CombatEffect>>> _combat_effects;

	vector<weak_ptr<Character>>	_turn_table;

	int _turn_index = 0;

	int _turn = 0;

	bool bNext = false;
};