#pragma once
#include "../RPGTypes.h"

class ActiveEffect;
class PassiveEffect;
class PlayerCharacter;

class Item {
public:
	struct ItemInfo {
		int				_item_lvl;
		int				_item_dmg_min;
		int				_item_dmg_max;
		int				_item_armor;

		std::string		_item_name;

		EItemSlot		_item_slot;
		EItemRarity		_item_rarity;
		EItemType		_item_type;
		EWeaponType		_weapon_type;
	};

	Item(ItemInfo& item_info);

	static std::vector<std::shared_ptr<Item>> GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl);
	static std::shared_ptr<Item> CreateItem(int player_lvl, float mf_bonus, EItemType item_type);
	void GenerateItemInfo(EItemType item_type, EItemRarity item_rarity);

	const int GetItemLevel() const { return _item_lvl; }

	inline const std::unique_ptr<ActiveEffect>& GetActiveEffect() const { return _active_effect; }
	inline const std::unique_ptr<PassiveEffect>& GetPassiveEffect() const { return _passive_effect; }

protected:
	ItemInfo		_item_info;

	std::unique_ptr<ActiveEffect> _active_effect;
	std::unique_ptr<PassiveEffect> _passive_effect;
};