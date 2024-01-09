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
		float			_arm_mod = 0.f;
		float			_wpn_mod = 0.f;

		bool			_bUsable = false;
		bool			_bUsableMap = false;

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

	// This will only be used for starting items [pre-determined]
	// This will MAYBE be used for UNIQUEs [pre-determined]
	Item(const ItemData& data);

	// Calls CreateItem multiple times depending on power_lvl of slain monsters and random roll chance.
	static std::vector<std::unique_ptr<Item>> GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl);
	
	// Creates RANDOM item from BASE items + prefixes(max 1 - passive spell), suffixes(max 1 - active spell) and affixes(all other stat modifiers).
	static std::unique_ptr<Item> CreateItem(int player_lvl, float mf_bonus, EItemType item_type);

	// Used exclusively for starting pre-determined items for each class and enemy.
	// might be used later for unique items (which are also pre-determined).
	static std::unique_ptr<Item> CreateItemByID(EItemID id);

	// Use the consumable, scroll, or an active spell of an item.
	void Use(Character* character);

public:
	ItemInfo _item_info;

private:
	static ItemInfo GenerateItemInfo(int player_lvl, EItemType item_type, EItemRarity item_rarity);

	// Calculates Ilvl depending on player_lvl and number of affixes(rarity)
	static int CalcItemLvl(int player_lvl, int n_affixes);
	
	// Generates an item slot depending on the item type
	static void GenerateItemSlot(ItemInfo& item_info);

	// Gets the BASE item with base stats and its slot-dependent modifier and then rolls affixes on top of it depending on the rarity
	static void GetBaseItem(ItemInfo& item_info);

	// Rolls Affixes. Number increases with rarity. Also Changes names if active / pasive affix was rolled.
	static void RollAffixes(ItemInfo& item_info);

	// Further randoms the damage with Ilvl and rarity formula
	static void CalcItemDamage(ItemInfo& item_info);

	// Further randoms the armor with Ilvl and rarity formula
	static void CalcItemArmor(ItemInfo& item_info);

	// Generate a random consumable from a consumable pool (might be usable in map or not)
	static void GenerateRndConsumable(ItemInfo& item_info);
	
	// Generate a random scroll of any castable Active Spell (+ maybe some exclusive SCROLL SPELLS)
	static void GenerateRndScroll(ItemInfo& item_info);
};