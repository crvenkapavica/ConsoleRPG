#include "Item.h"
#include "ItemData.h"
#include "../GameplayStatics.h"
#include "../Items/ItemData.h"
#include "../Characters/Character.h"
#include "../Characters/PlayerCharacter.h"
#include "../Spells/SpellData.h"

namespace
{
	std::vector<std::pair<EItemType, std::pair<int, double>>> DropTable_ItemType{
		{ EItemType::RELIC,{ 45,0.02 } },
		{ EItemType::WEAPON,{ 28,0.10 } },
		{ EItemType::JEWELLERY,{ 22,0.15 } },
		{ EItemType::ARMOR,{ 15,0.25 } },
		{ EItemType::SCROLL,{ 6,0.35 } },
		{ EItemType::CONSUMABLE,{ 2,0.45 } }
	};

	std::vector<std::pair<EItemRarity, double>> DropTable_ItemRarity{
		{ EItemRarity::UNIQUE,0.005 },
		{ EItemRarity::GODLIKE,0.025 },
		{ EItemRarity::LEGENDARY,0.05 },
		{ EItemRarity::EPIC,0.12 },
		{ EItemRarity::RARE,0.25 },
		{ EItemRarity::COMMON,1 }
	};
}

//Consumable
//Health potion
//essence potion
//elixir of fortitude (armor)
//elixir of strength
//elixir of dexterity
//rune of fire magic (increase fire magic) out of combat
//rune of ...all types of magic out of combat
//rune of wizardy (increase all types of magic power) out of combat
//stone of sharpening (increase wpn dmg) ooc
//stone of precission (increase ranged critical chance) ooc
// -> nema unique
//string name, double drop_chance, item_ID


//scroll
// scroll of [ACTIVE_EFFECT] ( spell_level = player-Level + rarity )
// -> nema unique
//string name, double drop_chance(koji ovisi o spell_rarity), item_ID

//ARMOR
// name = sufix + base + prefix
// base = item_Level_range
// rarity = n_affixes
// string base_name, iLevel-drop-range (int, int), item_ID

// Armor_Base = Chest_Armor / 8
// Weapon_Base = Bow_Damage 10-20 % above 2H
// 1H DMG = 0.34 x 2H DMG

/////////////////ARMOR SPECIAL EFFECTS////////////////////////////
// 
// // Head - ability level +
// 
//Hands = slots for spells
//Legs = magic_damage
//Belt = slots for consumables
//Feet = magic_resistance


//SpellType = MAGIC | ABILITY


//ITEMS
//mace = min_max dmg skoro isti
//sword = min_max dmg veliki range
//axe = min_max dmg mali range

//--> dodati offhand za castere


//EFFECTS
// critical damage or spell damage ignores armor / resistance ?? affix or fact?

Item::Item(const ItemProperties& ItemInfo)
	: ItemInfo(ItemInfo)
{}

Item::Item(ItemProperties&& ItemInfo)
	: ItemInfo(std::move(ItemInfo))
{}

Item::Item(const ItemData& Data) {
	ItemInfo.Id = Data.Id;
	ItemInfo.Level = Data.MinLevel;  // We will add only specific items for the start of the game so the iLevel min max will be the same
	ItemInfo.MinDmg = Data.MinDmg;
	ItemInfo.MaxDmg = Data.MaxDmg;
	ItemInfo.Armor = Data.Armor;
	ItemInfo.Amount = Data.Amount;

	ItemInfo.Slots = Data.Slots;
	ItemInfo.ArmorMod = Data.ArmorMod;	// For each armor type different mod [TODO IMPLEMENT]
	ItemInfo.WeaponMod = Data.WeaponMod;	// Each weapon type different mod, and different meaning, programmed in a [TODO FUNCTION]

	ItemInfo.Name = Data.Name;
	ItemInfo.ItemSlot = Data.ItemSlot;
	ItemInfo.ItemRarity = EItemRarity::COMMON;

	ItemInfo.ItemType = Data.ItemType;
	ItemInfo.WeaponType = Data.WeaponType;
}

