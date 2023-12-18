#pragma once

#include "CharacterData.h"
#include "../RPGTypes.h"
#include "../Resistances.h"

using namespace std;

class SpellBook;
class PassiveSpell;
class ActiveSpell;

class Character {

public:

	Character() = delete;

	// Enemy
	Character(const CharacterData::EnemyStats& data, char alias);

	// Player
	Character(const CharacterData::PlayerAttributes& attributes, char alias);

	// Summon
	Character(const CharacterData::SummonData& data, int team, char alias);

	Character(const Character& other);

	Character(Character&& other);

	Character& operator=(const Character& other) {
		// "reset" all stats that effects might modify
		_health.SetMax(other._health.GetMax()); // treba testirati, dali u combatu, nakon bonus gaina, koji povecaju actual, i tako i maximum, da li se maximum restarta na prijasnji nakon bonus expire
		_essence = other._essence;
		_stamina = other._stamina;
		_armor = other._armor;
		_attack_power = other._attack_power;
		_crit_chance = other._crit_chance;
		_crit_damage = other._crit_damage;
		_spell_power = other._spell_power;
		_spell_crit_chance = other._spell_crit_chance;
		_spell_crit_damage = other._spell_crit_damage;
		_resistances = other._resistances;
		return *this;
	}

	virtual ~Character();

	virtual void TakeTurn() = 0;

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

			return _base;	// zakaj base ? prouciti
		}

		inline const float& GetBase() const { return _base; }
		inline float& GetActual() { return _actual; }
		inline const float& GetMax() const { return _max; }

		void UpdateBase(const float value);
		void UpdateActual(const float value, Character* character);
		void UpdateMax(const float value);

		void SetActual(const float value);
		void SetMax(float value);

	private:
		//void OnStatChanged();
	};

protected:

	ECharacterClass	_class;
	
	Stat _health;
	Stat _essence;
	Stat _stamina;

	Stat _armor;
	Stat _attack_power;
	Stat _crit_chance;
	Stat _crit_damage; 
	Stat _spell_power;
	Stat _spell_crit_chance;
	Stat _spell_crit_damage;

	CharacterData::PlayerAttributes	_player_attributes;
	Resistances	_resistances;

	char _alias = 'X';

	int	 _lvl = 1;
	bool _bIsInCombat = false;
	bool _bIsAlive = true;
	bool _bIsOnTurn = false;

	int _team = -1;

public:
	
	inline Stat& GetHealth() { return _health; }
	inline Stat& GetEssence() { return _essence; }
	inline Stat& GetStamina() { return _stamina; }
	inline Stat& GetArmor() { return _armor; }
	inline Stat& GetAP() { return _attack_power; }
	inline Stat& GetCritChance() { return _crit_chance; }
	inline Stat& GetCritDmg() { return _crit_damage; } 
	inline Stat& GetSP() { return _spell_power; }
	inline Stat& GetSpellCritChance() { return _spell_crit_chance; }
	inline Stat& GetSpellCritDmg() { return _spell_crit_damage; }
	inline ECharacterClass GetCharacterClass() { return _class; }

public:
	
	void UpdateAttribute(Attribute& attribute, const int amount);

	void AddActiveSpell(unique_ptr<ActiveSpell>& spell);
	void AddPassiveSpell(unique_ptr<PassiveSpell>& spell);

	inline vector<unique_ptr<ActiveSpell>>& GetActiveSpells() { return _active_spells; }
	inline vector<unique_ptr<PassiveSpell>>& GetPassiveSpells() { return _passive_spells; }

	inline void AddEffectId(ESpellID effect_id) { _effect_ids.push_back(effect_id); }
	void RemoveEffectById(ESpellID effect_id);
	inline const vector<ESpellID>& GetEffectIds() { return _effect_ids; }

public:

	// EXTRA STATS COMBAT
	//////////////////////////////////////////
	// item attributes
	int _i_str = 0;
	int _i_agi = 0;
	int _i_int = 0;
	int _i_vit = 0;
	int _i_con = 0;
	int _i_end = 0;

	int _multi_strike = 0;
	// damage
	float _arcane_damage = 0.f;
	float _fire_damage = 0.f;
	float _lightning_damage = 0.f;
	float _cold_damage = 0.f;
	float _poison_damage = 0.f;
	float _necrotic_damage = 0.f;
	float _physical_damage = 0.f;
	
	int	_min_damage = 0;
	int _max_damage = 0;
	int _avg_damage = 0;

	// healing
	float _healing = 0.f;

	// duration
	int _buff_duration = 0;
	int _debuff_duration = 0;
	/////////////////////////////////////////


	// EXTRA STATS OUT OF COMBAT
	/////////////////////////////////////////
	float _magic_find = 0.f;
	int _light_radius = 0;

	int _n_cons_slots = 0;
	int _n_spell_slots = 0;
	int _n_passive_slots = 0;
	///////////////////////////////////////////

protected:

	// Active spells
	//vector<shared_ptr<SpellBook>> _spellbooks;

	std::vector<unique_ptr<ActiveSpell>> _active_spells;
	std::vector<unique_ptr<PassiveSpell>> _passive_spells;

	// ID's of spell effects
	std::vector<ESpellID> _effect_ids;

	using stat_pair = std::vector<pair<Stat*, float>>;
	// Stat change per attribute
	std::vector<pair<Attribute*, stat_pair>> _stat_per_attribute;

protected:

	void InitStats();

	// Set stat gain / loss per attribute for each class
	void InitStatsPerAttribute();

	void InitStatsPerAttirbute_Barbarian();
	void InitStatsPerAttribute_Warlock();

public:

	void CheckDie();

	void EndTurn();

	inline const char GetAlias() const { return _alias; }
	inline void SetAlias(char value) { _alias = value; }
	inline const int GetLevel() const { return _lvl; }
	inline const bool IsInCombat() const { return _bIsInCombat; }
	inline const bool IsAlive() const { return _bIsAlive; }
	inline const bool IsOnTurn() const { return _bIsOnTurn; }
	inline const int GetTeam() const { return _team; }
	inline void SetIsOnTurn(bool value) { _bIsOnTurn = value; }
	inline void SetIsInCombat(bool in_combat) { _bIsInCombat = in_combat; }
	inline const CharacterData::PlayerAttributes& GetPlayerAttributes() { return _player_attributes; }
	inline Resistances& GetResistances() { return _resistances; }

public:
	void OnMagicReceivedBegin();
	void OnMagicReceivedEnd();
	void OnMeleeReceivedBegin();
	void OnMeleeReceivedEnd();
	void OnRangedReceivedBegin();
	void OnRangedReceivedEnd();
};