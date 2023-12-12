#include "PlayerCharacter.h"
#include "../GameplayStatics.h"
//#include "../Inventory/Item.h"

PlayerCharacter::PlayerCharacter(const CharacterData::PlayerAttributes& attributes)
	: Character(attributes)
{
	InitExperienceForLevel();
}

void PlayerCharacter::LevelUp() {
	++_level;
	// print you have leveled up!
	_unspent_attributes += 2;
}

void PlayerCharacter::ReceiveExperience(const int experience) {
	_experience += experience;
	if (_experience >= _experience_next_level[_level - 1]) {
		_experience -= _experience_next_level[_level - 1];
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

//unique_ptr<Item> PlayerCharacter::EquipItem(unique_ptr<Item> item) {
//	EItemSlot slot;
//	if (item->_item_info._item_type == EItemType::WEAPON) {
//		if (slot == EItemSlot::WPN_BOTH)
//			slot = EItemSlot::WPN_MAIN;
//	}
//	_item_slots[static_cast<int>(slot)].swap(item);
//}
//
//void PlayerCharacter::UnEquipItem(unique_ptr<Item> item) {
//
//}
//
//void PlayerCharacter::DisplayEquipedItems() {
//	for (const auto& item : _item_slots) 
//		if (item) cout << GameplayStatics::GetEnumString(item->_item_info._item_slot) << " --> " << item->_item_info._name << endl;
//}
//
//void PlayerCharacter::DisplayInventory() {
//	for (int i = 0; i < _inventory.size(); i++)
//		cout << i << ".) " << _inventory[i]->_item_info._name << endl;
//}
//
//void PlayerCharacter::DisplaySpellSlots() {
//
//}
//
//void PlayerCharacter::DisplayConsumableSlots() {
//
//}
//
//unique_ptr<Item> PlayerCharacter::DisplayAllItems(EItemType type, EItemRarity rarity) {
//
//	return nullptr;
//}
//
//void PlayerCharacter::InspectItem(unique_ptr<Item> item) {
//
//}