#include "SpellData.h"
#include "../GameplayStatics.h"

std::unordered_map<ESpellID, vector<SpellData>> SpellDB::_spell_level_map;
std::unordered_map<ESpellID, ActiveConstData> SpellDB::_active_const_map;
std::unordered_map<ESpellID, PassiveConstData> SpellDB::_passive_const_map;

SpellDB::SpellLevelMapConstructor::SpellLevelMapConstructor() {
	_spell_level_map[ESpellID::FIREBALL] = {
//		app_min	   app_max	   val_min	   val_max	 duration   cd	   cost	
		{ 5.2f,		6.6f,		0.f,		0.f,		2,		2,		15 },
		{ 6.2f,		8.2f,		0.f,		0.f,		2,		2,		20 },
		{ 7.5f,		11.2f,		0.f,		0.f,		2,		2,		30 },
		{ 11.5f,	15.8f,		0.f,		0.f,		2,		1,		45 },
		{ 16.3f,	21.9f,		0.f,		0.f,		2,		1,		65 }
	};
	_spell_level_map[ESpellID::BURNING] = {
		{ 0.f,		0.f,		13.2f,		14.3f,		3,		2,		10 },
		{ 0.f,		0.f,		4.3f,		5.4f,		3,		2,		20 },
		{ 0.f,		0.f,		5.5f,		7.f,		3,		2,		30 },
		{ 0.f,		0.f,		7.2f,		9.1f,		4,		1,		45 },
		{ 0.f,		0.f,		9.2f,		11.5f,		4,		1,		60 }
	};
	_spell_level_map[ESpellID::MOLTEN_ARMOR] = {
		{ 2.5f,		3.8f,		1.4f,		2.6f,		3,		2,		10 },
		{ 3.4f,		5.2f,		2.3f,		3.4f,		3,		2,		20 },
		{ 4.8f,		6.6f,		3.2f,		4.2f,		3,		2,		30 },
		{ 6.2f,		9.9f,		4.0f,		5.3f,		3,		2,		45 },
		{ 9.5f,		13.5f,		5.0f,		6.6f,		4,		2,		60 }
	};
	_spell_level_map[ESpellID::EXPOSURE] = {
		{ 20.f,		25.f,		0.f,		0.f,		4,		2,		35 },
		{ 25.f,		40.f,		0.f,		0.f,		4,		2,		60 },
		{ 40.f,		60.f,		0.f,		0.f,		4,		2,		85 },
		{ 60.f,		75.f,		0.f,		0.f,		4,		2,		100 },
		{ 80.f,		120.f,		0.f,		0.f,		4,		2,		130 }
	};
	_spell_level_map[ESpellID::STONESKIN] = {
		{ 0.6f,		0.8f,		0.0f,		0.0f,		2,		1,		10 },
		{ 0.8f,		1.1f,		0.0f,		0.0f,		2,		1,		12 },
		{ 1.1f,		1.5f,		0.0f,		0.0f,		2,		1,		15 },
		{ 1.6f,		2.1f,		0.0f,		0.0f,		3,		1,		20 },
		{ 2.0f,		2.7f,		0.0f,		0.0f,		3,		1,		25 }
	};
	_spell_level_map[ESpellID::DISARM] = {
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 }
	};
	_spell_level_map[ESpellID::THRONS] = {
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 }
	};
	_spell_level_map[ESpellID::BLOODBATH] = {
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0 }
	};
	_spell_level_map[ESpellID::ARCANE_INFUSION] = {
		{ 5.f,		10.f,		5.6f,		11.3f,		2,		4,		30 },
		{ 7.5f,		12.f,		9.8f,		14.3f,		2,		4,		45 },
		{ 11.5f,	15.f,		14.1f,		18.6f,		2,		4,		60 },
		{ 15.f,		20.f,		17.9f,		23.1f,		2,		4,		75 },
		{ 19.f,		25.f,		20.5,		28.5f,		3,		4,		115 }
	};
	_spell_level_map[ESpellID::BLOOD_RAIN] = {
		{ 0.f,		0.f,		3.6f,		5.2f,		2,		4,		20 },
		{ 0.f,		0.f,		5.0f,		7.7f,		2,		4,		20 },
		{ 0.f,		0.f,		8.2f,		11.7f,		2,		4,		20 },
		{ 0.f,		0.f,		12.3f,		16.5f,		2,		4,		20 },
		{ 0.f,		0.f,		15.8f,		19.2f,		3,		4,		20 }
	};
	_spell_level_map[ESpellID::VISCOUS_ACID] = {
		{ 0.5f,		0.7f,		4.6f,		5.9f,		2,		4,		20 },
		{ 0.7f,		0.9f,		6.0f,		8.2f,		2,		4,		20 },
		{ 0.9f,		1.1f,		9.5f,		12.4f,		2,		4,		20 },
		{ 1.1f,		1.4f,		13.8f,		19.8f,		2,		4,		20 },
		{ 1.4f,		2.1f,		21.0f,		26.6f,		2,		4,		20 }
	};
}
SpellDB::SpellLevelMapConstructor SpellDB::_spell_level_map_constructor;

SpellDB::ActiveConstDataConstructor::ActiveConstDataConstructor() {
	_active_const_map[ESpellID::FIREBALL] = { EDamageType::FIRE, ESpellType::PROJECTILE, ESpellRarity::BASIC, 1 };
	_active_const_map[ESpellID::BURNING] = { EDamageType::FIRE, ESpellType::DEBUFF, ESpellRarity::BASIC, 1 };
	_active_const_map[ESpellID::MOLTEN_ARMOR] = { EDamageType::FIRE, ESpellType::DEBUFF, ESpellRarity::BASIC, 1 };
	_active_const_map[ESpellID::EXPOSURE] = { EDamageType::FIRE, ESpellType::DEBUFF, ESpellRarity::BASIC, 3 };
	_active_const_map[ESpellID::STONESKIN] = { EDamageType::NONE, ESpellType::BUFF, ESpellRarity::BASIC, 1 };
	_active_const_map[ESpellID::DISARM] = { EDamageType::NONE, ESpellType::DEBUFF, ESpellRarity::ENHANCED, 7 };
	_active_const_map[ESpellID::THRONS] = { EDamageType::PHYSICAL, ESpellType::BUFF, ESpellRarity::ENHANCED, 9 };
	_active_const_map[ESpellID::BLOODBATH] = { EDamageType::PURE, ESpellType::BUFF, ESpellRarity::EPIC, 12 };
	_active_const_map[ESpellID::ARCANE_INFUSION] = { EDamageType::ARCANE, ESpellType::BUFF, ESpellRarity::ENHANCED, 5 };
	_active_const_map[ESpellID::BLOOD_RAIN] = { EDamageType::NECROTIC, ESpellType::DEBUFF, ESpellRarity::ENHANCED, 5 };
	_active_const_map[ESpellID::VISCOUS_ACID] = { EDamageType::POISON, ESpellType::DEBUFF, ESpellRarity::BASIC, 3 };
};
SpellDB::ActiveConstDataConstructor SpellDB::_active_const_data_constructor;

SpellDB::PassiveConstDataConstructor::PassiveConstDataConstructor() {
	_passive_const_map[ESpellID::VAMPIRIC_TOUCH] = { ECombatEvent::ON_TURN_END, EPassiveType::PASSIVE, ESpellRarity::BASIC, 1 };
};
SpellDB::PassiveConstDataConstructor SpellDB::_passive_const_data_constructor;