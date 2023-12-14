#include "PlayerCharacter.h"
#include "../GameplayStatics.h"
#include "../Inventory/ItemData.h"

PlayerCharacter::PlayerCharacter(const CharacterData::PlayerAttributes& attributes)
	: Character(attributes)
{
	InitExperienceForLevel();

	_item_slots.resize(ITEM_SLOTS);
	_inventory.resize(INV_SLOTS);

	//unique_ptr<Item> item = Item::CreateItem(_lvl, _magic_find, EItemType::WEAPON);
	//AddItemToInventory(item);

	/*auto items = Item::GenerateLoot(this, 125);
	for (auto& item : items)
		AddItemToInventory(move(item));*/

	AddItemToInventory(Item::GetItemByID(EItemID::HatchAxe));
	AddItemToInventory(Item::GetItemByID(EItemID::AssNeedle));
	AddItemToInventory(Item::GetItemByID(EItemID::HatchAxe));
	AddItemToInventory(Item::GetItemByID(EItemID::AssNeedle));
	AddItemToInventory(Item::GetItemByID(EItemID::HatchAxe));
	AddItemToInventory(Item::GetItemByID(EItemID::AssNeedle));
}

void PlayerCharacter::ReceiveExperience(const int experience) {
	_experience += experience;
	if (_experience >= _experience_next_level[_lvl - 1]) {
		_experience -= _experience_next_level[_lvl - 1];
		LevelUp();
	}
}

void PlayerCharacter::TakeTurn() {
	
	if (_bIsInCombat) {
		GameplayStatics::RedrawGameScreen();
		int input = GameplayStatics::DisplayCombatMenu();
		GameplayStatics::HandleCombatInput(this, input);
	}
}

void PlayerCharacter::EquipItem(unique_ptr<Item> item) {
	if (!item || _n_inventory == INV_SLOTS) return;

	auto slot = EItemSlot::NONE;
	if (item->_item_info._item_type == EItemType::WEAPON) {
		if (item->_item_info._item_slot == EItemSlot::WPN_BOTH)
			slot = EItemSlot::WPN_MAIN;
	}

	_item_slots[static_cast<int>(slot)].swap(item);
	AddItemToInventory(move(item));

	SortInventory();
	// CalculatePlayerItemValue (stats);
}

void PlayerCharacter::UnEquipItem(unique_ptr<Item> item) {
	// if inventory is full -> return;

	// CalculatePlayerItemValue (stats);
}

bool PlayerCharacter::AddItemToInventory(unique_ptr<Item> item) {

	for (auto& inv_item : _inventory) {
		if (!inv_item) {
			inv_item = move(item);
			++_n_inventory;
			return true;
		}
		++_n_inventory;
	}

	return false;
}

void PlayerCharacter::InspectItem(Item* item) {

}

void PlayerCharacter::DestroyItem(unique_ptr<Item> item) {
	item.reset();
	SortInventory();
}

Item* PlayerCharacter::DisplayEquipedItems() {
	for (const auto& item : _item_slots) 
		if (item) cout << GameplayStatics::GetEnumString(item->_item_info._item_slot) << " --> " << item->_item_info._name << endl;

	return nullptr;
}

Item* PlayerCharacter::DisplayInventory() {
	for (int i = 0; i < _inventory.size(); i++)
		if (_inventory[i].get())
			cout << i << ".) " << _inventory[i]->_item_info._name << "  " << GameplayStatics::GetEnumString(_inventory[i]->_item_info._item_slot) << endl;

	cout << endl << "Press any key to go back.";
	cin.get();

	return nullptr;
}

Item* PlayerCharacter::DisplayConsumableSlots() {
	return nullptr;
}

ActiveSpell* PlayerCharacter::DisplayActiveSpellSlots() {
	return nullptr;
}

PassiveSpell* PlayerCharacter::DisplayPassiveSpellSlots() {
	return nullptr;
}

std::unique_ptr<Item> PlayerCharacter::DisplayAllItems(bool& bIsEquiped) {

	vector<string> v = { "ALL ITEMS", "RELICS", "WEAPONS", "JEWLERY", "ARMOR", "SCROLLS", "CONSUMABLES", "<--BACK--<" };
	int input;
	if ((input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;
	auto type = static_cast<EItemType>(ITEM_TYPES - input);

	v = { "ALL RARITIES", "COMMON", "RARE", "EPIC", "LEGENDARY", "GODLIKE", "UNIQUE", "<--BACK--<" };
	if ((input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;
	auto rarity = static_cast<EItemRarity>(input);

	v.clear();
	// treba auto sortirati da su svi v inventoriju po redu a na kraju nullptr
	for (const auto& item : _inventory)
		if (!item.get()) {
			if (type == EItemType::MISC)
				v.push_back("INVENTORY --> (empty)");
		}
		else if ((item->_item_info._item_type == type || type == EItemType::MISC) && (item->_item_info._item_rarity == rarity || rarity == EItemRarity::MISC))
			v.push_back("INVENTORY --> " + item->_item_info._name);

	for (int i = 0; i < _item_slots.size(); i++)
		if (((_item_slots[i] && (_item_slots[i]->_item_info._item_type == type || type == EItemType::MISC)) || type == EItemType::MISC)
			&&
			((_item_slots[i] && (_item_slots[i]->_item_info._item_rarity == rarity || rarity == EItemRarity::MISC)) || rarity == EItemRarity::MISC)) {
			string s = GameplayStatics::GetEnumString(static_cast<EItemSlot>(i));
			if (i <= 8 || i == 11) s += "\t  *--> "; if (i == 9) s += " *--> "; if (i == 10) s += "  *--> ";
			if (_item_slots[i]) s += _item_slots[i]->_item_info._name;
			else s += "(empty)";
			v.push_back(s);
		}
			
	v.push_back("<--BACK--<");
	if ((input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;

	std::unique_ptr<Item> item;
	// item is from inventory
	if (input < _n_inventory) {
		item = move(_inventory[input]);
	}
	// item is equiped
	else {
		item = move(_item_slots[input - _n_inventory]);
		bIsEquiped = true;
	}	
	return item;
}

void PlayerCharacter::InitExperienceForLevel() {

	int start = 17;
	_experience_next_level[0] = 17;

	int constant = 7;
	for (int i = 1; i <= MAX_LVL; i++) {
		if (i % 3 == 0) {
			constant += constant / 2;
		}
		int next_level = static_cast<int>(_experience_next_level[i - 1] + (_experience_next_level[i - 1] * 0.1) + constant);
		_experience_next_level[i] = next_level;
	}
}

void PlayerCharacter::LevelUp() {
	++_lvl;
	// print you have leveled up!
	_unspent_attributes += 2;
}

void PlayerCharacter::SortInventory() {
	sort(_inventory.begin(), _inventory.end(), [&](const std::unique_ptr<Item>& first, const std::unique_ptr<Item>& second) { return first && !second; });
}