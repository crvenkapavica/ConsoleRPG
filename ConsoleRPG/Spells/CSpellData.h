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

struct ActiveConstData {
	EDamageType _damage_type;
	ESpellType _spell_type;
	ESpellRarity _rarity;
	int _min_req_lvl;
};

struct PassiveConstData {
	ECombatEvent _combat_event;
	EPassiveType _passive_type;
	ESpellRarity _rarity;
	int _min_req_lvl;
};

class SpellDB {
public:
	static std::unordered_map<ESpellID, std::vector<SpellData>> _spell_level_map;
	static std::unordered_map<ESpellID, ActiveConstData> _active_const_map;
	static std::unordered_map<ESpellID, PassiveConstData> _passive_const_map;

private:
	SpellDB(ESpellID spell_id);

	static struct SpellLevelMapConstructor {
		SpellLevelMapConstructor();
	} _spell_level_map_constructor;
	static struct ActiveConstDataConstructor {
		ActiveConstDataConstructor();
	} _active_const_data_constructor;
	static struct PassiveConstDataConstructor {
		PassiveConstDataConstructor();
	} _passive_const_data_constructor;
};
