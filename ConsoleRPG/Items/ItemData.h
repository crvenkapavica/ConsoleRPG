#pragma once

#include "../RPGTypes.h"

struct ItemData {
	EItemID			_ID;
	std::string		_name;
	EItemType		_item_type;
	EWeaponType		_wpn_type;
	EItemSlot		_item_slot;
	ESpellID		_effect_id;
	int				_min_lvl;
	int				_max_lvl;
	int				_min_dmg;
	int				_max_dmg;
	int				_armor;
	float			_amount;

	//armor
	//Hands = slots for spells
	//Belt = slots for consumables
	int				_slots;

	//Legs = magic_damage
	//Feet = magic_resistance
	float			_arm_mod;

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
	float			_wpn_mod;

	int				_drop_chnc;
};

class ItemDB {
public:
	static std::vector<ItemData> _data;

private:
	ItemDB() = delete;

	static struct ItemDataConstructor {
		ItemDataConstructor();
	} _item_data_constructor;
};
