#pragma once

#include "../RPGTypes.h"
#include "../Characters/Character.h"
#include "../Spells/ActiveSpell.h"

struct CharacterStat {
	Character* PtrCharacter;
	EStatType StatType;
	EStatMod StatMod;
	float* Stat;
	std::function<float(const std::shared_ptr<Character>& SPtrCharacter)> GetDelta;
	float Total = 0;
};

struct EffectStat {
	EffectStat(std::vector<CharacterStat>&& AllyStats, std::vector<CharacterStat>&& EnemyStats)
		: AllyStats(AllyStats)
		, EnemyStats(EnemyStats)
	{}
	
	std::vector<CharacterStat> AllyStats;
	std::vector<CharacterStat> EnemyStats;
};

struct EffectParams {
	ECombatEvent OnEvent;
	int StructFlags = 0;
	std::optional<EffectStat> EffectStat;
};

struct ApplyParams {
	int Flags = 0;
	std::optional<EffectStat> EffectStat;
};

struct CombatEffect {
	CombatEffect(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets, std::unique_ptr<ActiveSpell>&& Spell, const std::optional<ApplyParams>& ApplyParams, const std::optional<EffectParams>& EffectParams, const int Duration)
		: Instigator(Instigator)
		, Targets(Targets)
		, Spell(std::move(Spell))
		, ApplyParams(ApplyParams)
		, EffectParams(EffectParams)
		, Duration(Duration)
	{}

	std::shared_ptr<Character> Instigator;
	std::vector<std::weak_ptr<Character>> Targets;
	std::unique_ptr<ActiveSpell> Spell;
	std::optional<ApplyParams> ApplyParams;
	std::optional<EffectParams> EffectParams;
	int Duration;
	int Index = 0;
	int TurnApplied = -1;
};
