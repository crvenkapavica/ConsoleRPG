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
	static std::unordered_map<ESpellID, std::vector<SpellData>> _spell_level_map;

private:
	SpellDB(ESpellID spell_id);

	static struct SpellLevelMapConstructor {
		SpellLevelMapConstructor();
	} _spell_level_map_constructor;
};
