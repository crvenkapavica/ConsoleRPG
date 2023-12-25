#include "SpellData.h"
#include "../GameplayStatics.h"

std::unordered_map<ESpellID, vector<SpellData>> SpellDB::_data;
std::unordered_map<ESpellID, ActiveConstData> SpellDB::_active_const_map;
std::unordered_map<ESpellID, PassiveConstData> SpellDB::_passive_const_map;

SpellDB::SpellDataConstructor::SpellDataConstructor() {
	// MAGIC
	_data[ESpellID::FIREBALL] = {
//		app_min	   app_max	   val_min	   val_max	 duration   cd	   e_cost	s_cost
		{ 5.2f,		6.6f,		0.f,		0.f,		2,		2,		15,		0 },
		{ 6.2f,		8.2f,		0.f,		0.f,		2,		2,		20,		0 },
		{ 7.5f,		11.2f,		0.f,		0.f,		2,		2,		30,		0 },
		{ 11.5f,	15.8f,		0.f,		0.f,		2,		1,		45,		0 },
		{ 16.3f,	21.9f,		0.f,		0.f,		2,		1,		65,		0 }
	};
	_data[ESpellID::BURNING] = {
		{ 0.f,		0.f,		113.2f,		114.3f,		3,		2,		10,		0 },
		{ 0.f,		0.f,		4.3f,		5.4f,		3,		2,		20,		0 },
		{ 0.f,		0.f,		5.5f,		7.f,		3,		2,		30,		0 },
		{ 0.f,		0.f,		7.2f,		9.1f,		4,		1,		45,		0 },
		{ 0.f,		0.f,		9.2f,		11.5f,		4,		1,		60,		0 }
	};
	_data[ESpellID::MOLTEN_ARMOR] = {
		{ 3.f,		3.f,		1.4f,		2.6f,		3,		2,		10,		0 },
		{ 3.4f,		5.2f,		2.3f,		3.4f,		3,		2,		20,		0 },
		{ 4.8f,		6.6f,		3.2f,		4.2f,		3,		2,		30,		0 },
		{ 6.2f,		9.9f,		4.0f,		5.3f,		3,		2,		45,		0 },
		{ 9.5f,		13.5f,		5.0f,		6.6f,		4,		2,		60,		0 }
	};
	_data[ESpellID::EXPOSURE] = {
		{ 20.f,		25.f,		0.f,		0.f,		4,		2,		35,		0 },
		{ 25.f,		40.f,		0.f,		0.f,		4,		2,		60,		0 },
		{ 40.f,		60.f,		0.f,		0.f,		4,		2,		85,		0 },
		{ 60.f,		75.f,		0.f,		0.f,		4,		2,		100,	0 },
		{ 80.f,		120.f,		0.f,		0.f,		4,		2,		130,	0 }
	};
	_data[ESpellID::STONESKIN] = {
		{ 0.6f,		0.8f,		0.0f,		0.0f,		2,		1,		10,		0 },
		{ 0.8f,		1.1f,		0.0f,		0.0f,		2,		1,		12,		0 },
		{ 1.1f,		1.5f,		0.0f,		0.0f,		2,		1,		15,		0 },
		{ 1.6f,		2.1f,		0.0f,		0.0f,		3,		1,		20,		0 },
		{ 2.0f,		2.7f,		0.0f,		0.0f,		3,		1,		25,		0 }
	};
	_data[ESpellID::DISARM] = {
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 }
	};
	_data[ESpellID::BLOODBATH] = {
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 },
		{ 0.0f,		0.0f,		0.0f,		0.0f,		0,		0,		0,		0 }
	};
	_data[ESpellID::ARCANE_INFUSION] = {
		{ 5.f,		10.f,		5.6f,		11.3f,		2,		4,		30,		0 },
		{ 7.5f,		12.f,		9.8f,		14.3f,		2,		4,		45,		0 },
		{ 11.5f,	15.f,		14.1f,		18.6f,		2,		4,		60,		0 },
		{ 15.f,		20.f,		17.9f,		23.1f,		2,		4,		75,		0 },
		{ 19.f,		25.f,		20.5,		28.5f,		3,		4,		115,	0 }
	};
	_data[ESpellID::BLOOD_RAIN] = {
		{ 0.f,		0.f,		3.6f,		5.2f,		2,		4,		20,		0 },
		{ 0.f,		0.f,		5.0f,		7.7f,		2,		4,		20,		0 },
		{ 0.f,		0.f,		8.2f,		11.7f,		2,		4,		20,		0 },
		{ 0.f,		0.f,		12.3f,		16.5f,		2,		4,		20,		0 },
		{ 0.f,		0.f,		15.8f,		19.2f,		3,		4,		20,		0 }
	};
	_data[ESpellID::VISCOUS_ACID] = {
		{ 0.5f,		0.7f,		4.6f,		5.9f,		2,		4,		20,		0 },
		{ 0.7f,		0.9f,		6.0f,		8.2f,		2,		4,		20,		0 },
		{ 0.9f,		1.1f,		9.5f,		12.4f,		2,		4,		20,		0 },
		{ 1.1f,		1.4f,		13.8f,		19.8f,		2,		4,		20,		0 },
		{ 1.4f,		2.1f,		21.0f,		26.6f,		2,		4,		20,		0 }
	};
	//==============================================================================
	// MELEE
	_data[ESpellID::MELEE] = { { 0.f, 0.f, 0.f, 0.f, 0, 0, 0, 15 } };
	//==============================================================================
	// RANGED
	_data[ESpellID::RANGED] = { { 0.f, 0.f, 0.f, 0.f, 0, 0, 0, 10 } };
}
SpellDB::SpellDataConstructor SpellDB::_spell_data_constructor;

