#pragma once

#include "../RPGTypes.h"

struct ItemData {
	EItemID			Id;
	std::string		Name;
	EItemType		ItemType;
	EWeaponType		WeaponType;
	EItemSlot		ItemSlot;
	ESpellID		EffectId;
	int				MinLevel;
	int				MaxLevel;
	int				MinDmg;
	int				MaxDmg;
	int				Armor;
	float			Amount;

	//armor
	//Hands = slots for spells
	//Belt = slots for consumables
	int				Slots;

	//Legs = magic_damage
	//Feet = magic_resistance
	float			ArmorMod;

	//weapon
	// Bow extra random magic damage
	// Staff spell_crit_dmg bonus
	// Axe 2h & 1h armor penetration amount
	// mace 2h crit_dmg bonus
	// Sword 2h & 1h double swing chance
	// mace 1h crit_chance & spell_crit_chance bonus
	// dagger bleed_chance  (if equipped while casting a spell, bleed damage is based on spell damage)
	// Orb(offhand) spell_resistance decrease
	// shield amount % blocked
	float			WeaponMod;

	int				DropChance;
};

class ItemDb {
public:
	ItemDb() = delete;
	static std::vector<ItemData> Data;
	
private:
	static struct ItemDataConstructor {
		ItemDataConstructor();
	} ItemData;
};
