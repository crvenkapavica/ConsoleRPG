#include "CharacterData.h"

std::vector<CharacterData> CharDB::_data;
std::vector<PlayerAttributes> CharDB::_attributes;

CharDB::CharacterDataConstructor::CharacterDataConstructor() {
	//		               class							   health   essence    stamina      armor        AP         crit_%     crit_dmg		 SP	  s_crit_%    s_crit_dmg	   damage_type				a_r		f_r	    l_r     c_r     p_r		n_r		spell_1								spell_2							spell_3							spell_4							passive1						passive2						slot_MH									slot_OH
			// PLAYERS
	_data.push_back({ ECharacterClass::BARBARIAN,				0.f,	0.f,		0.f,		0.f,		0.f,		0.07f,		200.f,		0.f,	0.03f,		160.f,		EDamageType::PHYSICAL,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{},									{},								{},								{},								{},								{},								{},										{} });
	_data.push_back({ ECharacterClass::WARLOCK,					0.f,	0.f,		0.f,		0.f,		0.f,		0.03f,		140.f,		0.f,	0.08f,		200.f,		EDamageType::PHYSICAL,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{},									{},								{},								{},								{},								{},								{},										{} });

			// SUMMONS
	_data.push_back({ ECharacterClass::FIRE_ELEMENTAL,			0.f,	0.f,		0.f,		0.f,		0.f,		0.03f,		140.f,		0.f,	0.08f,		200.f,		EDamageType::PHYSICAL,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::FIREBALL, 0},			{},								{},								{},								{},								{},								{},										{} });

			// ENEMIES
	_data.push_back({ ECharacterClass::ENEMY_LVL1,				35.f,	20.f,		0.f,		25.f,		3.8f,		0.05f,		180.f,		10.f,	0.04f,		200.f,		EDamageType::PHYSICAL,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::SUM_FIRE_ELE, 0},		{},								{},								{},								{},								{},								{},										{} });
	_data.push_back({ ECharacterClass::ENEMY_LVL2,				50.f,	30.f,		0.f,		40.f,		5.2f,		0.07f,		190.f,		10.f,	0.04f,		210.f,		EDamageType::PHYSICAL,		{7.f,	7.f,	7.f,	7.f,	7.f,	0.f},	{ESpellID::SUM_FIRE_ELE, 0},		{},								{},								{},								{},								{},								{},										{} });
	_data.push_back({ ECharacterClass::ENEMY_LVL3,				80.f,	50.f,		0.f,		55.f,		6.6f,		0.11f,		220.f,		10.f,	0.07f,		220.f,		EDamageType::PHYSICAL,		{10.f,	10.f,	10.f,	10.f,	10.f,	5.f},	{ESpellID::SUM_FIRE_ELE, 0},		{},								{},								{},								{},								{},								{},										{} });
}
CharDB::CharacterDataConstructor CharDB::_character_data_constructor;

CharDB::PlayerAttributesConstructor::PlayerAttributesConstructor() {
//	 	    class											str     agi     int     vit     con	   end    
	_attributes.push_back({ ECharacterClass::BARBARIAN,		3,		2,		1,		6,		1,		4 });
	_attributes.push_back({ ECharacterClass::WARLOCK,		1,		1,		4,		2,		3,		1 });
}
CharDB::PlayerAttributesConstructor CharDB::_player_attributes_constructor;