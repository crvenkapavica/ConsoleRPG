#include "Item.h"
#include "ItemData.h"
#include "../GameplayStatics.h"
#include "../Items/ItemData.h"
#include "../Characters/Character.h"
#include "../Characters/PlayerCharacter.h"
#include "../Spells/SpellData.h"

std::vector<pair<EItemType, pair<int, double>>> DropTable_ItemType{
	{EItemType::RELIC,		{ 45, 0.02 } },
	{EItemType::WEAPON,		{ 28, 0.10 } },
	{EItemType::JEWLERY,	{ 22, 0.15 } },
	{EItemType::ARMOR,		{ 15, 0.25 } },
	{EItemType::SCROLL,		{ 6, 0.35 } },
	{EItemType::CONSUMABLE, { 2, 0.45 } }
};

std::vector<pair<EItemRarity, double>> DropTable_ItemRarity{
	{EItemRarity::UNIQUE,		0.005},
	{EItemRarity::GODLIKE,		0.025},
	{EItemRarity::LEGENDARY,	0.05},
	{EItemRarity::EPIC,			0.12},
	{EItemRarity::RARE,			0.25},
	{EItemRarity::COMMON,		1}
};

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
// scroll of [ACTIVE_EFFECT] ( spell_level = player-lvl + rarity )
	// -> nema unique
//string name, double drop_chance(koji ovisi o spell_rarity), item_ID

//ARMOR
// name = sufix + base + prefix
// base = item_lvl_range
// rarity = n_affixes
// string base_name, ilvl-drop-range (int, int), item_ID

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

Item::Item(ItemInfo item_info)
	: _item_info(move(item_info))
{}

Item::Item(const ItemData& data)
{
	_item_info._ID = data._ID;
	_item_info._lvl = data._min_lvl;  // we will add only specific items for the start of the game so the ilvl min max will be the same
	_item_info._min_dmg = data._min_dmg;
	_item_info._max_dmg = data._max_dmg;
	_item_info._armor = data._armor;
	_item_info._amount = data._amount;
	
	_item_info._slots = data._slots;
	_item_info._arm_mod = data._arm_mod;	// for each armor type different mod [TODO IMPLEMENT]
	_item_info._wpn_mod = data._wpn_mod;	// each weapon type different mod, and different meaning, programmed in a [TODO FUNCTION]

	_item_info._name = data._name;
	_item_info._item_slot = data._item_slot;
	_item_info._item_rarity = EItemRarity::COMMON;
	_item_info._item_type = data._item_type;
	_item_info._wpn_type = data._wpn_type;
}

std::vector<std::unique_ptr<Item>> Item::GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl) {
	std::vector<unique_ptr<Item>> loot;
	std::vector<pair<int, int>> type_limit = {
		{0, 1}, {0, 1}, {0, 1}, {0, 2}, {0, 3}, {0, 3}
	};

	while (power_lvl > 0) {
		for (int i = 0; i < ITEM_TYPES; i++) {
			while (type_limit[i].first == type_limit[i].second) {
				++i;
				// we should adjust the formulas so that this actually never happens
				if (i == ITEM_TYPES) return loot;
			}

			int rnd = GameplayStatics::GetRandInt(1, 1000);
			int weight = DropTable_ItemType[i].second.first * player.lock()->GetLevel();
			if (power_lvl - weight >= 0 && rnd <= DropTable_ItemType[i].second.second * 1000) {
				loot.push_back(Item::CreateItem(player.lock()->GetLevel(), player.lock()->GetMagicFind(), DropTable_ItemType[i].first));
				type_limit[i].first++;
				power_lvl -= weight;
				break;
			}
			else if (power_lvl - weight < 0) return loot;

			// no loot was rolled
			if (i == ITEM_TYPES - 1) return loot;
		}
	}
	return loot;
}

std::unique_ptr<Item> Item::CreateItem(int player_lvl, float mf_bonus, EItemType item_type) {

	EItemRarity item_rarity = EItemRarity::COMMON;
	int rnd = GameplayStatics::GetRandInt(1, 100000);

	for (int i = 0; i < ITEM_RARITIES; i++) {
		int chance = static_cast<int>(DropTable_ItemRarity[i].second * mf_bonus * 100000);
		if (rnd <= chance) {
			item_rarity = DropTable_ItemRarity[i].first;
			break;
		}
	}

	ItemInfo item_info = GenerateItemInfo(player_lvl, item_type, item_rarity);

	return make_unique<Item>(move(item_info));
}

std::unique_ptr<Item> Item::CreateItemByID(EItemID id) {
	for (const auto& item : ItemDB::_data)
		if (item._ID == id)
			return make_unique<Item>(item);

	return nullptr;
}

