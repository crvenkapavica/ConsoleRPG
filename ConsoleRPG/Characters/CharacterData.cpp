#include "CharacterData.h"

CharacterData::CharacterData(ECharacterClass character_class)
	: _player_attribute_data(InitPlayerCharacterAttributes(character_class))
{
	if (static_cast<int>(character_class) >= 50) {
		_enemy_data = InitEnemyCharacterStats(character_class);
	}
}

CharacterData::EnemyStats CharacterData::InitEnemyCharacterStats(ECharacterClass enemy_class) {
	
	vector<EnemyStats> enemy_character_stats {
//		               class			   health  essence    stamina       armor        AP       crit_chnc    crit_dmg		SP	  s_crit_chnc  s_crit_dmg	 a_r    f_r	     l_r    c_r     p_r		n_r			spell_1					s_2		s_3		s_4
		{ ECharacterClass::ENEMY_LVL1,		35.f,	20.f,		0.f,		25.f,		3.8f,		0.05f,		180.f,		10.f,	0.04f,		200.f,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::FIREBALL, 1},	{},		{},		{} },
		{ ECharacterClass::ENEMY_LVL2,		50.f,	30.f,		0.f,		40.f,		5.2f,		0.07f,		190.f,		10.f,	0.04f,		210.f,		{7.f,	7.f,	7.f,	7.f,	7.f,	0.f},	{ESpellID::FIREBALL, 2},	{},		{},		{} },
		{ ECharacterClass::ENEMY_LVL3,		80.f,	50.f,		0.f,		55.f,		6.6f,		0.11f,		220.f,		10.f,	0.07f,		220.f,		{10.f,	10.f,	10.f,	10.f,	10.f,	5.f},	{ESpellID::FIREBALL, 3},	{},		{},		{} }
	};

	for (const auto& stats : enemy_character_stats)
		if (stats._class == enemy_class) return stats;

	return EnemyStats();
}

CharacterData::PlayerAttributes CharacterData::InitPlayerCharacterAttributes(ECharacterClass player_class) {

	vector<PlayerAttributes> player_attribute_data {
//	 	    class		               str     agi     int     vit     con	   end    
		{ECharacterClass::BARBARIAN,	3,		2,		1,		6,		1,		4},
		{ECharacterClass::WARLOCK,		1,		1,		4,		2,		3,		1}

		// dodati kolko gainas life i essence po levelu (mozda i jos nekaj -> neovisno o atributima)
		// odvojiti skills od attributes
	};
	
	for (const auto& attributes : player_attribute_data)
		if (attributes._class == player_class) return attributes;

	return PlayerAttributes();
}