#pragma once

#include "../RPGTypes.h"
#include "../Resistances.h"

struct CharacterData {
	ECharacterClass CharacterClass;

	float Health;
	float Essence;
	float Stamina;
	float Armor;

	float AttackPower;
	float CritChance;
	float CritDamage;

	float SpellPower;
	float SpellCritChance;
	float SpellCritDamage;

	EDamageType	DamageType;

	Resistances Resistances;

	std::pair<ESpellID, int> Spell1;
	std::pair<ESpellID, int> Spell2;
	std::pair<ESpellID, int> Spell3;
	std::pair<ESpellID, int> Spell4;

	std::pair<ESpellID, int> Passive1;
	std::pair<ESpellID, int> Passive2;

	std::pair<EItemSlot, EItemId> SlotMainHand;
	std::pair<EItemSlot, EItemId> SlotOffHand;

	int PowerLevel;
};

struct PlayerAttributes {
	attribute		Strength;
	attribute		Agility;
	attribute		Intelligence;

	attribute		Vitality;
	attribute		Consciousness;
	attribute		Endurance;
};

class CharDb {
public:
	CharDb() = delete;
	static std::unordered_map<ECharacterClass, CharacterData> Data;
	static std::unordered_map<ECharacterClass, PlayerAttributes> Attributes;
	
private:
	static struct CharacterDataConstructor {
		CharacterDataConstructor();
	} CharacterData;
	static struct PlayerAttributesConstructor {
		PlayerAttributesConstructor();
	} PlayerAttributes;
};


//Skill		_craftmanship; // kraftanje i repejranje itemi - efekt repair itema - efekt craftaj bez towna (recimo)
//Skill		_perception; // odnosi se na uocavanje detalja, mores videti enemy power level sa vecom preciznoscu (efekt INSPECT)
//Skill		_acrobatics; // odnosi se na preskakanje neksih prepreki na mapi i generalni movement (efekt skoci prek kamena, vise se kreci)
//Skill		_pathfinding; // utjece na to dal mores mapu videti (pamti de je kaj na mapi i pamti mapu, morres videti mapu zbog toga skilla)