Item::ItemInfo Item::GenerateItemInfo(int player_lvl, EItemType item_type, EItemRarity item_rarity) {
	ItemInfo item_info;
	item_info._n_affixes = ITEM_RARITIES - 1 - static_cast<int>(item_rarity);
	item_info._lvl = CalcItemLvl(player_lvl, item_info._n_affixes);
	item_info._item_rarity = item_rarity;
	item_info._item_type = item_type;
	item_info._wpn_type = EWeaponType::NONE;

	GenerateItemSlot(item_info);

	if (item_info._item_type != EItemType::CONSUMABLE && item_info._item_type != EItemType::SCROLL) {
		GetBaseItem(item_info);
		RollAffixes(item_info);
	}

	     if (item_info._item_type == EItemType::ARMOR)	CalcItemArmor(item_info); // TODO RENAME FUNCTIONS
	else if (item_info._item_type == EItemType::WEAPON) CalcItemDamage(item_info);// TODO RENAME FUNCTIONS

	return item_info;
}

int Item::CalcItemLvl(int player_lvl, int n_affixes) {
	int b_Ilvl = player_lvl * 8;
	b_Ilvl += static_cast<int>(n_affixes * 0.10 * b_Ilvl);
	int min_Ilvl = static_cast<int>(b_Ilvl - b_Ilvl * 0.15 - 3);
	int max_Ilvl = static_cast<int>(b_Ilvl + b_Ilvl * 0.15 + 3);
	return GameplayStatics::GetRandInt(min_Ilvl, max_Ilvl - min_Ilvl);
}

void Item::GenerateItemSlot(ItemInfo& item_info) {
	int rnd;
	switch (item_info._item_type) {
	case EItemType::CONSUMABLE:
		GenerateRndConsumable(item_info);
		item_info._item_slot = EItemSlot::NONE;
		item_info._bUsable = true;
		break;
	case EItemType::SCROLL:
		GenerateRndScroll(item_info);
		item_info._item_slot = EItemSlot::NONE;
		item_info._bUsable = true;
		break;
	case EItemType::ARMOR:
		rnd = GameplayStatics::GetRandInt(0, 5);
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		break;
	case EItemType::JEWLERY:
		rnd = GameplayStatics::GetRandInt(6, 8);
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		item_info._name = "JEWLERY";
		break;
	case EItemType::WEAPON:
		rnd = GameplayStatics::GetRandInt(9, 10);
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		if (item_info._item_slot == EItemSlot::WPN_MAIN)
			rnd = GameplayStatics::GetRandInt(0, static_cast<int>(EWeaponType::LAST));
		else
			rnd = GameplayStatics::GetRandInt(static_cast<int>(EWeaponType::LAST_1H) - static_cast<int>(EWeaponType::FIRST_1H) - 1, static_cast<int>(EWeaponType::FIRST_1H));
		item_info._wpn_type = static_cast<EWeaponType>(rnd);
		break;
	case EItemType::RELIC:
		item_info._item_slot = EItemSlot::RELIC;
		item_info._name = "RELIC";
		break;
	default:
		break;
	}
}

void Item::GetBaseItem(ItemInfo& item_info) {
	for (const auto& item : ItemDB::_data) {
		if (item._item_type == item_info._item_type &&
			item._wpn_type == item_info._wpn_type &&
			item._item_slot == item_info._item_slot &&
			item._max_lvl >= item_info._lvl && item._min_lvl <= item_info._lvl) {
			
			item_info._ID = item._ID;
			item_info._name = (item_info._item_rarity == EItemRarity::COMMON ? "" : GameplayStatics::GetEnumString(item_info._item_rarity) + " ") + item._name;
			item_info._min_dmg = item._min_dmg;
			item_info._max_dmg = item._max_dmg;
			item_info._armor = item._armor;
			item_info._slots = item._slots;
			item_info._arm_mod = item._arm_mod;
			item_info._wpn_mod = item._wpn_mod;
		}
	}
}

void Item::RollAffixes(ItemInfo& item_info) {
	// rare = 1 affix [ 1 passive ]
	// epic = 2 affix [ 1 passive ]
	// legend = 3 affix [ 1 passive , 1 active]
	// godlike = 4 affix [1 passive, 1 acitve + MUST BE passive / active]

	// legend passive 1 out of 2
	// passive 1 out of 5
	
	// active 1 out of 10



}

