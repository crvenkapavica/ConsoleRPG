#pragma once
#include "../RPGTypes.h"

using namespace std;

class CSpellData {

public:

	CSpellData(ESpellBookID spell_id, ESpellActivity spell_activity);

	struct EffectLevelData {
		float   _apply_min;
		float   _apply_max;
		float	_effect_min;
		float	_effect_max;
		int		_duration;
		int		_cooldown;
		int		_cost;
	};

	struct EffectData {
		vector<EffectLevelData> _e;
	};

	struct SpellConstData {
		ESpellBookID			_ID;
		ESpellActivity		_spell_activity;
		ESpellRarity		_spell_rarity;
	};


public:

	inline vector<unique_ptr<EffectData>>& GetEffectData() { return _effect_data; }

	inline const SpellConstData* GetSpellConstData() const { return _spell_const_data; }

	inline const vector<pair<int, string>> GetEffectLevels() const { return _effect_levels; }

private:

	void AddSpellConsts(const string& spell_name, const SpellConstData& data);

	void AddSpellLevels(const string& spell_name, const vector<EffectData>& data);

	void AddEffectLevels(const string& spell_name, const vector<pair<int, string>> data);

	void CreateSpellMap();

	void FindAndAssignSpellData(ESpellBookID spell_id);

	void CreatePassiveSpellMap();

	void FindAndAssignPassiveSpellData(ESpellBookID spell_id);

private:

	SpellConstData* _spell_const_data;
	map<string, SpellConstData>	 _spell_const_map;

	vector<unique_ptr<EffectData>> _effect_data;
	map<string, vector<unique_ptr<EffectData>>>	 _effect_map;

	vector<pair<int, string>> _effect_levels;
	map<string, vector<pair<int, string>>> _effect_level_map;
};








/*
void PrintStats() const {
	std::string stackable = _bStackable ? "Yes" : "No";
	std::cout << ANSI_COLOR_MAGENTA << "=====================" << GetEnumString(_spell_ID) << "=====================" << ANSI_COLOR_CYAN << std::endl;
	std::cout << "Min: " << _value_min << "\nMax: " << _value_max << "\nDuration: " << _duration << "\nCooldown: " << _cooldown << std::endl;
	std::cout << "Cost: " << _cost << "\nStackable: " << stackable << "\nSpell Type: " << GetEnumString(_spell_type) << std::endl;
	std::cout << "Damage type: " << GetEnumString(_damage_type) << "\nSpell activity: " << GetEnumString(_activity) << std::endl;
	std::cout << ANSI_COLOR_MAGENTA << "#########################################################" << std::endl;
	std::cout << ANSI_COLOR_RESET << std::endl;
}*/