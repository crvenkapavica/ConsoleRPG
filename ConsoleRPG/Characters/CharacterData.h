#pragma once

#include "../RPGTypes.h"
#include "../Resistances.h"

using namespace std;

class CharacterData {

public:

	CharacterData(ECharacterClass character_class);

	struct PlayerStats {

		ECharacterClass	_class;
		float _health;
		float _essence;
		float _stamina;
		float _armor;
		float _attack_speed;
		float _damage_melee;
		float _damage_ranged;
		float _crit_chance;
		float _crit_damage;
		float _spell_crit_chance;
		float _spell_crit_damage;
		float _magic_find;
		float _light_radius;
	};

	struct EnemyStats {

		ECharacterClass	_class;
		float _health;
		float _essence;
		float _stamina;
		float _armor;
		float _attack_speed;
		float _damage_melee;
		float _damage_ranged;
		float _crit_chance;
		float _crit_damage;
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

		Attribute		_strength;
		Attribute		_agility;
		Attribute		_intelligence;
		Attribute		_consciousness;
		Attribute		_vitality;
		
		//Attribute		_endurance // ili nest slicno, mogel bi biti attribut kolko mores koristiti "powera" po turnu. 
								  // power je attack, spell, items usage, sve koristi nekse resursa 
								//(mozda istoga ko i spell kak magija mozda budu svi skupa kao neksi power i mores tolko potrositi na turnu pa treba planirati)
								
		// SKillovi dajeju efekte pasivne ili aktivne
		// moreju imati recimo 3 levela
		// jako su jako i tesko ih je zleveleati
		Skill		_craftmanship; // kraftanje i repejranje itemi - efekt repair itema - efekt craftaj bez towna (recimo)
		Skill		_perception; // odnosi se na uocavanje detalja, mores videti enemy power level sa vecom preciznoscu (efekt INSPECT)
		Skill		_acrobatics; // odnosi se na preskakanje neksih prepreki na mapi i generalni movement (efekt skoci prek kamena, vise se kreci)
		Skill		_pathfinding; // utjece na to dal mores mapu videti (pamti de je kaj na mapi i pamti mapu, morres videti mapu zbog toga skilla)
	};

protected:

	PlayerStats			_player_data;
	EnemyStats			_enemy_data;
	PlayerAttributes	_player_attribute_data;

private:

	PlayerAttributes InitPlayerCharacterAttributes(ECharacterClass player_class);

	PlayerStats InitPlayerCharacterStats(ECharacterClass player_class);

	EnemyStats InitEnemyCharacterStats(ECharacterClass enemy_class);

public:

	inline const PlayerStats& GetPlayerData() const { return _player_data; }
	inline const EnemyStats& GetEnemyData() const { return _enemy_data; }
	
	PlayerAttributes& GetPlayerAttributeData() { return _player_attribute_data; }
};