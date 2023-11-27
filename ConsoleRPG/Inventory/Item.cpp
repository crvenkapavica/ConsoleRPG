#include "Item.h"
#include "../Effects/ActiveEffect.h"
#include "../Effects/PassiveEffect.h"
#include "../Characters/PlayerCharacter.h"

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

Item::Item(ItemInfo& item_info)
	: _item_info(item_info)
{}

std::vector<std::shared_ptr<Item>> GenerateLoot(weak_ptr<PlayerCharacter> player, int power_lvl) {
	std::vector<shared_ptr<Item>> loot;
	std::vector<pair<int, int>> type_limit = {
		{0, 1}, {0, 1}, {0, 1}, {0, 2}, {0, 3}, {0, 3}
	};

	while (power_lvl > 0) {
		for (int i = 0; i < ITEM_TYPES; i++) {
			while (type_limit[i].first == type_limit[i].second) ++i;
			// we should adjust the formulas so that this actually never happens
			if (i == ITEM_TYPES) return loot;

			int rnd = std::rand() % 1000;
			int weight = static_cast<int>(DropTable_ItemType[i].first) * player.lock()->GetLevel() / 2;
			if (power_lvl - weight >= 0 && rnd <= DropTable_ItemType[i].second * 1000) {
				loot.push_back(Item::CreateItem(player.lock()->GetLevel(), player.lock()->GetMagicFind(), DropTable_ItemType[i].first));
				type_limit[i].first++;
				power_lvl -= weight;
				continue;
			}
			else if (power_lvl - weight < 0) {
				return loot;
			}
			// no loot was rolled
			if (i == ITEM_TYPES - 1) return loot;
		}
	}
	return loot;
}

std::shared_ptr<Item> Item::CreateItem(int player_lvl, float mf_bonus, EItemType item_type) {

	int n_affixes;
	EItemRarity item_rarity;

	int rnd = std::rand() % 100000;
	for (int i = 0; i < ITEM_RARITIES; i++) {
		int chance = DropTable_ItemRarity[i].second + DropTable_ItemRarity[i].second * mf_bonus * 100000;
		if (rnd <= chance) {
			item_rarity = DropTable_ItemRarity[i].first;
			n_affixes = ITEM_RARITIES - 1 - i;
			break;
		}
	}

	int b_Ilvl = player_lvl * 8 + 10;
	b_Ilvl += n_affixes * 0.05;
	int min_Ilvl = b_Ilvl - b_Ilvl * 0.15;
	int max_Ilvl = b_Ilvl + b_Ilvl * 0.15;

	ItemInfo item_info = GenerateItemInfo(item_type, item_rarity)

	return make_shared<Item>(item_info);
}

Item::ItemInfo Item::GenerateItemInfo(EItemType item_type, EItemRarity item_rarity) {
	ItemInfo item_info;
	item_info._item_name = "PLACEHOLDER ITEM NAME"; // TODO IMPLEMENT
	item_info._item_rarity = item_rarity;
	item_info._weapon_type = EWeaponType::NONE;

	switch (item_type) {
	case EItemType::CONSUMABLE:
	case EItemType::SCROLL:
		item_info._item_slot = EItemSlot::NONE;
		break;
	case EItemType::ARMOR:
		int rnd = std::rand() % 6;
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		break;
	case EItemType::JEWLERY:
		int rnd = std::rand() % 3 + 10;
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		break;
	case EItemType::WEAPON:
		int rnd = std::rand() % 2 + 20;
		item_info._item_slot = static_cast<EItemSlot>(rnd);
		if (item_info._item_slot == EItemSlot::WEAPON_MAIN) 
			rnd = std::rand() % (static_cast<int>(EWeaponType::LAST_1H) - static_cast<int>(EWeaponType::FIRST_1H)) + static_cast<int>(EWeaponType::FIRST_1H;
		else 
			rnd = std::rand() % static_cast<int>(EWeaponType::LAST_2H);
		item_info._weapon_type = static_cast<EWeaponType>(rnd);
		break;
	case EItemType::RELIC:
		item_info._item_slot = EItemSlot::RELIC;
		break;
	default:
	}
}