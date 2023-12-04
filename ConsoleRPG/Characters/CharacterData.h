#pragma once

#include "../RPGTypes.h"
#include "../Resistances.h"

using namespace std;

class CharacterData {

public:

	struct PlayerStats {

		ECharacterClass	_class;
		float _health;
		float _essence;
		float _stamina;
		float _armor;

		int   _attack_power;
		float _crit_chance;
		float _crit_damage;

		int   _spell_power;
		float _spell_crit_chance;
		float _spell_crit_damage;
	};

	struct EnemyStats {

		ECharacterClass	_class;
		float _health;
		float _essence;
		float _stamina;
		float _armor;
		
		int	  _attack_power;
		float _crit_chance;
		float _crit_damage;

		int	  _spell_power;
		float _spell_crit_chance;
		float _spell_crit_damage;

		Resistances _resistances;

		pair<ESpellID, int> _spell1;
		pair<ESpellID, int> _spell2;
		pair<ESpellID, int> _spell3;
		pair<ESpellID, int> _spell4;
	};

	struct PlayerAttributes {	

		ECharacterClass	_class;

		Attribute		_strength;		// increase AP
		Attribute		_agility;		// increase AP
		Attribute		_intelligence;  // increase SP

		Attribute		_vitality;		// increase health
		Attribute		_consciousness;	// increase essence
		Attribute		_endurance;		// inrease stamina
	};

	CharacterData(ECharacterClass character_class);

protected:

	PlayerStats			_player_data;
	EnemyStats			_enemy_data;
	PlayerAttributes	_player_attribute_data;

	using stat_pair = vector<pair<Stat*, float>>;
	// Stat change per attribute
	vector<pair<Attribute*, stat_pair>> _stat_per_attribute;

private:

	PlayerAttributes InitPlayerCharacterAttributes(ECharacterClass player_class);

	PlayerStats InitPlayerCharacterStats(ECharacterClass player_class);

	EnemyStats InitEnemyCharacterStats(ECharacterClass enemy_class);

	// Set stat gain / loss per attribute for each class
	void InitStatsPerAttribute(ECharacterClass character_class);

	void InitStatsPerAttirbute_Barbarian();

public:

	inline const PlayerStats& GetPlayerData() const { return _player_data; }
	inline const EnemyStats& GetEnemyData() const { return _enemy_data; }
	
	PlayerAttributes& GetPlayerAttributeData() { return _player_attribute_data; }
};


//Skill		_craftmanship; // kraftanje i repejranje itemi - efekt repair itema - efekt craftaj bez towna (recimo)
//Skill		_perception; // odnosi se na uocavanje detalja, mores videti enemy power level sa vecom preciznoscu (efekt INSPECT)
//Skill		_acrobatics; // odnosi se na preskakanje neksih prepreki na mapi i generalni movement (efekt skoci prek kamena, vise se kreci)
//Skill		_pathfinding; // utjece na to dal mores mapu videti (pamti de je kaj na mapi i pamti mapu, morres videti mapu zbog toga skilla)