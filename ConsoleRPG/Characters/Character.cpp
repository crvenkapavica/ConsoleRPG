#include "Character.h"
#include "../Spells/SpellManager.h"
#include "../GameplayStatics.h"
#include "../Spells/PassiveSpell.h"
#include "../Spells/ActiveSpell.h"

Character::Character(CharacterData data, PlayerAttributes attributes, char alias)
	: _player_attributes(attributes)
	, _alias(alias)
	, _team(1)
{
	_class = data._class;
	_health = data.Health;
	_essence = data.Essence;
	_stamina = data.Stamina;
	_armor = data.Armor;
	_attack_power = data.AttackPower;
	_crit_chance = data.CritChance;
	_crit_damage = data.CritDamage;
	_spell_power = data.SpellPower;
	_spell_crit_chance = data.SpellCritChance;
	_spell_crit_damage = data.SpellCritDamage;
	_damage_type = data.DamageType;
	_resistances = data.Resistances;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CreateActiveSpell(this, data.Spell1.first);
	sm.CreateActiveSpell(this, data.Spell2.first);
	sm.CreateActiveSpell(this, data.Spell3.first);
	sm.CreateActiveSpell(this, data.Spell4.first);

	sm.CreateActiveSpell(this, ESpellID::MELEE);
	sm.CreateActiveSpell(this, ESpellID::RANGED);

	sm.CreatePassiveSpell(this, data.Passive1.first);
	sm.CreatePassiveSpell(this, data.Passive2.first);

	// TODO === ADD MAIN_HAND AND OFF_HAND WEAPON SLOTS TO CHARACTER


	// For player Characters
	InitStatsPerAttribute();
	InitStats();
}

Character::Character(CharacterData data, char alias)
	: _alias(alias)
	, _team(2)
{
	_class = data._class;
	_health = data.Health;
	_essence = data.Essence;
	_stamina = data.Stamina;
	_armor = data.Armor;
	_attack_power = data.AttackPower;
	_crit_chance = data.CritChance;
	_crit_damage = data.CritDamage;
	_spell_power = data.SpellPower;
	_spell_crit_chance = data.SpellCritChance;
	_spell_crit_damage = data.SpellCritDamage;
	_damage_type = data.DamageType;
	_resistances = data.Resistances;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CreateActiveSpell(this, data.Spell1.first);
	sm.CreateActiveSpell(this, data.Spell2.first);
	sm.CreateActiveSpell(this, data.Spell3.first);
	sm.CreateActiveSpell(this, data.Spell4.first);

	//sm.CreateActiveSpell(this, ESpellID::MELEE);
	//sm.CreateActiveSpell(this, ESpellID::RANGED);

	sm.CreatePassiveSpell(this, data.Passive1.first);
	sm.CreatePassiveSpell(this, data.Passive2.first);

	// TODO === ADD MAIN_HAND AND OFF_HAND WEAPON SLOTS TO CHARACTER
}

Character::Character(CharacterData data, int team, std::function<char(void)> alias)
	: _alias(alias())
	, _team(team)
{
	_class = data._class;
	_health = data.Health;
	_essence = data.Essence;
	_stamina = data.Stamina;
	_armor = data.Armor;
	_attack_power = data.AttackPower;
	_crit_chance = data.CritChance;
	_crit_damage = data.CritDamage;
	_spell_power = data.SpellPower;
	_spell_crit_chance = data.SpellCritChance;
	_spell_crit_damage = data.SpellCritDamage;
	_damage_type = data.DamageType;
	_resistances = data.Resistances;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CreateActiveSpell(this, data.Spell1.first);
	sm.CreateActiveSpell(this, data.Spell2.first);
	sm.CreateActiveSpell(this, data.Spell3.first);
	sm.CreateActiveSpell(this, data.Spell4.first);

	sm.CreateActiveSpell(this, ESpellID::MELEE);
	sm.CreateActiveSpell(this, ESpellID::RANGED);

	sm.CreatePassiveSpell(this, data.Passive1.first);
	sm.CreatePassiveSpell(this, data.Passive2.first);
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
	, _damage_type(other._damage_type)
	, _resistances(other._resistances)
	, _alias(other._alias)
	, _team(other._team)
{}

Character::Character(Character&& character) noexcept
	: _class(character._class)
	, _player_attributes(character._player_attributes)
	, _damage_type(character._damage_type)
	, _alias(character._alias)
	, _team(character._team)
{}

Character::~Character() {}

void Character::Stat::UpdateBase(const float value) {
	_base += value;
	_actual += value;
	_max += value;
}

void Character::Stat::SetActual(const float value) {
	_actual = value;
}

void Character::Stat::SetMax(float value) {
	_max = value;
	
	if (_actual > _max)
		_actual = _max;
}

void Character::Stat::UpdateActual(const float value, Character* character) {
	_actual += value;
	if (_actual > _max)
		_actual = _max;
}

void Character::Stat::UpdateMax(const float value) {
	_max += value;

	if (value > 0)
		_actual += value;
	
	if (_max < _actual)
		_actual = _max;
}

void Character::InitStats() {

	for (int idx = 0; idx < _stat_per_attribute.size(); ++idx)
		if (_stat_per_attribute[idx].first == &_player_attributes.Strength)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * (_player_attributes.Strength + _i_str));
		else if (_stat_per_attribute[idx].first == &_player_attributes.Agility)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * (_player_attributes.Agility + _i_agi));
		else if (_stat_per_attribute[idx].first == &_player_attributes.Intelligence)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * (_player_attributes.Intelligence + _i_int));
		else if (_stat_per_attribute[idx].first == &_player_attributes.Vitality)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * (_player_attributes.Vitality + _i_vit));
		else if (_stat_per_attribute[idx].first == &_player_attributes.Consciousness)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * (_player_attributes.Consciousness + _i_con));
		else if (_stat_per_attribute[idx].first == &_player_attributes.Endurance)
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++)
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * (_player_attributes.Endurance + _i_end));
}

