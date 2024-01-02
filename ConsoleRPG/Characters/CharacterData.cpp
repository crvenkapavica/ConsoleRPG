#include "CharacterData.h"

std::unordered_map<ECharacterClass, CharacterData> CharDB::_data;
std::unordered_map<ECharacterClass, PlayerAttributes> CharDB::_attributes;

CharDB::CharacterDataConstructor::CharacterDataConstructor() {
	//		         															  health   essence    stamina      armor        AP         crit_%     crit_dmg		 SP	  s_crit_%    s_crit_dmg	   damage_type				a_r		f_r	    l_r     c_r     p_r		n_r		spell_1								spell_2							spell_3							spell_4							passive1						passive2						slot_MH									slot_OH					power_lvl
			// PLAYERS
	_data[ECharacterClass::BARBARIAN] = { ECharacterClass::BARBARIAN,				0.f,	0.f,		0.f,		0.f,		0.f,		0.07f,		200.f,		0.f,	0.03f,		160.f,		EDamageType::PHYSICAL,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::NONE, 0},				{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						{}			};
	_data[ECharacterClass::WARLOCK] = { ECharacterClass::WARLOCK,					0.f,	0.f,		0.f,		0.f,		0.f,		0.03f,		140.f,		0.f,	0.08f,		200.f,		EDamageType::PHYSICAL,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::NONE, 0},				{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						{}			};

			// SUMMONS
	_data[ECharacterClass::FIRE_ELEMENTAL] = { ECharacterClass::FIRE_ELEMENTAL,		300.f,	0.f,		0.f,		0.f,		0.f,		0.03f,		140.f,		0.f,	0.08f,		200.f,		EDamageType::FIRE,			{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::FIREBALL, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						{}			};
	_data[ECharacterClass::FIRE_IMP] = { ECharacterClass::FIRE_IMP,					16.f,	40.f,		0.f,		4.f,		0.f,		0.11f,		220.f,		10.f,	0.07f,		220.f,		EDamageType::FIRE,			{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::BURNING, 0},				{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						{}			};

			// ENEMIES
	_data[ECharacterClass::E_GHOUL] = { ECharacterClass::E_GHOUL,					35.f,	0.f,		30.f,		15.f,		3.8f,		0.05f,		180.f,		10.f,	0.04f,		200.f,		EDamageType::PHYSICAL,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::MELEE, 0},				{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						1,			};
	_data[ECharacterClass::E_ARCHER] = { ECharacterClass::E_ARCHER,					20.f,	0.f,		10.f,		7.f,		5.2f,		0.07f,		190.f,		10.f,	0.04f,		210.f,		EDamageType::PHYSICAL,		{7.f,	7.f,	7.f,	7.f,	7.f,	0.f},	{ESpellID::RANGED, 0},				{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						2,			};
	_data[ECharacterClass::E_FIRE_IMP] = { ECharacterClass::E_FIRE_IMP,				24.f,	40.f,		0.f,		4.f,		6.6f,		0.11f,		220.f,		10.f,	0.07f,		220.f,		EDamageType::FIRE,			{10.f,	10.f,	10.f,	10.f,	10.f,	5.f},	{ESpellID::BURNING, 0},				{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						3,			};
	_data[ECharacterClass::E_MAGE] = { ECharacterClass::E_MAGE,						36.f,	30.f,		10.f,		12.f,		6.6f,		0.11f,		220.f,		10.f,	0.07f,		220.f,		EDamageType::FIRE,			{10.f,	10.f,	10.f,	10.f,	10.f,	5.f},	{ESpellID::FIREBALL, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						4,			};
	_data[ECharacterClass::E_SWORDSMAN] = { ECharacterClass::E_SWORDSMAN,			48.f,	0.f,		20.f,		45.f,		6.6f,		0.11f,		220.f,		10.f,	0.07f,		220.f,		EDamageType::PHYSICAL,		{10.f,	10.f,	10.f,	10.f,	10.f,	5.f},	{ESpellID::NONE, 0},				{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::THORNS, 0},			{ESpellID::NONE, 0},			{},										{},						5,			};
	_data[ECharacterClass::E_WARLOCK] = { ECharacterClass::E_WARLOCK,				30.f,	50.f,		10.f,		20.f,		6.6f,		0.11f,		220.f,		10.f,	0.07f,		220.f,		EDamageType::FIRE,			{10.f,	10.f,	10.f,	10.f,	10.f,	5.f},	{ESpellID::SUM_FIRE_IMP, 0},		{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{ESpellID::NONE, 0},			{},										{},						6,			};
}
CharDB::CharacterDataConstructor CharDB::_character_data_constructor;

CharDB::PlayerAttributesConstructor::PlayerAttributesConstructor() {
//	 	    class								   str     agi     int     vit     con	   end    
	_attributes[ECharacterClass::BARBARIAN] = {		3,		2,		1,		6,		1,		4 };
	_attributes[ECharacterClass::WARLOCK] = {		1,		1,		4,		2,		3,		1 };
}
CharDB::PlayerAttributesConstructor CharDB::_player_attributes_constructor;