std::vector<std::unique_ptr<Item>> Item::GenerateLoot(const std::weak_ptr<PlayerCharacter>& Player, int PowerLevel) {
	std::vector<std::unique_ptr<Item>> Loot;
	std::vector<std::pair<int, int>> ItemTypeLimit = {
		{ 0,1 },{ 0,1 },{ 0,1 },{ 0,2 },{ 0,3 },{ 0,3 }
	};

	while (PowerLevel > 0) {
		for (int i = 0; i < ITEM_TYPES; i++) {
			while (ItemTypeLimit[i].first == ItemTypeLimit[i].second) {
				++i;
				// we should adjust the formulas so that this actually never happens
				if (i == ITEM_TYPES) return Loot;
			}

			const int Rnd = GameplayStatics::GetRandInt(1, 1000);
			if (const int Weight = DropTable_ItemType[i].second.first * Player.lock()->GetLevel(); PowerLevel - Weight >= 0 && Rnd <= DropTable_ItemType[i].second.second * 1000) {
				Loot.push_back(Item::CreateItem(Player.lock()->GetLevel(), Player.lock()->GetMagicFind(), DropTable_ItemType[i].first));
				ItemTypeLimit[i].first++;
				PowerLevel -= Weight;
				break;
			}
			else if (PowerLevel - Weight < 0) return Loot;

			// no loot was rolled
			if (i == ITEM_TYPES - 1) return Loot;
		}
	}
	
	return Loot;
}

std::unique_ptr<Item> Item::CreateItem(const int PlayerLevel, const float MfBonus, const EItemType ItemType) {
	auto Rarity = EItemRarity::COMMON;
	const int Rnd = GameplayStatics::GetRandInt(1, 100000);

	for (int i = 0; i < ITEM_RARITIES; i++)
		if (const int Chance = static_cast<int>(DropTable_ItemRarity[i].second * MfBonus * 100000); Rnd <= Chance) {
			Rarity = DropTable_ItemRarity[i].first;
			break;
		}
	
	return std::make_unique<Item>(GenerateItemInfo(PlayerLevel, ItemType, Rarity));
}

std::unique_ptr<Item> Item::CreateItemById(const EItemId Id) {
	for (const auto& ItemData : ItemDb::Data)
		if (ItemData.Id == Id)
			return std::make_unique<Item>(ItemData);
	
	return nullptr;
}

Item::ItemProperties Item::GenerateItemInfo(const int PlayerLevel, const EItemType ItemType, const EItemRarity ItemRarity) {
	ItemProperties ItemInfo;
	ItemInfo.nAffixes = ITEM_RARITIES - 1 - static_cast<int>(ItemRarity);
	ItemInfo.Level = CalculateItemLevel(PlayerLevel, ItemInfo.nAffixes);
	ItemInfo.ItemRarity = ItemRarity;
	ItemInfo.ItemType = ItemType;
	ItemInfo.WeaponType = EWeaponType::NONE;

	GenerateItemSlot(ItemInfo);

	if (ItemInfo.ItemType != EItemType::CONSUMABLE && ItemInfo.ItemType != EItemType::SCROLL) {
		GetBaseItem(ItemInfo);
		RollAffixes(ItemInfo);
	}

	if (ItemInfo.ItemType == EItemType::ARMOR) CalculateItemArmor(ItemInfo);	// TODO RENAME FUNCTIONS
	else if (ItemInfo.ItemType == EItemType::WEAPON) CalculateItemDamage(ItemInfo);	// TODO RENAME FUNCTIONS

	return ItemInfo;
}

int Item::CalculateItemLevel(const int PlayerLevel, const int nAffixes) {
	int ItemLevel = PlayerLevel * 8;
	ItemLevel += static_cast<int>(nAffixes * 0.10 * ItemLevel);
	const int MinItemLevel = static_cast<int>(ItemLevel - ItemLevel * 0.15 - 3);
	const int MaxItemLevel = static_cast<int>(ItemLevel + ItemLevel * 0.15 + 3);
	return GameplayStatics::GetRandInt(MinItemLevel, MaxItemLevel - MinItemLevel);
}

