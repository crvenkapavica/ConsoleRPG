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
	vector<CharacterStat> _ally_stat;
	vector<CharacterStat> _enemy_stat;

	Effect_Stat(vector<CharacterStat> ally_stat, vector<CharacterStat> enemy_stat)
		: _ally_stat(move(ally_stat))
		, _enemy_stat(move(enemy_stat))
	{}
};

struct EffectParams {

	ECombatEvent _on_event;
	int _struct_flags = 0;

	std::optional<Effect_Stat> _effect_stat;
};

struct ApplyParams {

	ECombatEvent _on_event;
	int _struct_flags = 0;

	std::optional<Effect_Stat> _effect_stat;
};

struct CombatEffect {

	Character* _instigator;
	std::vector<weak_ptr<Character>> _targets;
	std::unique_ptr<ActiveSpell> _spell;
	ApplyParams _apply_params;
	EffectParams _effect_params;
	int _duration;

	int i = 0;
	int _turn_applied = -1;

	CombatEffect(Character* instigator, std::vector<weak_ptr<Character>> targets, unique_ptr<ActiveSpell>& spell, ApplyParams apply_params, EffectParams effect_params, int duration)
		: _instigator(instigator)
		, _targets(targets)
		, _spell(move(spell))
		, _apply_params(apply_params)
		, _effect_params(effect_params)
		, _duration(duration)
	{}
};
