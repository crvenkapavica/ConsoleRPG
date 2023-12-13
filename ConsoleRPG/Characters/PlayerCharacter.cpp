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
}

void PlayerCharacter::LevelUp() {
	++_lvl;
	// print you have leveled up!
	_unspent_attributes += 2;
}

void PlayerCharacter::ReceiveExperience(const int experience) {
	_experience += experience;
	if (_experience >= _experience_next_level[_lvl - 1]) {
		_experience -= _experience_next_level[_lvl - 1];
		LevelUp();
	}
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

void PlayerCharacter::TakeTurn() {
	
	if (_bIsInCombat) {
		GameplayStatics::RedrawGameScreen();
		int input = GameplayStatics::DisplayCombatMenu();
		GameplayStatics::HandleCombatInput(this, input);
	}
}

unique_ptr<Item> PlayerCharacter::EquipItem(unique_ptr<Item> item) {
	auto slot = EItemSlot::NONE;
	if (item->_item_info._item_type == EItemType::WEAPON) {
		if (slot == EItemSlot::WPN_BOTH)
			slot = EItemSlot::WPN_MAIN;
	}
	_item_slots[static_cast<int>(slot)].swap(item);

	return nullptr;
}

void PlayerCharacter::UnEquipItem(unique_ptr<Item> item) {
	// if inventory is full -> return;
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

unique_ptr<Item> PlayerCharacter::GetItem(Item* item) {
	for (const auto& i : _inventory)
		if (i.get() == item)
			return i;
	for (const auto& i : _item_slots)
		if (i.get() == item)
			return i;
}

void PlayerCharacter::InspectItem(Item* item) {

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

Item* PlayerCharacter::DisplayAllItems(EItemType type, EItemRarity rarity) {

	vector<string> v;
	for (const auto& item : _inventory)
		if (!item) break;
		else ((item->_item_info._item_type == type || type == EItemType::MISC) && (item->_item_info == rarity || rarity == EItemRarity::MISC))
			v.push_back("Inventory: -> " + item->_item_info._name);
	for (const auto& item : _item_slots)
		if (item && (item->_item_info._item_type == type || type == EItemType::MISC) && (item->_item_info == rarity || rarity == EItemRarity::MISC))
			v.push_back(GameplayStatics::GetEnumString(type) + ": -> " + item->_item_info._name);
	v.push_back("<--BACK--<");
	int input;
	if ((input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;

	Item* item;
	// item is from inventory
	if (input < _n_inventory) {
		item = _inventory[input].get();
	}
	// item is equiped
	else {

	}	
	return item;
}