#pragma once
#include "../RPGTypes.h"

struct SpellData {
	float   _apply_min;
	float   _apply_max;
	float	_effect_min;
	float	_effect_max;
	int		_duration;
	int		_cooldown;
	int		_cost;
};

class SpellDB {

public:

	SpellDB(ESpellBookID spell_id, ESpellActivity spell_activity);

	//inline vector<unique_ptr<EffectData>>& GetEffectData() { return _effect_data; }

	//inline const SpellConstData* GetSpellConstData() const { return _spell_const_data; }

	//inline const vector<pair<int, string>> GetEffectLevels() const { return _effect_levels; }

private:

	void AddSpellConsts(const string& spell_name, const SpellConstData& data);

	void AddSpellLevels(const string& spell_name, const vector<EffectData>& data);

	void AddEffectLevels(const string& spell_name, const vector<pair<int, string>> data);

	void CreateSpellMap();

	void FindAndAssignSpellData(ESpellBookID spell_id);

	void FindAndAssignPassiveSpellData(ESpellBookID spell_id);

private:

	//vector<unique_ptr<EffectLevelData>> _effect_data;
	static std::unordered_map<ESpellID, vector<SpellData>> _spell_level_map;
	static struct SpellLevelMapConstructor {
		SpellLevelMapConstructor();
	} _spell_level_map_constructor;
};
