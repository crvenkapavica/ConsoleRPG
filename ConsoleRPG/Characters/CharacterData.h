#pragma once

#include "../RPGTypes.h"
#include "../Resistances.h"

struct CharacterData {
	float _health;
	float _essence;
	float _stamina;
	float _armor;

	float _attack_power;
	float _crit_chance;
	float _crit_damage;

	float _spell_power;
	float _spell_crit_chance;
	float _spell_crit_damage;

	EDamageType	_damage_type;

	Resistances _resistances;

	std::pair<ESpellID, int> _spell1;
	std::pair<ESpellID, int> _spell2;
	std::pair<ESpellID, int> _spell3;
	std::pair<ESpellID, int> _spell4;

	std::pair<ESpellID, int> _passive1;
	std::pair<ESpellID, int> _passive2;

	std::pair<EItemSlot, EItemID> slot_mh;
	std::pair<EItemSlot, EItemID> slot_oh;
};

struct PlayerAttributes {
	Attribute		_strength;
	Attribute		_agility;
	Attribute		_intelligence;

	Attribute		_vitality;
	Attribute		_consciousness;
	Attribute		_endurance;
};

class CharDB {
public:
	static std::unordered_map<ECharacterClass, CharacterData> _data;
	static std::unordered_map<ECharacterClass, PlayerAttributes> _attributes;
private:
	CharDB() = delete;

	static struct CharacterDataConstructor {
		CharacterDataConstructor();
	} _character_data_constructor;
	static struct PlayerAttributesConstructor {
		PlayerAttributesConstructor();
	} _player_attributes_constructor;
};


//Skill		_craftmanship; // kraftanje i repejranje itemi - efekt repair itema - efekt craftaj bez towna (recimo)
//Skill		_perception; // odnosi se na uocavanje detalja, mores videti enemy power level sa vecom preciznoscu (efekt INSPECT)
//Skill		_acrobatics; // odnosi se na preskakanje neksih prepreki na mapi i generalni movement (efekt skoci prek kamena, vise se kreci)
//Skill		_pathfinding; // utjece na to dal mores mapu videti (pamti de je kaj na mapi i pamti mapu, morres videti mapu zbog toga skilla)