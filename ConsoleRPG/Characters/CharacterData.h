#pragma once

#include "../RPGTypes.h"
#include "../Resistances.h"

using namespace std;

class CharacterData {

public:

	struct EnemyStats {

		ECharacterClass	_class;
		float _health;
		float _essence;
		float _stamina;
		float _armor;
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

		Attribute		_vitality;
		Attribute		_consciousness;
		Attribute		_endurance;
	};

	CharacterData(ECharacterClass character_class);

protected:

	EnemyStats			_enemy_data;
	PlayerAttributes	_player_attribute_data;

private:

	PlayerAttributes InitPlayerCharacterAttributes(ECharacterClass player_class);
	EnemyStats InitEnemyCharacterStats(ECharacterClass enemy_class);

public:

	inline const EnemyStats& GetEnemyData() const { return _enemy_data; }
	
	PlayerAttributes& GetPlayerAttributeData() { return _player_attribute_data; }
};


//Skill		_craftmanship; // kraftanje i repejranje itemi - efekt repair itema - efekt craftaj bez towna (recimo)
//Skill		_perception; // odnosi se na uocavanje detalja, mores videti enemy power level sa vecom preciznoscu (efekt INSPECT)
//Skill		_acrobatics; // odnosi se na preskakanje neksih prepreki na mapi i generalni movement (efekt skoci prek kamena, vise se kreci)
//Skill		_pathfinding; // utjece na to dal mores mapu videti (pamti de je kaj na mapi i pamti mapu, morres videti mapu zbog toga skilla)