#pragma once

#include "CharacterData.h"
#include "../RPGTypes.h"
#include "../Resistances.h"

using namespace std;

class Spell;
class PassiveEffect;

class Character {

public:

	Character(const CharacterData::EnemyStats& data);

	Character(const CharacterData::PlayerAttributes& attributes);

	Character(const Character& other);

	Character& operator=(const Character& other) {
		// "reset" all stats that effects might modify
		_essence = other._essence;
		_stamina = other._stamina;
		_armor = other._armor;
		_damage_melee = other._damage_melee;
		_damage_ranged = other._damage_ranged;
		_crit_chance = other._crit_chance;
		_crit_damage = other._crit_damage;
		_spell_crit_chance = other._spell_crit_chance;
		_spell_crit_damage = other._spell_crit_damage;
		_resistances = other._resistances;
		return *this;
	}

	virtual ~Character();

public:

	class Stat {

		float _base;
		float _actual;
		float _max;

	public:
		Stat()
			: _base(0.f)
			, _actual(0.f)
			, _max(0.f)
		{}

		const float& operator=(const float& value) {
			_base = value;
			_actual = value;
			_max = value;

			return _base;
		}

		inline const float& GetBase() const { return _base; }
		inline const float& GetActual() const { return _actual; }
		inline const float& GetMax() const { return _max; }

		void UpdateBase(const float value);
		void UpdateActual(const float value, Character* character);

		void SetActual(const float value);

	private:
		//void OnStatChanged();
	};

protected:

	ECharacterClass	_class;
	
	Stat _health;
	Stat _essence;
	Stat _stamina;

	Stat _armor;
	Stat _damage_melee;
	Stat _damage_ranged;
	Stat _crit_chance;
	Stat _crit_damage; 
	Stat _spell_crit_chance;
	Stat _spell_crit_damage;

	CharacterData::PlayerAttributes	_player_attributes;
	Resistances	_resistances;

	char _alias = 'X';

	int	 _level = 1;
	bool _bIsInCombat = false;
	bool _bIsAlive = true;
	bool _bIsOnTurn = false;


	// EXTRA STATS
	/////////////////////////////
	int _multi_strike = 0;
	float _fire_damage = 0.f;
	int _buff_duration = 0;
	int _debuff_duration = 0;
	float _magic_find = 0.f;
	int _light_radius = 0;
	////////////////////////////////////////////////

public:
	
	inline Stat& GetHealth() { return _health; }
	inline Stat& GetEssence() { return _essence; }
	inline Stat& GetStamina() { return _stamina; }
	inline Stat& GetArmor() { return _armor; }
	inline Stat& GetDmgMelee() { return _damage_melee; }
	inline Stat& GetDmgRanged() { return _damage_ranged; }
	inline Stat& GetCritChance() { return _crit_chance; }
	inline Stat& GetCritDmg() { return _crit_damage; }
	inline Stat& GetSpellCritChance() { return _spell_crit_chance; }
	inline Stat& GetSpellCrtDmg() { return _spell_crit_damage; }
	inline ECharacterClass GetCharacterClass() { return _class; }

public:
	
	// Get StatPerAttribute vector for updating stats after attribute change
	const auto GetStatPerAttribute() const { return _stat_per_attribute; }

	void UpdateAttribute(Attribute& attribute, const int amount);

	void AddSpell(shared_ptr<Spell> spell);

	void AddPassive(shared_ptr<PassiveEffect> spell);

	inline vector<shared_ptr<Spell>>& GetSpells() { return _spells; }
	inline vector<shared_ptr<PassiveEffect>>& GetPassives() { return _passives; }
	inline const vector<EEffectID>& GetEffectIds() { return _effect_ids; }
	inline void RemoveEffectById(const EEffectID effect_id) {
		for (auto it = _effect_ids.begin(); it != _effect_ids.end();)
			if (*it == effect_id)
				it = _effect_ids.erase(it);
			else ++it;
	}
	inline void AddEffectId(const EEffectID effect_id) { _effect_ids.push_back(effect_id); }

	virtual void TakeTurn() = 0;

protected:

	// Active spells
	vector<shared_ptr<Spell>> _spells;
	// Passive effects
	vector<shared_ptr<PassiveEffect>> _passives;

	// ID's of spell effects
	vector<EEffectID> _effect_ids;

	using stat_pair = vector<pair<Stat*, float>>;
	// Stat change per attribute
	vector<pair<Attribute*, stat_pair>> _stat_per_attribute;

protected:

	void Die();

	void InitStats();

	// Set stat gain / loss per attribute for each class
	void InitStatsPerAttribute();

	void InitStatsPerAttirbute_Barbarian();

public:

	void EndTurn();

	inline const char GetAlias() const { return _alias; }
	inline void SetAlias(char value) { _alias = value; }
	inline const int GetLevel() const { return _level; }
	inline const bool IsInCombat() const { return _bIsInCombat; }
	inline const bool IsAlive() const { return _bIsAlive; }
	inline const bool IsOnTurn() const { return _bIsOnTurn; }
	inline void SetIsOnTurn(bool value) { _bIsOnTurn = value; }
	inline void SetIsInCombat(bool in_combat) { _bIsInCombat = in_combat; }
	inline const CharacterData::PlayerAttributes& GetPlayerAttributes() { return _player_attributes; }
	inline Resistances& GetResistances() { return _resistances; }

	inline const int GetMultiStrike() const { return _multi_strike; }
};