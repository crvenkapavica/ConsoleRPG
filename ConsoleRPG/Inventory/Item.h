#pragma once
#include "../RPGTypes.h"
#include "../Characters/PlayerCharacter.h"

class ActiveEffect;
class PassiveEffect;

class Item {
public:
	struct ItemInfo {
		int				_lvl = 0;
		int				_dmg_min = 0;
		int				_dmg_max = 0;
		int				_armor = 0;

		bool			_bOutOfCombat = false;

		std::string		_name;

		EItemSlot		_item_slot;
		EItemRarity		_item_rarity;
		EItemType		_item_type;
		EWeaponType		_weapon_type;

		//std::unique_ptr<ActiveEffect> _active_effect;
		//std::unique_ptr<PassiveEffect> _passive_effect;
	};

	Item(ItemInfo item_info);

	static std::vector<std::unique_ptr<Item>> GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl);
	static std::unique_ptr<Item> CreateItem(int player_lvl, float mf_bonus, EItemType item_type);

public:

	ItemInfo _item_info;

private:

	static ItemInfo GenerateItemInfo(int item_lvl, EItemType item_type, EItemRarity item_rarity);
	static void CalcItemDamage(int item_lvl, EWeaponType weapon_type, OUT int& min_dmg, OUT int& max_dmg);
	static void CalcItemArmor(int item_lvl, EItemSlot item_slot, OUT int& armor);
	static void GenerateRndConsumable(EItemRarity item_rarity);
};