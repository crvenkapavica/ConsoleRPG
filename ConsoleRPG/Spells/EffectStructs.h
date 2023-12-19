#pragma once
#include "../RPGTypes.h"
#include "../Characters/Character.h"
#include "../Spells/ActiveSpell.h"

struct CharacterStat {
	Character* _character;
	EStatType _stat_type;
	EStatMod _stat_mod;
	float* _stat;
	std::function<float(Character* character)> GetDelta;
	float _total = 0;
};

struct Effect_Stat {

	Effect_Stat(vector<CharacterStat> ally_stat, vector<CharacterStat> enemy_stat)
		: _ally_stat(move(ally_stat))
		, _enemy_stat(move(enemy_stat))
	{}

	vector<CharacterStat> _ally_stat;
	vector<CharacterStat> _enemy_stat;
};

struct EffectParams {

	ECombatEvent _on_event;
	int _struct_flags = 0;

	std::optional<Effect_Stat> _effect_stat;
};

struct ApplyParams {

	int _struct_flags = 0;

	std::optional<Effect_Stat> _effect_stat;
};

struct CombatEffect {

	CombatEffect(Character* instigator, std::vector<weak_ptr<Character>> targets, unique_ptr<ActiveSpell>& spell, std::optional<ApplyParams> apply_params, std::optional<EffectParams> effect_params, int duration)
		: _instigator(instigator)
		, _targets(targets)
		, _spell(move(spell))
		, _apply_params(apply_params)
		, _effect_params(effect_params)
		, _duration(duration)
	{}

	Character* _instigator;
	std::vector<weak_ptr<Character>> _targets;
	std::unique_ptr<ActiveSpell> _spell;
	std::optional<ApplyParams> _apply_params;
	std::optional<EffectParams> _effect_params;
	int _duration;

	int i = 0;
	int _turn_applied = -1;
};
