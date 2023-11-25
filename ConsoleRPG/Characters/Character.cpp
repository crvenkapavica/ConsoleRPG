#include "Character.h"
#include "../Spells/Spell.h"
#include "../Spells/SpellManager.h"
#include "../GameplayStatics.h"
#include "../Effects/PassiveEffect.h"

Character::Character(const CharacterData::PlayerStats& data)
{
	_class = data._class;
	_health = data._health;
	_essence = data._essence;
	_stamina = data._stamina;
	_armor = data._armor;
	_attack_speed = data._attack_speed;
	_damage_melee = data._damage_melee;
	_damage_ranged = data._damage_ranged;
	_crit_chance = data._crit_chance;
	_crit_damage = data._crit_damage;
}

Character::Character(const CharacterData::EnemyStats& data)
{
	_class = data._class;
	_health = data._health;
	_essence = data._essence;
	_stamina = data._stamina;
	_armor = data._armor;
	_attack_speed = data._attack_speed;
	_damage_melee = data._damage_melee;
	_damage_ranged = data._damage_ranged;
	_crit_chance = data._crit_chance;
	_crit_damage = data._crit_damage;
	_resistances = data._resistances;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CreateSpell(this, data._spell1.first, data._spell1.second);
}

Character::Character(const CharacterData::PlayerStats& data, const CharacterData::PlayerAttributes& attributes)
	: _player_attributes(attributes)
{
	_class = data._class;
	_health = data._health;
	_essence = data._essence;
	_stamina = data._stamina;
	_armor = data._armor;
	_attack_speed = data._attack_speed;
	_damage_melee = data._damage_melee;
	_damage_ranged = data._damage_ranged;
	_crit_chance = data._crit_chance;
	_crit_damage = data._crit_damage;

	InitStatsPerAttribute();
}

Character::Character(const Character& other)
	: _class(other._class)
	, _health(other._health)
	, _essence(other._essence)
	, _stamina(other._stamina)
	, _armor(other._armor)
	, _attack_speed(other._attack_speed)
	, _damage_melee(other._damage_melee)
	, _damage_ranged(other._damage_ranged)
	, _crit_chance(other._crit_chance)
	, _crit_damage(other._crit_damage)
	, _spell_crit_chance(other._spell_crit_chance)
	, _spell_crit_damage(other._spell_crit_damage)
	, _player_attributes(other._player_attributes)
	, _resistances(other._resistances)
	, _alias(other._alias)
	, _level(other._level)
	, _multi_strike(other._multi_strike)
{}

Character::~Character() 
{
}

void Character::Stat::UpdateBase(const float value) {
	_base += value;
	_max = _base + _bonus;
	//OnStatChanged();
}

void Character::Stat::SetActual(const float value) {
	_actual = value;
	//OnStatChanged();
}

void Character::Stat::UpdateActual(const float value, Character* character) {
	_actual += value;
	if (character->GetHealth().GetActual() <= 0)
		character->Die();
}

void Character::Stat::UpdateBonus(const float value) {
	_bonus += value;
	if (_bonus < 0)
		_bonus = 0.f;
	_max = _base + _bonus;
	//OnStatChanged();
}

void Character::UpdateAttribute(Attribute& attribute, const int amount) {

	// have to TEST
	for (int idx = 0; idx < _stat_per_attribute.size(); ++idx) {
		if (_stat_per_attribute[idx].first == &attribute) {
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++) {
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * amount);
			}
		}
	}
	/*
	int idx = 0;
	for (auto& _attribute : _stat_per_attribute) {
		if (_attribute.first == &attribute) {
			for (int i = 0; i < _attribute.second.size(); i++) {
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * amount);
			}
			++idx;
		}
	}*/
}

void Character::AddSpell(shared_ptr<Spell> spell) {
	_spells.push_back(spell);
}

void Character::AddPassive(shared_ptr<PassiveEffect> passive) {
	_passives.push_back(passive);
}

void Character::InitStatsPerAttribute() {

	switch (_class) {
		case ECharacterClass::BARBARIAN: {
			InitStatsPerAttirbute_Barbarian();
		}
	}
}

void Character::InitStatsPerAttirbute_Barbarian() {

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

void Character::EndTurn() {
	GameplayStatics::EndTurn(this);
}

void Character::Die() {
	_bIsAlive = false;
}