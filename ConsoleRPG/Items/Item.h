#pragma once
#include "../RPGTypes.h"

class ActiveEffect;
class PassiveEffect;

class Item {
public:
	Item();

	inline const std::unique_ptr<ActiveEffect> GetActiveEffect() const { return _active_effect; }
	inline const std::unique_ptr<PassiveEffect> GetPassiveEffect() const { return _passive_effect; }

protected:
	

	int			_item_level;

	EItemSlot	_item_slot;
	EItemRarity	_item_rarity;

	std::unique_ptr<ActiveEffect> _active_effect;
	std::unique_ptr<PassiveEffect> _passive_effect;


};