void Item::GenerateItemSlot(ItemProperties& ItemInfo) {
	int Rnd;
	switch (ItemInfo.ItemType) {
	case EItemType::CONSUMABLE:
		GenerateRandomConsumable(ItemInfo);
		ItemInfo.ItemSlot = EItemSlot::NONE;
		ItemInfo.bUsable = true;
		break;
	case EItemType::SCROLL:
		GenerateRandomScroll(ItemInfo);
		ItemInfo.ItemSlot = EItemSlot::NONE;
		ItemInfo.bUsable = true;
		break;
	case EItemType::ARMOR:
		Rnd = GameplayStatics::GetRandInt(0, 5);
		ItemInfo.ItemSlot = static_cast<EItemSlot>(Rnd);
		break;
	case EItemType::JEWELLERY:
		Rnd = GameplayStatics::GetRandInt(6, 8);
		ItemInfo.ItemSlot = static_cast<EItemSlot>(Rnd);
		ItemInfo.Name = "JEWELLERY";
		break;
	case EItemType::WEAPON:
		Rnd = GameplayStatics::GetRandInt(9, 10);
		ItemInfo.ItemSlot = static_cast<EItemSlot>(Rnd);
		if (ItemInfo.ItemSlot == EItemSlot::WPN_MAIN)
			Rnd = GameplayStatics::GetRandInt(0, static_cast<int>(EWeaponType::LAST));
		else
			Rnd = GameplayStatics::GetRandInt(static_cast<int>(EWeaponType::LAST_1H) - static_cast<int>(EWeaponType::FIRST_1H) - 1, static_cast<int>(EWeaponType::FIRST_1H));
		ItemInfo.WeaponType = static_cast<EWeaponType>(Rnd);
		break;
	case EItemType::RELIC:
		ItemInfo.ItemSlot = EItemSlot::RELIC;
		ItemInfo.Name = "RELIC";
		break;
	case EItemType::MISC:
		break;
	}
}

void Item::GetBaseItem(ItemProperties& ItemInfo) {
	for (const auto& BaseItem : ItemDb::Data) {
		if (BaseItem.ItemType == ItemInfo.ItemType &&
			BaseItem.WeaponType == ItemInfo.WeaponType &&
			BaseItem.ItemSlot == ItemInfo.ItemSlot &&
			BaseItem.MaxLevel >= ItemInfo.Level && BaseItem.MinLevel <= ItemInfo.Level) {

			ItemInfo.Id = BaseItem.Id;
			ItemInfo.Name = (ItemInfo.ItemRarity == EItemRarity::COMMON ? "" : GameplayStatics::GetEnumString(ItemInfo.ItemRarity) + " ") + BaseItem.Name;
			ItemInfo.MinDmg = BaseItem.MinDmg;
			ItemInfo.MaxDmg = BaseItem.MaxDmg;
			ItemInfo.Armor = BaseItem.Armor;
			ItemInfo.Slots = BaseItem.Slots;
			ItemInfo.ArmorMod = BaseItem.ArmorMod;
			ItemInfo.WeaponMod = BaseItem.WeaponMod;
		}
	}
}

void Item::RollAffixes(ItemProperties& ItemInfo) {
	// rare = 1 affix [ 1 passive ]
	// epic = 2 affix [ 1 passive ]
	// legend = 3 affix [ 1 passive , 1 active]
	// godlike = 4 affix [1 passive, 1 active + MUST BE passive / active]

	// legend passive 1 out of 2
	// passive 1 out of 5

	// active 1 out of 10



}

void Item::CalculateItemDamage(ItemProperties& ItemInfo) {

	// VERY SUPERFICIAL PROTOTYPE
	// HAVE TO TEST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	switch (ItemInfo.WeaponType) {
	case EWeaponType::AXE_1H:
	case EWeaponType::MACE_1H:
	case EWeaponType::SWORD_1H:
		ItemInfo.MinDmg = static_cast<int>(ItemInfo.MinDmg - 0.15 * ItemInfo.Level);
		ItemInfo.MaxDmg = static_cast<int>(ItemInfo.MaxDmg + 0.15 * ItemInfo.Level);
		return;
	case EWeaponType::DAGGER:
		ItemInfo.MinDmg = static_cast<int>(ItemInfo.MinDmg - 0.12 * ItemInfo.Level);
		ItemInfo.MaxDmg = static_cast<int>(ItemInfo.MaxDmg + 0.12 * ItemInfo.Level);
		return;
	case EWeaponType::AXE_2H:
	case EWeaponType::MACE_2H:
	case EWeaponType::SWORD_2H:
		ItemInfo.MinDmg = static_cast<int>(ItemInfo.MinDmg - 0.28 * ItemInfo.Level);
		ItemInfo.MaxDmg = static_cast<int>(ItemInfo.MaxDmg + 0.28 * ItemInfo.Level);
		return;
	case EWeaponType::BOW:
		ItemInfo.MinDmg = static_cast<int>(ItemInfo.MinDmg - 0.22 * ItemInfo.Level);
		ItemInfo.MaxDmg = static_cast<int>(ItemInfo.MaxDmg + 0.22 * ItemInfo.Level);
		return;
	case EWeaponType::STAFF:
		ItemInfo.MinDmg = static_cast<int>(ItemInfo.MinDmg - 0.25 * ItemInfo.Level);
		ItemInfo.MaxDmg = static_cast<int>(ItemInfo.MaxDmg + 0.25 * ItemInfo.Level);
		return;
	}
}