SpellDB::ActiveConstDataConstructor::ActiveConstDataConstructor() {
	// MAGIC
	_active_const_map[ESpellID::FIREBALL] = { EDamageType::FIRE, ESpellType::DAMAGE, ESpellRarity::BASIC, ESpellClass::MAGIC, 1 };
	_active_const_map[ESpellID::BURNING] = { EDamageType::FIRE, ESpellType::DEBUFF, ESpellRarity::BASIC, ESpellClass::MAGIC, 1 };
	_active_const_map[ESpellID::MOLTEN_ARMOR] = { EDamageType::NONE, ESpellType::DEBUFF, ESpellRarity::BASIC, ESpellClass::MAGIC, 1 };
	_active_const_map[ESpellID::EXPOSURE] = { EDamageType::NONE, ESpellType::DEBUFF, ESpellRarity::BASIC, ESpellClass::MAGIC, 3 };
	_active_const_map[ESpellID::STONESKIN] = { EDamageType::NONE, ESpellType::BUFF, ESpellRarity::BASIC, ESpellClass::MAGIC, 1 };
	_active_const_map[ESpellID::DISARM] = { EDamageType::NONE, ESpellType::DEBUFF, ESpellRarity::ENHANCED, ESpellClass::MAGIC, 7 };
	_active_const_map[ESpellID::BLOODBATH] = { EDamageType::PURE, ESpellType::BUFF, ESpellRarity::EPIC, ESpellClass::MAGIC, 12 };
	_active_const_map[ESpellID::ARCANE_INFUSION] = { EDamageType::ARCANE, ESpellType::BUFF, ESpellRarity::ENHANCED, ESpellClass::MAGIC, 5 };
	_active_const_map[ESpellID::BLOOD_RAIN] = { EDamageType::NECROTIC, ESpellType::DEBUFF, ESpellRarity::ENHANCED, ESpellClass::MAGIC, 5 };
	_active_const_map[ESpellID::VISCOUS_ACID] = { EDamageType::POISON, ESpellType::DEBUFF, ESpellRarity::BASIC, ESpellClass::MAGIC, 3 };

	// SUMMON
	_active_const_map[ESpellID::SUM_FIRE_ELE] = { EDamageType::FIRE, ESpellType::SUMMON, ESpellRarity::GODLIKE, ESpellClass::MAGIC, 1 };

	//MELEE
	_active_const_map[ESpellID::MELEE] = { EDamageType::PHYSICAL, ESpellType::NONE, ESpellRarity::BASIC, ESpellClass::MELEE, 1 };
	//RANGED
	_active_const_map[ESpellID::RANGED] = { EDamageType::PHYSICAL, ESpellType::NONE, ESpellRarity::BASIC, ESpellClass::RANGED, 1 };
};
SpellDB::ActiveConstDataConstructor SpellDB::_active_const_data_constructor;

SpellDB::PassiveConstDataConstructor::PassiveConstDataConstructor() {
	_passive_const_map[ESpellID::VAMPIRIC_TOUCH] = { ECombatEvent::ON_MAGIC_END, ESpellRarity::EPIC, ESpellClass::MAGIC, 1 };
	_passive_const_map[ESpellID::THORNS] = { ECombatEvent::ON_MELEE_RECEIVED_END, ESpellRarity::BASIC, ESpellClass::MELEE, 1 };
};
SpellDB::PassiveConstDataConstructor SpellDB::_passive_const_data_constructor;