#pragma once

#include "../RPGTypes.h"

struct ItemData {
	EItemID			_ID;
	std::string		_name;
	EItemType		_item_type;
	EWeaponType		_wpn_type;
	EItemSlot		_slot;
	ESpellID		_effect_id;
	int				_min_lvl;
	int				_max_lvl;
	int				_min_dmg;
	int				_max_dmg;
	int				_armor;
	float			_value;

	//armor
	int				_spell_slots;	//Hands = slots for spells
	float			_mag_dmg; 		//Legs = magic_damage
	int				_cons_slots; 	//Belt = slots for consumables
	float			_mag_res;		//Feet = magic_resistance

	//weapon
	float			_extra_dmg;		// Bow extra random magic damage
	float			_spell_c_dmg;	// Staff spell_crit_dmg bonus
	float			_armor_pen;		// Axe 2h & 1h armor penetration amount
	float			_crit_dmg;		// mace 2h crit_dmg bonus
	float			_double_chnc;   // Sword 2h & 1h double swing chance
	float			_crit_chnc;     // mace 1h crit_chance & spell_crit_chance bonus
	float			_bleed_chnc;	// dagger bleed_chance  (if equipped while casting a spell, bleed damage is based on spell damage)
	float			_spell_res_dec;	// Orb(offhand) spell_resistance decrease
	float			_block_val;		// shield amount % blocked

	int				_drop_chnc;
	bool			_bNoCombat;
	bool			_bUsable;
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