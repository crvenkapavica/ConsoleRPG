#include "Character.h"
#include "../Spells/SpellBook.h"
#include "../Spells/SpellManager.h"
#include "../GameplayStatics.h"
#include "../Effects/PassiveSpell.h"

Character::Character(const CharacterData::EnemyStats& data)
{
	_class = data._class;
	_health = data._health;
	_essence = data._essence;
	_stamina = data._stamina;
	_armor = data._armor;
	_attack_power = data._attack_power;
	_crit_chance = data._crit_chance;
	_crit_damage = data._crit_damage;
	_spell_power = data._spell_power;
	_spell_crit_chance = data._spell_crit_chance;
	_spell_crit_damage = data._spell_crit_damage;
	_resistances = data._resistances;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CreateSpell(this, data._spell1.first, data._spell1.second);
}

Character::Character(const CharacterData::PlayerAttributes& attributes)
	: _player_attributes(attributes)
{
	_class = attributes._class;
	_crit_damage = 2.f;
	_crit_chance = 0.05f;
	_spell_crit_damage = 2.5f;
	_spell_crit_chance = 0.05f;
	InitStatsPerAttribute();
	InitStats();
}

Character::Character(const Character& other)
	: _class(other._class)
	, _health(other._health)
	, _essence(other._essence)
	, _stamina(other._stamina)
	, _armor(other._armor)
	, _attack_power(other._attack_power)
	, _crit_chance(other._crit_chance)
	, _crit_damage(other._crit_damage)
	, _spell_power(other._spell_power)
	, _spell_crit_chance(other._spell_crit_chance)
	, _spell_crit_damage(other._spell_crit_damage)
	, _player_attributes(other._player_attributes)
	, _resistances(other._resistances)
{}

Character::~Character() 
{
}

void Character::Stat::UpdateBase(const float value) {
	_base += value;
	_actual += value;
}

void Character::Stat::SetActual(const float value) {
	_actual = value;
}

void Character::Stat::SetMax(const float value) {
	_max = value;
}

void Character::Stat::UpdateActual(const float value, Character* character) {
	_actual += value;
	if (_actual > _max)
		_max = _actual;

	if (character->GetHealth().GetActual() <= 0)
		character->Die();
}

void Character::InitStats() {
	for (int idx = 0; idx < _stat_per_attribute.size(); ++idx) {
		if (_stat_per_attribute[idx].first == &_player_attributes._strength)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * _player_attributes._strength);
		else if (_stat_per_attribute[idx].first == &_player_attributes._agility)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * _player_attributes._agility);
		else if (_stat_per_attribute[idx].first == &_player_attributes._intelligence)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * _player_attributes._intelligence);
		else if (_stat_per_attribute[idx].first == &_player_attributes._vitality)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * _player_attributes._vitality);
		else if (_stat_per_attribute[idx].first == &_player_attributes._consciousness)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * _player_attributes._consciousness);
		else if (_stat_per_attribute[idx].first == &_player_attributes._endurance)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * _player_attributes._endurance);
	}
}

void Character::UpdateAttribute(Attribute& attribute, const int amount) {

	for (int idx = 0; idx < _stat_per_attribute.size(); ++idx) {
		if (_stat_per_attribute[idx].first == &attribute) {
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++) {
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * amount);
			}
		}
	}
	attribute += amount;
}

void Character::AddSpell(shared_ptr<SpellBook> spell) {
	_spells.push_back(spell);
}

void Character::AddPassive(shared_ptr<PassiveSpell> passive) {
	_passives.push_back(passive);
}

void Character::RemoveEffectById(EEffectID effect_id) {
	for (auto it = _effect_ids.begin(); it != _effect_ids.end();)
		if (*it == effect_id)
			it = _effect_ids.erase(it);
		else ++it;
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

	stat_vector.push_back(make_pair(&_crit_damage, 5.f));
	stat_vector.push_back(make_pair(&_attack_power, 12.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._strength, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_armor, 20.f));
	stat_vector.push_back(make_pair(&_crit_chance, 0.002f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._agility, stat_vector));

	stat_vector.clear();
	//stat_vector.push_back(make_pair(&_spell_crit_chance, 0.00125f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._intelligence, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_health, 10.f));
	stat_vector.push_back(make_pair(&_stamina, 2.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._vitality, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_essence, 5.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._consciousness, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_stamina, 15.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes._endurance, stat_vector));
}

void Character::EndTurn() {
	GameplayStatics::EndTurn(this);
}

void Character::Die() {
	_bIsAlive = false;
}