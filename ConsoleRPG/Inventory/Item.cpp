#include "Item.h"
#include "../Effects/ActiveEffect.h"
#include "../Effects/PassiveEffect.h"

std::vector<pair<EItemType, double>> DropTable_ItemType{
	{EItemType::RELIC, 0.02},
	{EItemType::WEAPON, 0.5},
	{EItemType::JEWLERY, 0.7},
	{EItemType::ARMOR, 0.15},
	{EItemType::SCROLL, 0.28},
	{EItemType::CONSUMABLE, 0.35}
};

std::vector<pair<EItemRarity, double>> DropTable_ItemRarity{
	{EItemRarity::UNIQUE, 0.005},
	{EItemRarity::GODLIKE, 0.025},
	{EItemRarity::LEGENDARY, 0.05},
	{EItemRarity::EPIC, 0.12},
	{EItemRarity::RARE, 0.25},
	{EItemRarity::COMMON, 1}
};

Item::Item(ItemInfo item_info)
	: _item_info(move(item_info))
{}

std::vector<std::unique_ptr<Item>> Item::GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl) {
	std::vector<unique_ptr<Item>> loot;
	std::vector<pair<int, int>> type_limit = {
		{0, 1}, {0, 1}, {0, 1}, {0, 2}, {0, 3}, {0, 3}
	};

	while (power_lvl > 0) {
		for (int i = 0; i < ITEM_TYPES; i++) {
			while (type_limit[i].first == type_limit[i].second) ++i;
			// we should adjust the formulas so that this actually never happens
			if (i == ITEM_TYPES) return loot;

			int rnd = std::rand() % 1000;
			int weight = static_cast<int>(DropTable_ItemType[i].first) * player.lock()->GetLevel();
			if (power_lvl - weight >= 0 && rnd <= DropTable_ItemType[i].second * 1000) {
				loot.push_back(Item::CreateItem(player.lock()->GetLevel(), player.lock()->GetMagicFind().GetActual(), DropTable_ItemType[i].first));
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

	int n_affixes = 0;
	EItemRarity item_rarity = EItemRarity::COMMON;

	int rnd = std::rand() % 100000;
	for (int i = 0; i < ITEM_RARITIES; i++) {
		int chance = static_cast<int>(DropTable_ItemRarity[i].second + DropTable_ItemRarity[i].second * mf_bonus * 100000);
		if (rnd <= chance) {
			item_rarity = DropTable_ItemRarity[i].first;
			n_affixes = ITEM_RARITIES - 1 - i;
			break;
		}
	}

	int b_Ilvl = player_lvl * 8;
	b_Ilvl += static_cast<int>(n_affixes * 0.10);
	int min_Ilvl = static_cast<int>(b_Ilvl - b_Ilvl * 0.15 - 3);
	int max_Ilvl = static_cast<int>(b_Ilvl + b_Ilvl * 0.15 + 3);
	int item_lvl = std::rand() % (max_Ilvl - min_Ilvl) + min_Ilvl;

	ItemInfo item_info = GenerateItemInfo(item_lvl, item_type, item_rarity);

	return make_unique<Item>(move(item_info));
}

Item::ItemInfo Item::GenerateItemInfo(int item_lvl, EItemType item_type, EItemRarity item_rarity) {
	ItemInfo item_info;
	item_info._lvl = item_lvl;
	item_info._name = "PLACEHOLDER ITEM NAME"; // TODO IMPLEMENT
	item_info._item_type = item_type;
	item_info._item_rarity = item_rarity;
	item_info._weapon_type = EWeaponType::NONE;

	int rnd;
	switch (item_type) {
	case EItemType::CONSUMABLE:
	case EItemType::SCROLL:
		item_info._item_slot = EItemSlot::NONE;
		break;
	case EItemType::ARMOR:
		rnd = std::rand() % 6;
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		break;
	case EItemType::JEWLERY:
		rnd = std::rand() % 3 + 10;
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		break;
	case EItemType::WEAPON:
		rnd = std::rand() % 2 + 20;
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		if (item_info._item_slot == EItemSlot::WEAPON_MAIN)
			rnd = std::rand() % static_cast<int>(EWeaponType::LAST);
		else 
			rnd = std::rand() % (static_cast<int>(EWeaponType::LAST_1H) - static_cast<int>(EWeaponType::FIRST_1H)) + static_cast<int>(EWeaponType::FIRST_1H);
		item_info._weapon_type = static_cast<EWeaponType>(rnd);
		break;
	case EItemType::RELIC:
		item_info._item_slot = EItemSlot::RELIC;
		break;
	default:
		break;
	}

	CalcItemDamage(item_lvl, item_info._weapon_type, item_info._dmg_min, item_info._dmg_max);
	CalcItemArmor(item_lvl, item_info._item_slot, item_info._armor);

	return item_info;
}

void Item::CalcItemDamage(int item_lvl, EWeaponType weapon_type, OUT int& min_dmg, OUT int& max_dmg) {

	switch (weapon_type) {
	case EWeaponType::AXE_1H:
	case EWeaponType::MACE_1H:
	case EWeaponType::SWORD_1H:
		min_dmg = static_cast<int>(item_lvl * 1.5 - 0.15 * item_lvl); 
		max_dmg = static_cast<int>(item_lvl * 1.5 + 0.15 * item_lvl); 
		return;
	case EWeaponType::DAGGER:
		min_dmg = static_cast<int>(item_lvl * 1.2 - 0.12 * item_lvl); 
		max_dmg = static_cast<int>(item_lvl * 1.2 + 0.12 * item_lvl); 
		return;
	case EWeaponType::AXE_2H:
	case EWeaponType::MACE_2H:
	case EWeaponType::SWORD_2H:
		min_dmg = static_cast<int>(item_lvl * 2.8 - 0.28 * item_lvl); 
		max_dmg = static_cast<int>(item_lvl * 2.8 + 0.28 * item_lvl); 
		return;
	case EWeaponType::BOW:
		min_dmg = static_cast<int>(item_lvl * 2.2 - 0.22 * item_lvl); 
		max_dmg = static_cast<int>(item_lvl * 2.2 + 0.22 * item_lvl); 
		return;
	case EWeaponType::STAFF:
		min_dmg = static_cast<int>(item_lvl * 2.5 - 0.25 * item_lvl); 
		max_dmg = static_cast<int>(item_lvl * 2.5 + 0.25 * item_lvl); 
		return;
	default:
		return;
	}
}

void Item::CalcItemArmor(int item_lvl, EItemSlot item_slot, OUT int& armor) {

	switch (item_slot) {
	case EItemSlot::HEAD:
		armor = static_cast<int>(item_lvl * 0.3 + 50);
		return;
	case EItemSlot::CHEST:
		armor = static_cast<int>(item_lvl * 0.4 + 75);
		return;
	case EItemSlot::HANDS:
		armor = static_cast<int>(item_lvl * 0.2 + 18);
		return;
	case EItemSlot::BELT:
		armor = static_cast<int>(item_lvl * 0.15 + 8);
		return;
	case EItemSlot::LEGS:
		armor = static_cast<int>(item_lvl * 0.3 * 35);
		return;
	case EItemSlot::FEET:
		armor = static_cast<int>(item_lvl * 0.2 + 12);
		return;
	default:
		return;
	}
} 