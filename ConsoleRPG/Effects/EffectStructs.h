#pragma once
#include "../RPGTypes.h"
#include "../Characters/Character.h"
#include "../Effects/ActiveSpell.h"

struct Effect_MultiTarget {
	int min;
	int max;
};

struct CharacterStat {
	Character* _character;
	EStatType _stat_type;
	EStatMod _stat_mod;
	Character::Stat* _stat;
	std::function<float(Character* character)> _getDmg;
	float _total = 0;
};

struct CharacterRes {
	Character* _character;
	EStatType _stat_type;
	EStatMod _stat_mod;
	float* _res;
	float _value;
	float _total = 0;
};

struct Effect_Stat {
	vector<CharacterStat> _ally_stat;
	vector<CharacterStat> _enemy_stat;

	EStatValueAction _value_action;

	Effect_Stat(vector<CharacterStat> ally_stat, vector<CharacterStat> enemy_stat, EStatValueAction value_action)
		: _ally_stat(move(ally_stat))
		, _enemy_stat(move(enemy_stat))
		, _value_action(value_action)
	{}
};

struct Effect_Res {
	vector<CharacterRes> _ally_res;
	vector<CharacterRes> _enemy_res;

	Effect_Res(vector<CharacterRes> ally_res, vector<CharacterRes> enemy_res)
		: _ally_res(move(ally_res))
		, _enemy_res(move(enemy_res))
	{}
};

struct EffectParams {

	ECombatEvent _on_event;

	uint8_t	_struct_flags = 0;

	std::optional<Effect_MultiTarget>	_effect_multi_target;
	std::optional<Effect_Stat>			_effect_stat;
	std::optional<Effect_Res>			_effect_res;
};

struct OnApplyParams {

	ECombatEvent _on_event;

	int	_struct_flags = 0;

	std::optional<Effect_MultiTarget>	_effect_multi_target;
	std::optional<Effect_Stat>			_effect_stat;
	std::optional<Effect_Res>			_effect_res;
};

struct CombatEffect {

	Character* _instigator;
	std::vector<weak_ptr<Character>> _targets;
	EffectParams _effect_params;
	OnApplyParams _apply_params;
	std::shared_ptr<ActiveSpell> _spell;
	int i = 0;
	int _duration;
	int _turn_applied = -1;

	CombatEffect(Character* instigator, std::vector<weak_ptr<Character>> targets, EffectParams effect_params, OnApplyParams apply_params, shared_ptr<ActiveSpell> spell, int duration)
		: _instigator(instigator)
		, _targets(targets)
		, _effect_params(effect_params)
		, _apply_params(apply_params)
		, _spell(spell)
		, _duration(duration)
	{}
};
