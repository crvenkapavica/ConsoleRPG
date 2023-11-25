#pragma once
#include "../RPGTypes.h"
#include "CSpellData.h"
#include "../Effects/ActiveEffect.h"

class Character;
class PlayerCharacter;
class EnemyCharacter;
struct EffectParams;

class Spell {

public:

	Spell(CSpellData&& data, int level);

	// Create spell and assign it to owning character
	static std::shared_ptr<Spell> CreateSpell(ESpellID spell_id, int level);

	// Invoke the default spell effect + an additional effect (idx) that the spell supports (if chosen)
	void InvokeEffect(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<int>& t1_idx, vector<int>& t2_idx, int effect_idx);

	// Set effects of each spell (usually a default effect + 3 additional)
	inline void SetActiveEffects(const vector<shared_ptr<ActiveEffect>> effects) { _active_effects = effects; }

private:

	int _level;
	int _experience;
	
	// Spell name
	const ESpellID			_ID;

	// Spell activity (active / passive) // TODO [ADD OUT OF COMBAT ACTIVITY]
	const ESpellActivity	_spell_activity;

	// The power level (rarity) of the spell
	const ESpellRarity		_spell_rarity;

private:

	// Pointer to spell effects
	vector<shared_ptr<ActiveEffect>> _active_effects;

	// Minimum required level for each spells effect and their names
	vector<pair<int, string>> _effect_levels;

	// Spell data for each level
	vector<unique_ptr<CSpellData::EffectData>> _effect_data;

public:

	inline const vector<shared_ptr<ActiveEffect>>& GetEffects() const { return _active_effects; }
	inline const vector<pair<int, string>>& GetEffectLevelNameVector() const { return _effect_levels; }

	inline void SetLevel(int level) { _level = level; }
	inline const int GetLevel() const { return _level; }
	inline const int GetExperience() const { return _experience; }
	inline const ESpellID GetID() const { return _ID; }
	inline const ESpellActivity GetActivity() const { return _spell_activity; }

	inline const float GetEffectMax(int idx, int level) const { return _effect_data[idx]->_e[level]._effect_max; }
	inline const float GetEffectMin(int idx, int level) const { return _effect_data[idx]->_e[level]._effect_min; }
	inline const float GetOnApplyMax(int idx, int level) const { return _effect_data[idx]->_e[level]._apply_max; }
	inline const float GetOnApplyMin(int idx, int level) const { return _effect_data[idx]->_e[level]._apply_min; }
	inline const int GetDuration(int idx, int level) const { return _effect_data[idx]->_e[level]._duration; }
	inline const int GetCooldown(int idx, int level) const { return _effect_data[idx]->_e[level]._cooldown; }
	inline const int GetCost(int idx, int level) const { return _effect_data[idx]->_e[level]._cost; }
};