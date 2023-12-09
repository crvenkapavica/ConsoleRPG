#pragma once
#include "../GameplayStatics.h"
#include "../RPGTypes.h"
#include "../Effects/EffectStructs.h"
#include "../Characters/PlayerCharacter.h"


class Character;
class SpellManager;

class CombatManager {

private:

	CombatManager() {}
	CombatManager(const CombatManager&) = delete;
	CombatManager& operator=(const CombatManager&) = delete;

	vector<weak_ptr<PlayerCharacter>> _player_characters;
	vector<PlayerCharacter> _player_characters_base;

	vector<weak_ptr<EnemyCharacter>> _enemy_characters;
	vector<EnemyCharacter> _enemy_characters_base;

	vector<pair<int, unique_ptr<CombatEffect>>> _combat_effects;

	vector<weak_ptr<Character>>	_turn_table;

	int _turn_index = 0;

	int _turn = 0;

	bool bDeadOnTurn = false;

public:

	static CombatManager& GetInstance();

	void SetTurns(vector<weak_ptr<PlayerCharacter>> characters_1, vector<weak_ptr<EnemyCharacter>> characters_2);

	void StartCombat(weak_ptr<PlayerCharacter> player);

	void AddCombatEffect(unique_ptr<CombatEffect> effect);

	void BeginTurn(Character* character);

	void EndTurn(Character* character);

	// Gets the turn table aliases for printing turn order
	void DisplayTurnOrder();

	// Gets the Character currently on turn
	inline weak_ptr<Character> GetTurnCharacter() { return _turn_table[_turn_index]; }

	// Gets the alias of the character currently on turn
	inline char GetTurnAlias() { return _turn_table[_turn_index].lock()->GetAlias(); }

	void DestroyDeadCharacters();

	int GetDeadIdx();

	void RemoveDeadCharacters();

	void ExitCombatMode();

	void ResetCombatVariables();


	void OnCastBegin(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<weak_ptr<Character>> targets);
	void OnCastEnd(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<weak_ptr<Character>> targets);

private:

	//GetParamsEffectStruct()

	void ApplyStat(CombatEffect* effect, CharacterStat& character_stat, EStatValueAction value_action, float& _total, bool isOnApply);

	void ApplyRes(CombatEffect* effect, CharacterStat& character_res, float& _total);

	void HandleCombatEffect(CombatEffect* effect, Character* target = nullptr);

	void HandleApplyStat(CombatEffect* effect, Character* target);

	void HandleEffectStat(CombatEffect* effect, Character* target);

	void HandleApplyRes(CombatEffect* effect, Character* target);

	void HandleEffectRes(CombatEffect* effect, Character* target);

	void GetCharactersBase();

	void ResetCharacterValues();

	void RemoveExpiredCombatEffects();

	void ApplyEffectsOnEvent(ECombatEvent on_event);

	void ApplyPassiveEffects(ECombatEvent on_event, Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<weak_ptr<Character>> targets);

	//=====  EVENTS ===== //
	///////////////////////
	void OnApplyEffect();
	void OnCombatBegin();
	void OnCombatEnd();
	void OnTurnBegin();
	void OnTurnEnd();
	void OnCycleBegin();
	void OnCycleEnd();
};