#include "PlayerCharacter.h"
#include "../GameplayStatics.h"
#include "../Items/ItemData.h"
#include "../Characters/CharacterData.h"

int PlayerCharacter::_n = 0;

PlayerCharacter::PlayerCharacter(ECharacterClass character_class)
	: Character(CharDb::Data[character_class], CharDb::Attributes[character_class], '0' + _n++)
{
	InitExperienceForLevel();

	_item_slots.resize(ITEM_SLOTS);
	_inventory.resize(INV_SLOTS);

	//unique_ptr<Item> item = Item::CreateItem(_lvl, _magic_find, EItemType::WEAPON);
	//AddItemToInventory(item);

	/*auto items = Item::GenerateLoot(this, 125);
	for (auto& item : items)
		AddItemToInventory(move(item));*/

	AddItemToInventory(Item::CreateItemById(EItemId::HatchAxe));
	AddItemToInventory(Item::CreateItemById(EItemId::AssNeedle));
	AddItemToInventory(Item::CreateItemById(EItemId::HatchAxe));
	AddItemToInventory(Item::CreateItemById(EItemId::AssNeedle));
	AddItemToInventory(Item::CreateItemById(EItemId::HatchAxe));
	AddItemToInventory(Item::CreateItemById(EItemId::AssNeedle));
}

PlayerCharacter::PlayerCharacter(const PlayerCharacter& Other) 
	: Character(Other) 
{}

PlayerCharacter::PlayerCharacter(PlayerCharacter&& Player) noexcept
	: Character(Player)
{}

PlayerCharacter::~PlayerCharacter() = default;

