#pragma once

#include "../RPGTypes.h"

struct ItemData {
	EItemID			_ID;
	std::string		_name;
	EItemType		_item_type;
	EWeaponType		_wpn_type;
	EItemSlot		_slot;
	EEffectID		_effect_id;
	int				_min_lvl;
	int				_max_lvl;
	int				_min_dmg;
	int				_max_dmg;
	int				_armor;
	float			_value;
	int				_drop_chance;
	bool			_bNoCombat;
};

class ItemDatabase {
public:

	ItemDatabase() {}
	static std::vector<ItemData> _item_data;
	static struct ItemDataConstructor {
		ItemDataConstructor();
	} _item_data_constructor;
};
