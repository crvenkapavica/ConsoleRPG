#pragma once

#include "../RPGTypes.h"
#include "../Characters/Character.h"
#include "../Characters/EnemyCharacter.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/SummonCharacter.h"
#include "../GameplayStatics.h"
#include "../Spells/EffectStructs.h"

class CombatManager final {

public:
	CombatManager() = delete;
	CombatManager(const CombatManager&) = delete;
	CombatManager(CombatManager&&) = delete;
	~CombatManager() = delete;
	
	CombatManager& operator=(const CombatManager&) = delete;
	CombatManager& operator=(CombatManager&&) = delete;
	
	// Set turns for each side and add them to the turn table
	static void SetTurns(std::vector<std::weak_ptr<Character>>&& Team1, std::vector<std::weak_ptr<Character>>&& Team2);

	static void StartCombat(const std::weak_ptr<Character>& Avatar);
	
	// Add the effect of the spell to the effect pool until it expires
	static void AddCombatEffect(const std::shared_ptr<CombatEffect>& Effect);

	static void BeginTurn(const std::weak_ptr<Character>& InCharacter);
	static void EndTurn(Character& InCharacter);
	
	// Add a summoned unit (from spell or item) to the combat and turn order
	static void AddSummonToCombat(const std::shared_ptr<SummonCharacter>& Summon);

	// Gets the turn table aliases for printing turn order
	static void DisplayTurnOrder();

	// Gets the Character currently on turn
	static inline std::weak_ptr<Character> GetTurnCharacter() { return TurnTable[nTurn]; }

	// Gets the alias of the character currently on turn
	static inline char GetTurnAlias() { return TurnTable[nTurn].lock()->GetAlias(); }
	static inline int GetTurn() { return nCycle; }
	static inline bool IsCombatFinished() { return bCombatFinished; }  //TODO: Rename??

	static inline std::vector<std::weak_ptr<Character>>& GetPlayers() { return PlayerCharacters; }
	static inline std::vector<std::weak_ptr<Character>>& GetEnemies() { return EnemyCharacters; }
	static inline std::vector<std::shared_ptr<Character>>& GetSummons() { return SummonCharacters; }

	// TODO: MOVE TO PRIVATE?
	//////////////////////////////////////////////////////////////
	// Flags all characters with HP < 0 with bIsAlive = false
	static void FlagDeadCharacters();
	// Reset the pointer pointing to a dead character
	static void KillFlaggedCharacters();
	// Remove dead characters from the turn table
	static void RemoveDeadCharacters();
	
	static void ExitCombatMode();
	static void ResetCombatVariables();
	//////////////////////////////////////////////////////////////
	
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
	static void ApplyStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target, const CharacterStat& CharStat, INOUT float& Total, bool bIsOnApply);

	static void HandleCombatEffect(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target);

	// Calls Apply Stat for each character passed in the effect on effect application
	static void HandleApplyStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target);

	// Calls Apply Stat for each character passed in the effect with per tick value
	static void HandleEffectStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target);

	// Gets the characters' base for resetting values after each turn
	static void GetCharactersBase();

	// Resets the values so they are not re-applied if they're expired
	static void ResetCharacterValues();

	static void RemoveExpiredCombatEffects();

	static void ApplyEffectsOnEvent(const ECombatEvent OnEvent);

	// Called when the passive effect is being instigated, which means it's the instigators turn
	static void InstigatePassiveEffects(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets, const ECombatEvent OnEvent);

	// Called when the character with the passive was being instigated on
	static void TriggerPassiveEffects(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator, const ECombatEvent OnEvent);

	//=====  EVENTS ===== //
	///////////////////////

	static void OnApplyEffect();
	static void OnCombatBegin();
	static void OnCombatEnd();
	static void OnTurnBegin();
	static void OnTurnEnd();
	static void OnCycleBegin();
	static void OnCycleEnd();
	//////////////////////

private:
	static std::weak_ptr<Character> PlayerAvatar;
	static std::vector<std::weak_ptr<Character>> PlayerCharacters;
	static std::vector<PlayerCharacter*> PlayerCharactersBase;
	static std::vector<std::weak_ptr<Character>> EnemyCharacters;
	static std::vector<EnemyCharacter*> EnemyCharactersBase;
	static std::vector<std::shared_ptr<Character>> SummonCharacters;
	static std::vector<SummonCharacter> SummonCharactersBase;

	static std::vector<std::pair<int, std::shared_ptr<CombatEffect>>> CombatEffects;
	static std::vector<std::weak_ptr<Character>> TurnTable;

	static int nCycle;
	static int nTurn;
	static bool bNext;
	static bool bCombatFinished;
};