void PlayerCharacter::ReceiveExperience(const int Experience) {
	Experience += Experience;
	if (Experience >= ExperienceNextLevel[_lvl - 1]) {
		Experience -= ExperienceNextLevel[_lvl - 1];
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

void PlayerCharacter::EquipItem(unique_ptr<Item>& Item) {
	if (!Item || nInventory == INV_SLOTS) return; // treba promeniti da se pita ak je inventory pun samo ako je slot zauzet

	if (Item->ItemInfo.ItemType == EItemType::WEAPON) {
		if (Item->ItemInfo.ItemSlot == EItemSlot::WPN_BOTH)
			Item->ItemInfo.ItemSlot = EItemSlot::WPN_MAIN;
	}

	_item_slots[static_cast<int>(Item->ItemInfo.ItemSlot)].swap(Item);
	if (Item) AddItemToInventory(std::move(Item));

	SortInventory();
	CalculatePlayerItemSlots();
	CalculateInventorySlots();
}

void PlayerCharacter::UnEquipItem(unique_ptr<Item>&& InItem) {
	if (!InItem || nInventory == INV_SLOTS) return;

	AddItemToInventory(std::move(InItem));
	CalculatePlayerItemSlots();
	CalculateInventorySlots();
}

bool PlayerCharacter::AddItemToInventory(unique_ptr<Item>&& Item) {	

	for (auto& InvItem : _inventory) {
		if (!InvItem) {
			InvItem = std::move(Item);
			CalculateInventorySlots();
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

void PlayerCharacter::DestroyItem(unique_ptr<Item>* Item) {
	Item.reset();

	SortInventory();
	CalculatePlayerItemSlots();
	CalculateInventorySlots();
}

void PlayerCharacter::DisplayEquippedItems() const {
	for (const auto& item : _item_slots) 
		if (item) cout << GameplayStatics::GetEnumString(item->ItemInfo.ItemSlot) << " --> " << item->ItemInfo.Name << '\n';
}

Item* PlayerCharacter::DisplayInventory() const {
	for (int i = 0; i < _inventory.size(); i++)
		if (_inventory[i])
			cout << i << ".) " << _inventory[i]->ItemInfo.Name << "  " << GameplayStatics::GetEnumString(_inventory[i]->ItemInfo.ItemSlot) << '\n';

	cout << '\n' << "Press any key to go back.";
	cin.get();

	return nullptr;
}

const Item* PlayerCharacter::DisplayConsumableSlots(Item* Item1) const {
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

	map<int, int> ItemMap;
	int ItemIndex = 0;
	int nInv = 0;
	std::vector<Item*> items;
	v.clear();
	// treba auto sortirati da su svi v inventoriju po redu a na kraju nullptr
	for (std::size_t i = 0; i < _inventory.size(); i++)
		if (!_inventory[i]) {
			if (type == EItemType::MISC && rarity == EItemRarity::MISC) {
				v.emplace_back("INVENTORY ---> (empty)");
				items.push_back(nullptr);
				++nInv;
				ItemMap[ItemIndex++] = i;
			}
		}
		else if ((_inventory[i]->ItemInfo.ItemType == type || type == EItemType::MISC) && (_inventory[i]->ItemInfo.ItemRarity == rarity || rarity == EItemRarity::MISC)) {
			v.push_back("INVENTORY ---> " + _inventory[i]->ItemInfo.Name);
			items.push_back(_inventory[i].get());
			ItemMap[ItemIndex++] = i;
			++nInv;
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
			ItemMap[ItemIndex++] = i;
		}
	v.emplace_back("<--BACK--<");

	if ((input = GameplayStatics::InteractiveDisplay(v, 70, true, items)) == -1) return nullptr;

	// item is from inventory
	if (input < nInv) {
		return std::move(_inventory[ItemMap[input]]);
	}
	// item is equipped
	else {
		bIsEquipped = true;
		return std::move(_item_slots[ItemMap[input]]);
	}

	return nullptr;
}

void PlayerCharacter::DisplayStats() {
	system("cls");
	cout << "========     STATS    ============" << '\n';
	cout << "==================================" << '\n';
	cout << "STR: " << _player_attributes.Strength + _i_str << "\nAGI: " << _player_attributes.Agility + _i_agi << "\nINT: " << _player_attributes.Intelligence + _i_int << '\n';
	cout << "VIT: " << _player_attributes.Vitality + _i_vit << "\nCON: " << _player_attributes.Consciousness + _i_con << "\nEND: " << _player_attributes.Endurance + _i_end << '\n';
	cout << "MinDmg: " << _min_damage << "\nMaxDmg: " << _max_damage << "\nAvgDmg: " << _avg_damage << '\n';
	
	cout << '\n' << "Press any key to go back.\n";
	auto input = _getch();
	system("cls");
	GameplayStatics::DisplayMapMenuTitle();
}

void constexpr PlayerCharacter::InitExperienceForLevel() {

	constexpr int start = 17;
	ExperienceNextLevel[0] = start;

	int constant = 7;
	for (int i = 1; i <= MAX_LVL; i++) {
		if (i % 3 == 0) constant += constant / 2;
		ExperienceNextLevel[i] = static_cast<int>(ExperienceNextLevel[i - 1] + (ExperienceNextLevel[i - 1] * 0.1) + constant);
	}
}

void PlayerCharacter::LevelUp() {
	++_lvl;
	// print you have leveled up!
	UnspentAttributes += 2;
}

void PlayerCharacter::SortInventory() {
	ranges::sort(_inventory, [&](const std::unique_ptr<Item>& First, const std::unique_ptr<Item>& Second) { return First && !Second; });
}

void PlayerCharacter::CalculatePlayerItemSlots() {
	_health = _essence = _stamina = _armor = _attack_power = _crit_chance = _crit_damage = _spell_power = _spell_crit_chance = _spell_crit_damage = 0.f;
	_i_str = _i_agi = _i_int = _i_vit = _i_con = _i_end = 0;
	_min_damage = _max_damage = 0;


	for (const auto& item : _item_slots)
		if (item) {
			_min_damage += item->ItemInfo.MinDmg;
			_max_damage += item->ItemInfo.MaxDmg;
			_armor.UpdateBase(static_cast<float>(item->ItemInfo.Armor));   // check if base works fine in conjunction with max
		}

	_avg_damage = _min_damage && _max_damage ? static_cast<int>((_min_damage + _max_damage) / 2) : 0;

	InitStats();
}

void PlayerCharacter::CalculateInventorySlots() {
	nInventory = 0;
	for (const auto& item : _inventory)
		if (item) ++nInventory;
		else break;
}