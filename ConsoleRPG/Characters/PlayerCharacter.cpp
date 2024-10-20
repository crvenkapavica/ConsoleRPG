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

	AddItemToInventory(Item::CreateItemById(EItemID::HatchAxe));
	AddItemToInventory(Item::CreateItemById(EItemID::AssNeedle));
	AddItemToInventory(Item::CreateItemById(EItemID::HatchAxe));
	AddItemToInventory(Item::CreateItemById(EItemID::AssNeedle));
	AddItemToInventory(Item::CreateItemById(EItemID::HatchAxe));
	AddItemToInventory(Item::CreateItemById(EItemID::AssNeedle));
}

PlayerCharacter::PlayerCharacter(const PlayerCharacter& other) 
	: Character(other) 
{}

PlayerCharacter::PlayerCharacter(PlayerCharacter&& player) noexcept
	: Character(player)
{}

PlayerCharacter::~PlayerCharacter() = default;

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
		const int input = GameplayStatics::DisplayCombatMenu(this);
		GameplayStatics::HandleCombatInput(this, input);
	}
}

void PlayerCharacter::EquipItem(unique_ptr<Item> item) {
	if (!item || _n_inventory == INV_SLOTS) return; // treba promeniti da se pita ak je inventory pun samo ako je slot zauzet

	if (item->ItemInfo.ItemType == EItemType::WEAPON) {
		if (item->ItemInfo.ItemSlot == EItemSlot::WPN_BOTH)
			item->ItemInfo.ItemSlot = EItemSlot::WPN_MAIN;
	}

	_item_slots[static_cast<int>(item->ItemInfo.ItemSlot)].swap(item);
	if (item) AddItemToInventory(std::move(item));

	SortInventory();
	CalcPlayerItemSlots();
	CalcInvSlots();
}

void PlayerCharacter::UnEquipItem(unique_ptr<Item> item) {
	if (!item || _n_inventory == INV_SLOTS) return;

	AddItemToInventory(std::move(item));
	CalcPlayerItemSlots();
	CalcInvSlots();
}

bool PlayerCharacter::AddItemToInventory(unique_ptr<Item> item) {	

	for (auto& inv_item : _inventory) {
		if (!inv_item) {
			inv_item = std::move(item);
			CalcInvSlots();
			return true;
		}
	}

	return false;
}

int PlayerCharacter::GetInventorySpace() const {
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

Item* PlayerCharacter::DisplayEquippedItems() const {
	for (const auto& item : _item_slots) 
		if (item) cout << GameplayStatics::GetEnumString(item->ItemInfo.ItemSlot) << " --> " << item->ItemInfo.Name << '\n';

	return nullptr;
}

Item* PlayerCharacter::DisplayInventory() const {
	for (int i = 0; i < _inventory.size(); i++)
		if (_inventory[i])
			cout << i << ".) " << _inventory[i]->ItemInfo.Name << "  " << GameplayStatics::GetEnumString(_inventory[i]->ItemInfo.ItemSlot) << '\n';

	cout << '\n' << "Press any key to go back.";
	cin.get();

	return nullptr;
}

Item* PlayerCharacter::DisplayConsumableSlots() const {
	return nullptr;
}

ActiveSpell* PlayerCharacter::DisplayActiveSpellSlots() const {
	return nullptr;
}

PassiveSpell* PlayerCharacter::DisplayPassiveSpellSlots() const {
	return nullptr;
}

std::unique_ptr<Item> PlayerCharacter::DisplayAllItems(OUT bool& bIsEquipped) {

	vector<string> v = { "ALL ITEMS", "RELICS", "WEAPONS", "JEWELLERY", "ARMOR", "SCROLLS", "CONSUMABLES", "<--BACK--<" };
	int input;
	if ((input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;
	const auto type = static_cast<EItemType>(ITEM_TYPES - input);

	v = { "ALL RARITIES", "COMMON", "RARE", "EPIC", "LEGENDARY", "GODLIKE", "UNIQUE", "<--BACK--<" };
	if ((input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;
	const auto rarity = static_cast<EItemRarity>(input);

	map<int, int> item_map;
	int item_index = 0;
	int n_inv = 0;
	std::vector<Item*> items;
	v.clear();
	// treba auto sortirati da su svi v inventoriju po redu a na kraju nullptr
	for (std::size_t i = 0; i < _inventory.size(); i++)
		if (!_inventory[i]) {
			if (type == EItemType::MISC && rarity == EItemRarity::MISC) {
				v.emplace_back("INVENTORY ---> (empty)");
				items.push_back(nullptr);
				++n_inv;
				item_map[item_index++] = i;
			}
		}
		else if ((_inventory[i]->ItemInfo.ItemType == type || type == EItemType::MISC) && (_inventory[i]->ItemInfo.ItemRarity == rarity || rarity == EItemRarity::MISC)) {
			v.push_back("INVENTORY ---> " + _inventory[i]->ItemInfo.Name);
			items.push_back(_inventory[i].get());
			item_map[item_index++] = i;
			++n_inv;
		}
	
	for (int i = 0; i < _item_slots.size(); i++)
		if (((_item_slots[i] && (_item_slots[i]->ItemInfo.ItemType == type || type == EItemType::MISC)) || type == EItemType::MISC)
			&&
			((_item_slots[i] && (_item_slots[i]->ItemInfo.ItemRarity == rarity || rarity == EItemRarity::MISC)) || rarity == EItemRarity::MISC)) {
			string s = GameplayStatics::GetEnumString(static_cast<EItemSlot>(i));
			if (i <= 8 || i == 11) s += "\t  *---> "; if (i == 9) s += " *---> "; if (i == 10) s += "  *---> ";
			if (_item_slots[i]) s += _item_slots[i]->ItemInfo.Name;
			else s += "(empty)";
			v.push_back(s);
			items.push_back(_item_slots[i].get());
			item_map[item_index++] = i;
		}
	v.emplace_back("<--BACK--<");

	if ((input = GameplayStatics::InteractiveDisplay(v, 70, true, items)) == -1) return nullptr;

	// item is from inventory
	if (input < n_inv) {
		return std::move(_inventory[item_map[input]]);
	}
	// item is equiped
	else {
		bIsEquipped = true;
		return std::move(_item_slots[item_map[input]]);
	}

	return nullptr;
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
			_min_damage += item->ItemInfo.min_dmg;
			_max_damage += item->ItemInfo.MaxDmg;
			_armor.UpdateBase(static_cast<float>(item->ItemInfo.Armor));   // check if base works fine in conjunction with max
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