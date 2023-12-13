#pragma once
#include "../RPGTypes.h"
#include "../Effects/ActiveSpell.h"
#include "../Effects/PassiveSpell.h"

class Character;
class PlayerCharacter;

class Item {
public:
	struct ItemInfo {
		EItemID			_ID = EItemID::NONE;
		int				_lvl = 0;
		int				_dmg_min = 0;
		int				_dmg_max = 0;
		int				_armor = 0;

		float			_amount = 0.f;

		int				_n_affixes = 0;
		float			_modifier = 0.f;

		bool			_bOutOfCombat = false;
		bool			_bUsable = false;

		std::string		_name = "";

		EItemSlot		_item_slot = EItemSlot::NONE;
		EItemRarity		_item_rarity = EItemRarity::MISC;
		EItemType		_item_type = EItemType::MISC;
		EWeaponType		_weapon_type = EWeaponType::NONE;

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