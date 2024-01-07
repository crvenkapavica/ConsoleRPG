#include "PlayerCharacter.h"
#include "../GameplayStatics.h"
#include "../Items/ItemData.h"
#include "../Characters/CharacterData.h"

int PlayerCharacter::_n = 0;

PlayerCharacter::PlayerCharacter(ECharacterClass character_class)
	: Character(CharDB::_data[character_class], CharDB::_attributes[character_class], '0' + _n++)
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

PlayerCharacter::PlayerCharacter(const PlayerCharacter& other) 
	: Character(other) 
{}

PlayerCharacter::PlayerCharacter(PlayerCharacter&& player) noexcept
	: Character(player)
{}

PlayerCharacter::~PlayerCharacter() 
{}

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
		int input = GameplayStatics::DisplayCombatMenu(this);
		GameplayStatics::HandleCombatInput(this, input);
	}
}

void PlayerCharacter::EquipItem(unique_ptr<Item> item) {
	if (!item || _n_inventory == INV_SLOTS) return; // treba promeniti da se pita ak je inventory pun samo ako je slot zauzet

	if (item->_item_info._item_type == EItemType::WEAPON) {
		if (item->_item_info._item_slot == EItemSlot::WPN_BOTH)
			item->_item_info._item_slot = EItemSlot::WPN_MAIN;
	}

	_item_slots[static_cast<int>(item->_item_info._item_slot)].swap(item);
	if (item) AddItemToInventory(move(item));

	SortInventory();
	CalcPlayerItemSlots();
	CalcInvSlots();
}

void PlayerCharacter::UnEquipItem(unique_ptr<Item> item) {
	if (!item || _n_inventory == INV_SLOTS) return;

	AddItemToInventory(move(item));
	CalcPlayerItemSlots();
	CalcInvSlots();
}

bool PlayerCharacter::AddItemToInventory(unique_ptr<Item> item) {	

	for (auto& inv_item : _inventory) {
		if (!inv_item) {
			inv_item = move(item);
			CalcInvSlots();
			return true;
		}
	}

	return false;
}

int PlayerCharacter::GetInventorySpace() {
	int size = 0;
	for (const auto& item : _inventory) 
		if (item) ++size;
	return INV_SLOTS - size;
}

void PlayerCharacter::InspectItem(Item* item) {

}

void PlayerCharacter::DestroyItem(unique_ptr<Item> item) {
	item.reset();

	SortInventory();
	CalcPlayerItemSlots();
	CalcInvSlots();
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

std::unique_ptr<Item> PlayerCharacter::DisplayAllItems(OUT bool& bIsEquiped) {

	vector<string> v = { "ALL ITEMS", "RELICS", "WEAPONS", "JEWLERY", "ARMOR", "SCROLLS", "CONSUMABLES", "<--BACK--<" };
	int input;
	if ((input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;
	auto type = static_cast<EItemType>(ITEM_TYPES - input);

	v = { "ALL RARITIES", "COMMON", "RARE", "EPIC", "LEGENDARY", "GODLIKE", "UNIQUE", "<--BACK--<" };
	if ((input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;
	auto rarity = static_cast<EItemRarity>(input);

	std::vector<Item*> items;
	v.clear();
	// treba auto sortirati da su svi v inventoriju po redu a na kraju nullptr
	for (const auto& item : _inventory)
		if (!item) {
			if (type == EItemType::MISC && rarity == EItemRarity::MISC) {
				v.push_back("INVENTORY ---> (empty)");
				items.push_back(nullptr);
			}
		}
		else if ((item->_item_info._item_type == type || type == EItemType::MISC) && (item->_item_info._item_rarity == rarity || rarity == EItemRarity::MISC)) {
			v.push_back("INVENTORY ---> " + item->_item_info._name);
			items.push_back(item.get());
		}

	map<int, int> equip_map;
	int equip_index = 0;
	for (int i = 0; i < _item_slots.size(); i++)
		if (((_item_slots[i] && (_item_slots[i]->_item_info._item_type == type || type == EItemType::MISC)) || type == EItemType::MISC)
			&&
			((_item_slots[i] && (_item_slots[i]->_item_info._item_rarity == rarity || rarity == EItemRarity::MISC)) || rarity == EItemRarity::MISC)) {
			string s = GameplayStatics::GetEnumString(static_cast<EItemSlot>(i));
			if (i <= 8 || i == 11) s += "\t  *---> "; if (i == 9) s += " *---> "; if (i == 10) s += "  *---> ";
			if (_item_slots[i]) s += _item_slots[i]->_item_info._name;
			else s += "(empty)";
			v.push_back(s);
			items.push_back(_item_slots[i].get());
			equip_map[equip_index++] = i;
		}
	v.push_back("<--BACK--<");

	if ((input = GameplayStatics::InteractiveDisplay(v, 70, true, items)) == -1) return nullptr;

	std::unique_ptr<Item> item;
	// item is from inventory
	if (input < _n_inventory) {
		item = move(_inventory[input]);
	}
	// item is equiped
	else {
		int start = static_cast<int>(v.size() - equip_map.size());
		int index = input - start + 1;
		item = move(_item_slots[equip_map[index]]);
		bIsEquiped = true;
	}	
	return item;
}

void PlayerCharacter::DisplayStats() {
	system("cls");
	cout << "========     STATS    ============" << endl;
	cout << "==================================" << endl;
	cout << "STR: " << _player_attributes._strength + _i_str << "\nAGI: " << _player_attributes._agility + _i_agi << "\nINT: " << _player_attributes._intelligence + _i_int << endl;
	cout << "VIT: " << _player_attributes._vitality + _i_vit << "\nCON: " << _player_attributes._consciousness + _i_con << "\nEND: " << _player_attributes._endurance + _i_end << endl;
	cout << "MinDmg: " << _min_damage << "\nMaxDmg: " << _max_damage << "\nAvgDmg: " << _avg_damage << endl;
	
	cout << endl << "Press any key to go back.\n";
	auto input = _getch();
	system("cls");
	GameplayStatics::DisplayMapMenuTitle();
}

void constexpr PlayerCharacter::InitExperienceForLevel() {

	int start = 17;
	_experience_next_level[0] = start;

	int constant = 7;
	for (int i = 1; i <= MAX_LVL; i++) {
		if (i % 3 == 0) constant += constant / 2;
		_experience_next_level[i] = static_cast<int>(_experience_next_level[i - 1] + (_experience_next_level[i - 1] * 0.1) + constant);
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

void PlayerCharacter::CalcPlayerItemSlots() {
	_health = _essence = _stamina = _armor = _attack_power = _crit_chance = _crit_damage = _spell_power = _spell_crit_chance = _spell_crit_damage = 0.f;
	_i_str = _i_agi = _i_int = _i_vit = _i_con = _i_end = 0;
	_min_damage = _max_damage = 0;


	for (const auto& item : _item_slots)
		if (item) {
			_min_damage += item->_item_info._min_dmg;
			_max_damage += item->_item_info._max_dmg;
			_armor.UpdateBase(static_cast<float>(item->_item_info._armor));   // check if base works fine in conjunction with max
		}

	_avg_damage = _min_damage && _max_damage ? static_cast<int>((_min_damage + _max_damage) / 2) : 0;

	InitStats();
}

void PlayerCharacter::CalcInvSlots() {
	_n_inventory = 0;
	for (const auto& item : _inventory)
		if (item) ++_n_inventory;
		else break;
}