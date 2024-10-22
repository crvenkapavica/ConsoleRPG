#pragma once

#include "ItemData.h"
#include "../RPGTypes.h"
#include "../Spells/ActiveSpell.h"
#include "../Spells/ActiveSpell.h"
#include "../Spells/PassiveSpell.h"
#include "../Spells/PassiveSpell.h"

class Character;
class PlayerCharacter;

class Item {
public:
	struct ItemInfo {
		EItemID			Id = EItemID::NONE;
		int				Lvl = 0;
		int				MinDmg = 0;
		int				MaxDmg = 0;
		int				Armor = 0;

		float			Amount = 0.f;

		int				nAffixes = 0;

		int				Slots;
		float			ArmorMod = 0.f;
		float			WeaponMod = 0.f;

		bool			bUsable = false;
		bool			bUsableMap = false;

		std::string		Name;

		EItemSlot		ItemSlot = EItemSlot::NONE;
		EItemRarity		ItemRarity = EItemRarity::MISC;
		EItemType		ItemType = EItemType::MISC;
		EWeaponType		WeaponType = EWeaponType::NONE;

		ESpellID		Active = ESpellID::NONE;
		ESpellID		Passive = ESpellID::NONE;
		
		EItemAffix		Affix1 = EItemAffix::NONE;
		EItemAffix		Affix2 = EItemAffix::NONE;
		EItemAffix		Affix3 = EItemAffix::NONE;
		EItemAffix		Affix4 = EItemAffix::NONE;
	};

	explicit Item(ItemInfo item_info);

	// This will only be used for starting items [pre-determined]
	// This will MAYBE be used for UNIQUE-s [pre-determined]
	explicit Item(const ItemData& data);

	// Calls CreateItem multiple times depending on power_lvl of slain monsters and random roll chance.
	static std::vector<std::unique_ptr<Item>> GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl);
	
	// Creates RANDOM item from BASE items + prefixes(max 1 - passive spell), suffixes(max 1 - active spell) and affixes(all other modifiers).
	static std::unique_ptr<Item> CreateItem(int player_lvl, float mf_bonus, EItemType item_type);

	// Used exclusively for starting pre-determined items for each class and enemy.
	// Might be used later for unique items (which are also pre-determined).
	static std::unique_ptr<Item> CreateItemById(EItemID id);

	// Use the consumable, scroll, or an active spell of an item.
	void Use(Character* character);

public:
	ItemInfo ItemInfo;

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