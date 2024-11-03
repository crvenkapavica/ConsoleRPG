#pragma once

#include "../GameplayStatics.h"
#include "../RPGTypes.h"
#include "../Characters/EnemyCharacter.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/SummonCharacter.h"
#include "../Spells/EffectStructs.h"

class Character;
class SpellManager;

class CombatManager final {

public:
	CombatManager() = default;
	CombatManager(const CombatManager&) = delete;
	CombatManager(CombatManager&&) = delete;
	~CombatManager() = default;
	
	CombatManager& operator=(const CombatManager&) = delete;
	CombatManager& operator=(CombatManager&&) = delete;
	
	static CombatManager& GetInstance();

	// Set turns for each side and add them to the turn table
	void SetTurns(const std::vector<std::weak_ptr<Character>>& Team1, const std::vector<std::weak_ptr<Character>>& Team2);

	void StartCombat(const std::weak_ptr<Character>& player);
	
	// Add the effect of the spell to the effect pool until it expires
	void AddCombatEffect(const std::shared_ptr<CombatEffect>& Effect);

	void BeginTurn(const std::weak_ptr<Character>& InCharacter);
	void EndTurn(Character* InCharacter);
	
	// Add a summoned unit (from spell or item) to the combat and turn order
	void AddSummonToCombat(std::shared_ptr<SummonCharacter> Summon);

	// Gets the turn table aliases for printing turn order
	void DisplayTurnOrder() const;

	// Gets the Character currently on turn
	inline std::weak_ptr<Character> GetTurnCharacter() const { return TurnTable[TurnIndex]; }

	// Gets the alias of the character currently on turn
	inline char GetTurnAlias() const { return TurnTable[TurnIndex].lock()->GetAlias(); }
	inline int GetTurn() const { return Turn; }

	inline std::vector<std::weak_ptr<Character>> GetPlayers() const { return Players; }
	inline std::vector<std::weak_ptr<Character>> GetEnemies() const { return Enemies; }
	inline std::vector<std::shared_ptr<Character>> GetSummons() const { return Summons; }

	// Flags all characters with HP < 0 with bIsAlive = false
	void FlagDeadCharacters();
	// Reset the pointer pointing to a dead character
	void KillFlaggedCharacters();
	// Remove dead characters from the turn table
	void RemoveDeadCharacters();
	
	void ExitCombatMode();
	void ResetCombatVariables();

	//===== PUBLIC EVENTS ===== //
	//////////////////////////////////////////////////////////////////////////////////

	static void OnMagicBegin(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets);
	static void OnMagicEnd(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets);

	static void OnMagicReceivedBegin(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator);
	static void OnMagicReceivedEnd(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator);

	static void OnMeleeBegin(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets);
	static void OnMeleeEnd(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets);

	static void OnMeleeReceivedBegin(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator);
	static void OnMeleeReceivedEnd(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator);

	static void OnRangedBegin(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets);
	static void OnRangedEnd(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets);

	static void OnRangedReceivedBegin(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator);
	static void OnRangedReceivedEnd(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator);

	//////////////////////////////////////////////////////////////////////////////////
private:
	// Apply the value of the effect to the related targets' stat
	void ApplyStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target, const CharacterStat& CharStat, INOUT float& Total, bool bIsOnApply);

	void HandleCombatEffect(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target);

	// Calls Apply Stat for each character passed in the effect on effect application
	void HandleApplyStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target);

	// Calls Apply Stat for each character passed in the effect with per tick value
	void HandleEffectStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target);

	// Gets the characters' base for resetting values after each turn
	void GetCharactersBase();

	// Resets the values so they are not re-applied if they're expired
	void ResetCharacterValues() const;

	void RemoveExpiredCombatEffects();

	void ApplyEffectsOnEvent(const ECombatEvent OnEvent);

	// Called when the passive effect is being instigated, which means it's the instigators turn
	static void InstigatePassiveEffects(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets, const ECombatEvent OnEvent);

	// Called when the character with the passive was being instigated on
	static void TriggerPassiveEffects(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator, const ECombatEvent OnEvent);

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
	std::weak_ptr<Character> Player;
	std::vector<std::weak_ptr<Character>> Players;
	std::vector<PlayerCharacter> PlayersBase;
	std::vector<std::weak_ptr<Character>> Enemies;
	std::vector<EnemyCharacter> EnemiesBase;
	std::vector<std::shared_ptr<Character>> Summons;
	std::vector<SummonCharacter> SummonsBase;

	std::vector<std::pair<int, std::shared_ptr<CombatEffect>>> CombatEffects;
	std::vector<std::weak_ptr<Character>> TurnTable;

	int TurnIndex = 0;
	int Turn = 0;
	bool bNext = false;
};