void Item::CalculateItemArmor(ItemProperties& ItemInfo) {

	// VERY SUPERFICIAL PROTOTYPE. HAVE TO TEST!!!!!!
	//-----------------------------------------------

	switch (ItemInfo.ItemSlot) {
	case EItemSlot::HEAD:
		ItemInfo.Armor = static_cast<int>(ItemInfo.Armor + 0.3 * ItemInfo.Level);
		return;
	case EItemSlot::CHEST:
		ItemInfo.Armor = static_cast<int>(ItemInfo.Armor + 0.4 * ItemInfo.Level);
		return;
	case EItemSlot::HANDS:
		ItemInfo.Armor = static_cast<int>(ItemInfo.Armor + 0.2 * ItemInfo.Level);
		return;
	case EItemSlot::BELT:
		ItemInfo.Armor = static_cast<int>(ItemInfo.Armor + 0.15 * ItemInfo.Level);
		return;
	case EItemSlot::LEGS:
		ItemInfo.Armor = static_cast<int>(ItemInfo.Armor + 0.3 * ItemInfo.Level);
		return;
	case EItemSlot::FEET:
		ItemInfo.Armor = static_cast<int>(ItemInfo.Armor + 0.2 * ItemInfo.Level);
		return;
	default:
		return;
	}
}

void Item::GenerateRandomConsumable(ItemProperties& ItemInfo) {
	const int RndInt = GameplayStatics::GetRandInt(1, 1000);
	for (const auto& RndItem : ItemDb::Data) {
		if (RndItem.ItemType == EItemType::CONSUMABLE && RndInt <= RndItem.DropChance) {
			ItemInfo.Name = ItemInfo.ItemRarity == EItemRarity::COMMON ? "" : GameplayStatics::GetEnumString(ItemInfo.ItemRarity) + " ";
			ItemInfo.Name += RndItem.Name;
			ItemInfo.Id = RndItem.Id;
			ItemInfo.Amount = RndItem.Amount * ItemInfo.nAffixes;

			// For now only health and essence potions cant be used out of combat
			if (RndItem.Id != EItemId::HPotion && RndItem.Id != EItemId::EPotion)
				ItemInfo.bUsableMap = true;

			break;
		}
	}
}

void Item::GenerateRandomScroll(ItemProperties& ItemInfo) {
	const int NUM_SCROLLS = static_cast<int>(SpellDb::Data.size() - 2);  // later change to a DEFINE
	while (true) {
		int Rnd = GameplayStatics::GetRandInt(1, NUM_SCROLLS);

		// Here we need a good formula for converting ItemLevel to PowerLevel of Spell
		if (const auto Scroll = SpellDb::ActiveConstMap[static_cast<ESpellID>(Rnd)]; Scroll.PowerLevel <= 5 || ItemInfo.Level / 8 >= Scroll.PowerLevel) {
			ItemInfo.Active = static_cast<ESpellID>(Rnd);
			ItemInfo.Name = (ItemInfo.ItemRarity == EItemRarity::COMMON ? "" : GameplayStatics::GetEnumString(ItemInfo.ItemRarity) + " ") + "Scroll of " + GameplayStatics::GetEnumString(ItemInfo.Active);
			break;
		}
	}
}

void Item::Use(Character* Character) const {
	switch (ItemInfo.ItemType) {
		case EItemType::CONSUMABLE:
			switch (ItemInfo.Id) {
			case EItemId::HPotion:
				Character->GetHealth().UpdateActual(ItemInfo.Amount, Character);
				break;
			default: break;
			}
		default: break;
	}
}