void Item::CalcItemDamage(ItemInfo& item_info) {

	// VERY SUPERFICIAL PROTOTYPE
	// HAVE TO TEST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	switch (item_info._wpn_type) {
	case EWeaponType::AXE_1H:
	case EWeaponType::MACE_1H:
	case EWeaponType::SWORD_1H:
		item_info._min_dmg = static_cast<int>(item_info._min_dmg - 0.15 * item_info._lvl);
		item_info._max_dmg = static_cast<int>(item_info._max_dmg + 0.15 * item_info._lvl);
		return;
	case EWeaponType::DAGGER:
		item_info._min_dmg = static_cast<int>(item_info._min_dmg - 0.12 * item_info._lvl);
		item_info._max_dmg = static_cast<int>(item_info._max_dmg + 0.12 * item_info._lvl);
		return;
	case EWeaponType::AXE_2H:
	case EWeaponType::MACE_2H:
	case EWeaponType::SWORD_2H:
		item_info._min_dmg = static_cast<int>(item_info._min_dmg - 0.28 * item_info._lvl);
		item_info._max_dmg = static_cast<int>(item_info._max_dmg + 0.28 * item_info._lvl);
		return;
	case EWeaponType::BOW:
		item_info._min_dmg = static_cast<int>(item_info._min_dmg  - 0.22 * item_info._lvl);
		item_info._max_dmg = static_cast<int>(item_info._max_dmg  + 0.22 * item_info._lvl);
		return;
	case EWeaponType::STAFF:
		item_info._min_dmg = static_cast<int>(item_info._min_dmg - 0.25 * item_info._lvl);
		item_info._max_dmg = static_cast<int>(item_info._max_dmg + 0.25 * item_info._lvl);
		return;
	default:
		return;
	}
}

void Item::CalcItemArmor(ItemInfo& item_info) {

	// VERY SUPERFICIAL PROTOTYPE. HAVE TO TEST!!!!!!
	//-----------------------------------------------

	switch (item_info._item_slot) {
	case EItemSlot::HEAD:
		item_info._armor = static_cast<int>(item_info._armor + 0.3 * item_info._lvl);
		return;
	case EItemSlot::CHEST:
		item_info._armor = static_cast<int>(item_info._armor + 0.4 * item_info._lvl);
		return;
	case EItemSlot::HANDS:
		item_info._armor = static_cast<int>(item_info._armor + 0.2 * item_info._lvl);
		return;
	case EItemSlot::BELT:
		item_info._armor = static_cast<int>(item_info._armor + 0.15 * item_info._lvl);
		return;
	case EItemSlot::LEGS:
		item_info._armor = static_cast<int>(item_info._armor + 0.3 * item_info._lvl);
		return;
	case EItemSlot::FEET:
		item_info._armor = static_cast<int>(item_info._armor + 0.2 * item_info._lvl);
		return;
	default:
		return;
	}
} 

void Item::GenerateRndConsumable(ItemInfo& item_info) {
	int rnd = GameplayStatics::GetRandInt(1, 1000);
	for (const auto& item : ItemDB::_data) {
		if (item._item_type == EItemType::CONSUMABLE && rnd <= item._drop_chnc) {
			item_info._name = item_info._item_rarity == EItemRarity::COMMON ? "" : GameplayStatics::GetEnumString(item_info._item_rarity) + " ";
			item_info._name += item._name;
			item_info._ID = item._ID;
			item_info._amount = item._amount * item_info._n_affixes;

			// for now only health and essence poitions cant be used out of combat
			if (item._ID != EItemID::HPotion && item._ID != EItemID::EPotion)
				item_info._bUsableMap = true;

			break;
		}
	}
}

void Item::GenerateRndScroll(ItemInfo& item_info) {
	const int NUM_SCROLLS = SpellDB::_data.size() - 2;  // later change to a DEFINE
	while (1) {
		int rnd = GameplayStatics::GetRandInt(1, NUM_SCROLLS);
		auto scroll = SpellDB::_active_const_map[static_cast<ESpellID>(rnd)];

		// here we need a good formula for converting item_lvl to power_lvl of SPELL
		if (scroll._power_lvl <= 5 || item_info._lvl / 8 >= scroll._power_lvl) {
			item_info._active = static_cast<ESpellID>(rnd);
			item_info._name = (item_info._item_rarity == EItemRarity::COMMON ? "" : GameplayStatics::GetEnumString(item_info._item_rarity) + " ") + "Scroll of " + GameplayStatics::GetEnumString(item_info._active);
			break;
		}
	}
}

void Item::Use(Character* character) {
	switch (_item_info._item_type) {
	case EItemType::CONSUMABLE:
		switch (_item_info._ID) {
		case EItemID::HPotion:
			character->GetHealth().UpdateActual(_item_info._amount, character);
		}
		break;
	}
}