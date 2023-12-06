#pragma once
#include "../RPGTypes.h"
#include "../Characters/PlayerCharacter.h"

class ActiveSpell;
class PassiveSpell;
class Character;

class Item {
public:
	struct ItemInfo {
		EItemID			_ID;
		int				_lvl = 0;
		int				_dmg_min = 0;
		int				_dmg_max = 0;
		int				_armor = 0;

		float				_amount;

		int				_n_affixes;
		float			_modifier;

		bool			_bOutOfCombat = false;
		bool			_bUsable = false;

		std::string		_name;

		EItemSlot		_item_slot;
		EItemRarity		_item_rarity;
		EItemType		_item_type;
		EWeaponType		_weapon_type;

		//std::unique_ptr<ActiveSpell> _active_effect;
		//std::unique_ptr<PassiveSpell> _passive_effect;
	};

	Item(ItemInfo item_info);

	static std::vector<std::unique_ptr<Item>> GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl);
	static std::unique_ptr<Item> CreateItem(int player_lvl, float mf_bonus, EItemType item_type);

	void Use(Character* character);
public:

	ItemInfo _item_info;

private:

	static ItemInfo GenerateItemInfo(int item_lvl, EItemType item_type, EItemRarity item_rarity);
	static void CalcItemDamage(int item_lvl, EWeaponType weapon_type, OUT int& min_dmg, OUT int& max_dmg);
	static void CalcItemArmor(int item_lvl, EItemSlot item_slot, OUT int& armor);
	static void GenerateRndConsumable(ItemInfo& item_info, EItemRarity item_rarity);
};