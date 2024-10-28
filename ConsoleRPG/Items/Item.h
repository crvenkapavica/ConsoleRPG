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
	struct ItemProperties {
		EItemId Id = EItemId::NONE;
		int Level = 0;
		int MinDmg = 0;
		int MaxDmg = 0;
		int Armor = 0;

		float Amount = 0.f;

		int nAffixes = 0;

		int Slots;
		float ArmorMod = 0.f;
		float WeaponMod = 0.f;

		bool bUsable = false;
		bool bUsableMap = false;

		std::string Name;

		EItemSlot ItemSlot = EItemSlot::NONE;
		EItemRarity ItemRarity = EItemRarity::MISC;
		EItemType ItemType = EItemType::MISC;
		EWeaponType WeaponType = EWeaponType::NONE;

		ESpellID Active = ESpellID::NONE;
		ESpellID Passive = ESpellID::NONE;

		EItemAffix Affix1 = EItemAffix::NONE;
		EItemAffix Affix2 = EItemAffix::NONE;
		EItemAffix Affix3 = EItemAffix::NONE;
		EItemAffix Affix4 = EItemAffix::NONE;
	};

	explicit Item(const ItemProperties& ItemInfo);

	explicit Item(ItemProperties&& ItemInfo);

	// This will only be used for starting items [pre-determined]
	// This will MAYBE be used for UNIQUE-s [pre-determined]
	explicit Item(const ItemData& Data);

	// Calls CreateItem multiple times depending on power_Level of slain monsters and random roll chance.
	static std::vector<std::unique_ptr<Item>> GenerateLoot(const std::weak_ptr<PlayerCharacter>& Player, const int PowerLevel);

	// Creates RANDOM item from BASE items + prefixes(max 1 - passive spell), suffixes(max 1 - active spell) and affixes(all other modifiers).
	static std::unique_ptr<Item> CreateItem(const int PlayerLevel, const float MfBonus, const EItemType ItemType);

	// Used exclusively for starting pre-determined items for each class and enemy.
	// Might be used later for unique items (which are also pre-determined).
	static std::unique_ptr<Item> CreateItemById(const EItemId Id);

	// Use the consumable, scroll, or an active spell of an item.
	void Use(Character* Character) const;

public:
	ItemProperties ItemInfo;

private:
	static ItemProperties GenerateItemInfo(const int PlayerLevel, const EItemType ItemType, const EItemRarity ItemRarity);

	// Calculates ILevel depending on player_Level and number of affixes(rarity)
	static int CalculateItemLevel(const int PlayerLevel, const int nAffixes);

	// Generates an item slot depending on the item type
	static void GenerateItemSlot(ItemProperties& ItemInfo);

	// Gets the BASE item with base stats and its slot-dependent modifier and then rolls affixes on top of it depending on the rarity
	static void GetBaseItem(ItemProperties& ItemInfo);

	// Rolls Affixes. Number increases with rarity. Also Changes names if active / passive affix was rolled.
	static void RollAffixes(ItemProperties& ItemInfo);

	// Further randoms the damage with ILevel and rarity formula
	static void CalculateItemDamage(ItemProperties& ItemInfo);

	// Further randoms the armor with ILevel and rarity formula
	static void CalculateItemArmor(ItemProperties& ItemInfo);

	// Generate a random consumable from a consumable pool (might be usable in map or not)
	static void GenerateRandomConsumable(ItemProperties& ItemInfo);

	// Generate a random scroll of any castable Active Spell (+ maybe some exclusive SCROLL SPELLS)
	static void GenerateRandomScroll(ItemProperties& ItemInfo);
};