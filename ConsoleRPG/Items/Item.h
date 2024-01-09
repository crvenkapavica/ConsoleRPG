#pragma once

#include "../RPGTypes.h"
#include "../Spells/ActiveSpell.h"
#include "../Spells/PassiveSpell.h"
#include "ItemData.h"
#include "../Spells/ActiveSpell.h"
#include "../Spells/PassiveSpell.h"

class Character;
class PlayerCharacter;

class Item {
public:
	struct ItemInfo {
		EItemID			_ID = EItemID::NONE;
		int				_lvl = 0;
		int				_min_dmg = 0;
		int				_max_dmg = 0;
		int				_armor = 0;

		float			_amount = 0.f;

		int				_n_affixes = 0;

		int				_slots;
		float			_armor_mod = 0.f;
		float			_wpn_mod = 0.f;

		bool			_bUsableMap = false;
		bool			_bUsable = false;

		std::string		_name = "";

		EItemSlot		_item_slot = EItemSlot::NONE;
		EItemRarity		_item_rarity = EItemRarity::MISC;
		EItemType		_item_type = EItemType::MISC;
		EWeaponType		_wpn_type = EWeaponType::NONE;

		ESpellID		_active = ESpellID::NONE;
		ESpellID		_passive1 = ESpellID::NONE;
		ESpellID		_passive2 = ESpellID::NONE;
		ESpellID		_passive3 = ESpellID::NONE;

		//std::unique_ptr<ActiveSpell> _active_effect;
		//std::unique_ptr<PassiveSpell> _passive_effect1;
		//std::unique_ptr<PassiveSpell> _passive_effect2;
		//std::unique_ptr<PassiveSpell> _passive_effect3;
	};

	Item(ItemInfo item_info);
	Item(const ItemData& data);

	static std::vector<std::unique_ptr<Item>> GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl);
	static std::unique_ptr<Item> CreateItem(int player_lvl, float mf_bonus, EItemType item_type);
	static std::unique_ptr<Item> GetItemByID(EItemID id);

	void Use(Character* character);

public:
	ItemInfo _item_info;

private:
	static ItemInfo GenerateItemInfo(int&& item_lvl, EItemType item_type, EItemRarity item_rarity);

	static void GetBaseItem(ItemInfo& item_info);

	static int CalcItemLvl(int player_lvl, int n_affixes);

	static void CalcItemDamage(ItemInfo& item_info);

	static void CalcItemArmor(ItemInfo& item_info);

	static void GenerateRndConsumable(ItemInfo& item_info);

	static void GenerateRndScroll(ItemInfo& item_info);

	static void GenerateItemName(ItemInfo& item_info);
};