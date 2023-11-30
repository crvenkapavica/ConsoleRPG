#include "CharacterData.h"

CharacterData::CharacterData(ECharacterClass character_class)
	: _player_attribute_data(InitPlayerCharacterAttributes(character_class))
{
	if (static_cast<int>(character_class) >= 50) {
		_enemy_data = InitEnemyCharacterStats(character_class);
	}
	else {
		_player_data = InitPlayerCharacterStats(character_class);
	}
}

CharacterData::PlayerStats CharacterData::InitPlayerCharacterStats(ECharacterClass player_class) {
	vector<PlayerStats> player_character_stats{
//		               class			   health  essence    stamina      armor        AS      dmg_melee    dmg_rng    crit_chnc    crit_dmg   s_crit_chnc  s_crit_dmg   magic_f  radius
		{ ECharacterClass::BARBARIAN,		227.f,	100.f,		40.f,		0.f,		4.2f,		4.2f,		1.f,		0.08f,		225.f,		0.035f,		140.f,		0.1f,	4.f },
		{ ECharacterClass::WARLOCK,			237.f,	60.f,		10.f,		0.f,		4.2f,		1.6f,		1.7f,		0.03f,		160.f,		0.085f,		210.f,		0.1f,	4.f }
	};

	for (const auto& stats : player_character_stats)
		if (stats._class == player_class) return stats;

	return PlayerStats();
}

CharacterData::EnemyStats CharacterData::InitEnemyCharacterStats(ECharacterClass enemy_class) {
	
	vector<EnemyStats> enemy_character_stats {
//		               class			   health  essence    stamina       armor        AS       dmg_melee   dmg_rng    crit_chnc    crit_dmg   s_crit_chnc  s_crit_dmg	 a_r    f_r	     l_r    c_r     p_r		n_r			spell_1					s_2		s_3		s_4
		{ ECharacterClass::ENEMY_LVL1,		35.f,	20.f,		0.f,		25.f,		1.2f,		3.8f,		1.2f,		0.05f,		180.f,		0.04f,		200.f,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::FIREBALL, 1},	{},		{},		{} },
		{ ECharacterClass::ENEMY_LVL2,		50.f,	30.f,		0.f,		40.f,		1.9f,		5.2f,		1.6f,		0.07f,		190.f,		0.04f,		210.f,		{7.f,	7.f,	7.f,	7.f,	7.f,	0.f},	{ESpellID::FIREBALL, 2},	{},		{},		{} },
		{ ECharacterClass::ENEMY_LVL3,		80.f,	50.f,		0.f,		55.f,		2.8f,		6.6f,		2.1f,		0.11f,		220.f,		0.07f,		220.f,		{10.f,	10.f,	10.f,	10.f,	10.f,	5.f},	{ESpellID::FIREBALL, 3},	{},		{},		{} }
	};

	for (const auto& stats : enemy_character_stats)
		if (stats._class == enemy_class) return stats;

	return EnemyStats();
}

CharacterData::PlayerAttributes CharacterData::InitPlayerCharacterAttributes(ECharacterClass player_class) {

	vector<PlayerAttributes> player_attribute_data {
//	 	    class		               str     agi     int     con     vit    craft   percep   acrob
		{ECharacterClass::BARBARIAN,	3,		2,		1,		2,		6,		4,		2,		1},
		{ECharacterClass::WARLOCK,		1,		1,		4,		8,		3,		2,		5,		1}

		// dodati kolko gainas life i essence po levelu (mozda i jos nekaj -> neovisno o atributima)
		// odvojiti skills od attributes
	};
	
	for (const auto& attributes : player_attribute_data)
		if (attributes._class == player_class) return attributes;

	return PlayerAttributes();
}