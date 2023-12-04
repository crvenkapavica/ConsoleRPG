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

	};

	switch (player_class) {

	}

	for (const auto& stats : player_character_stats)
		if (stats._class == player_class) return stats;

	return PlayerStats();
}

CharacterData::EnemyStats CharacterData::InitEnemyCharacterStats(ECharacterClass enemy_class) {
	
	vector<EnemyStats> enemy_character_stats {
//		               class			   health  essence    stamina       armor      dmg_melee   dmg_rng    crit_chnc    crit_dmg   s_crit_chnc  s_crit_dmg	 a_r    f_r	     l_r    c_r     p_r		n_r			spell_1					s_2		s_3		s_4
		{ ECharacterClass::ENEMY_LVL1,		35.f,	20.f,		0.f,		25.f,		3.8f,		1.2f,		0.05f,		180.f,		0.04f,		200.f,		{0.f,	0.f,	0.f,	0.f,	0.f,	0.f},	{ESpellID::FIREBALL, 1},	{},		{},		{} },
		{ ECharacterClass::ENEMY_LVL2,		50.f,	30.f,		0.f,		40.f,		5.2f,		1.6f,		0.07f,		190.f,		0.04f,		210.f,		{7.f,	7.f,	7.f,	7.f,	7.f,	0.f},	{ESpellID::FIREBALL, 2},	{},		{},		{} },
		{ ECharacterClass::ENEMY_LVL3,		80.f,	50.f,		0.f,		55.f,		6.6f,		2.1f,		0.11f,		220.f,		0.07f,		220.f,		{10.f,	10.f,	10.f,	10.f,	10.f,	5.f},	{ESpellID::FIREBALL, 3},	{},		{},		{} }
	};

	for (const auto& stats : enemy_character_stats)
		if (stats._class == enemy_class) return stats;

	return EnemyStats();
}

CharacterData::PlayerAttributes CharacterData::InitPlayerCharacterAttributes(ECharacterClass player_class) {

	vector<PlayerAttributes> player_attribute_data {
//	 	    class		               str     agi     int     vit     con     end
		{ECharacterClass::BARBARIAN,	3,		1,		1,		6,		1,		3 },
		{ECharacterClass::WARLOCK,		1,		1,		4,		3,		4,		1 }

		// dodati kolko gainas life i essence po levelu (mozda i jos nekaj -> neovisno o atributima)
		// odvojiti skills od attributes
	};
	
	for (const auto& attributes : player_attribute_data)
		if (attributes._class == player_class) return attributes;

	return PlayerAttributes();
}

void CharacterData::InitStatsPerAttribute(ECharacterClass character_class) {

	switch (character_class) {
	case ECharacterClass::BARBARIAN:
		InitStatsPerAttirbute_Barbarian();
		break;
	default:
		break;
	}
}

void CharacterData::InitStatsPerAttirbute_Barbarian() {

	stat_pair stat_vector;

	stat_vector.push_back(make_pair(&_health, 1.f));
	stat_vector.push_back(make_pair(&_stamina, 2.f));
	stat_vector.push_back(make_pair(&_damage_melee, 2.6f));
	stat_vector.push_back(make_pair(&_crit_damage, 8.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._strength, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_armor, 0.3f));
	stat_vector.push_back(make_pair(&_attack_speed, 0.15f));
	stat_vector.push_back(make_pair(&_damage_ranged, 0.3f));
	stat_vector.push_back(make_pair(&_crit_chance, 0.3f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._agility, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_essence, 5.f));
	stat_vector.push_back(make_pair(&_crit_chance, 0.5f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._intelligence, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_health, 0.5f));
	stat_vector.push_back(make_pair(&_stamina, 0.7f));
	stat_vector.push_back(make_pair(&_attack_speed, 0.08f));
	//stat_vector.push_back(make_pair(&_light_radius, 0.25f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._consciousness, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_health, 6.f));
	stat_vector.push_back(make_pair(&_stamina, 0.8f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._vitality, stat_vector));
}