void Character::UpdateAttribute(Attribute& attribute, int amount) {

	for (int idx = 0; idx < _stat_per_attribute.size(); ++idx)
		if (_stat_per_attribute[idx].first == &attribute) 
			for (int i = 0; i < _stat_per_attribute[idx].second.size(); i++) 
				_stat_per_attribute[idx].second[i].first->UpdateBase(_stat_per_attribute[idx].second[i].second * amount);

	attribute += amount;
}

void Character::AddActiveSpell(unique_ptr<ActiveSpell>& spell) {
	_active_spells.push_back(move(spell));
}

void Character::AddPassiveSpell(unique_ptr<PassiveSpell>& spell) {
	_passive_spells.push_back(move(spell));
}

void Character::RemoveEffectById(ESpellID effect_id) {
	for (auto it = _effect_ids.begin(); it != _effect_ids.end();)
		if (*it == effect_id)
			it = _effect_ids.erase(it);
		else ++it;
}

void Character::InitStatsPerAttribute() {

	switch (_class) {
	case ECharacterClass::BARBARIAN:
		InitStatsPerAttirbute_Barbarian();
		break;
	case ECharacterClass::WARLOCK:
		InitStatsPerAttribute_Warlock();
		break;
	default:
		break;
	}
}

void Character::InitStatsPerAttirbute_Barbarian() {

	stat_pair stat_vector;

	stat_vector.push_back(make_pair(&_crit_damage, 5.f));
	stat_vector.push_back(make_pair(&_attack_power, 12.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Strength, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_armor, 20.f));
	stat_vector.push_back(make_pair(&_crit_chance, 0.002f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Agility, stat_vector));

	stat_vector.clear();
	//stat_vector.push_back(make_pair(&_spell_crit_chance, 0.00125f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Intelligence, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_health, 10.f));
	stat_vector.push_back(make_pair(&_stamina, 2.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Vitality, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_essence, 5.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Consciousness, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_stamina, 15.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Endurance, stat_vector));
}

void Character::InitStatsPerAttribute_Warlock() {
	stat_pair stat_vector;

	stat_vector.push_back(make_pair(&_crit_damage, 1.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Strength, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_armor, 5.f));
	stat_vector.push_back(make_pair(&_crit_chance, 0.002f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Agility, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_spell_crit_chance, 0.005f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Intelligence, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_health, 8.f));
	stat_vector.push_back(make_pair(&_stamina, 3.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Vitality, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_essence, 15.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Consciousness, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&_stamina, 5.f));
	_stat_per_attribute.push_back(make_pair(&_player_attributes.Endurance, stat_vector));
}

void Character::EndTurn() {
	GameplayStatics::EndTurn(this);
}

bool Character::CheckDie() {
	if (GetHealth().GetActual() <= 0.005f) {
		_bIsAlive = false;
		return true;
	}
	return false;
}

void Character::OnMagicReceivedBegin() {

}

void Character::OnMagicReceivedEnd() {

}

void Character::OnMeleeReceivedBegin() {

}

void Character::OnMeleeReceivedEnd() {
	GetPassiveSpells()[0]->Apply();
}

void Character::OnRangedReceivedBegin() {

}

void Character::OnRangedReceivedEnd() {

}
