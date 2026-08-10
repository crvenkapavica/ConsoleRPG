#include "PlayerCharacter.h"
#include "../GameplayStatics.h"
#include "../Characters/CharacterData.h"
#include "../Items/ItemData.h"

int PlayerCharacter::nPlayerCharacters = 0;

PlayerCharacter::PlayerCharacter(const ECharacterClass InCharacterClass)
	: Character(CharDb::Data[InCharacterClass]
	, CharDb::Attributes[InCharacterClass]
	, '0' + static_cast<char>(nPlayerCharacters++))
{
	InitExperienceForLevel();

	ItemSlots.resize(ITEM_SLOTS);
	Inventory.resize(INV_SLOTS);

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

void PlayerCharacter::ReceiveExperience(const int InExperience) {
	Experience += InExperience;
	if (Experience >= ExperienceNextLevel[Level - 1]) {
		Experience -= ExperienceNextLevel[Level - 1];
		LevelUp();
	}
}

void PlayerCharacter::TakeTurn() {
	GameplayStatics::RedrawGameScreen();
	const int input = GameplayStatics::DisplayCombatMenu(this);
	GameplayStatics::HandleCombatInput(this, input);
}

// TODO : Add Auto-Equip for empty slots
void PlayerCharacter::EquipItem(std::unique_ptr<Item>&& InItem) {
	//if (nInventory == nInventoryMax) return; // treba promeniti da se pita ak je inventory pun samo ako je slot zauzet

	if (InItem->ItemInfo.ItemType == EItemType::WEAPON) {
		if (InItem->ItemInfo.ItemSlot == EItemSlot::WPN_BOTH)
			InItem->ItemInfo.ItemSlot = EItemSlot::WPN_MAIN;
	}

	ItemSlots[static_cast<int>(InItem->ItemInfo.ItemSlot)].swap(InItem);
	if (InItem) AddItemToInventory(std::move(InItem));

	SortInventory();
	CalculateItemSlots();
	CalculateInventorySlots();
}

// TODO: Add Item Swapping
void PlayerCharacter::UnEquipItem(std::unique_ptr<Item>&& InItem) {
	//if (nInventory == nInventoryMax) return;
	if (nInventory == INV_SLOTS) return;
	AddItemToInventory(std::move(InItem));
	CalculateItemSlots();
	CalculateInventorySlots();
}

void PlayerCharacter::AddItemToInventory(std::unique_ptr<Item>&& InItem) {
	for (auto& InvItem : Inventory)
		if (!InvItem) {
			InvItem = std::move(InItem);
			CalculateInventorySlots();
		}
}

void PlayerCharacter::InspectItem(std::unique_ptr<Item> Item) {}

void PlayerCharacter::DestroyItem(const std::unique_ptr<Item>&& InItem) {
	CalculateItemSlots();
	SortInventory();
	CalculateInventorySlots();
}

void PlayerCharacter::DisplayEquippedItems() const {
	for (const auto& item : ItemSlots)
		if (item) std::cout << GameplayStatics::GetEnumString(item->ItemInfo.ItemSlot) << " --> " << item->ItemInfo.Name << '\n';
}

void PlayerCharacter::DisplayInventory() {}

void PlayerCharacter::DisplayConsumableSlots() {}

void PlayerCharacter::DisplayActiveSpellSlots() {}

void PlayerCharacter::DisplayPassiveSpellSlots() {}

std::unique_ptr<Item> PlayerCharacter::DisplayAllItems(OUT bool& bIsEquipped) {
	std::vector<std::string> Menu = {"ALL ITEMS", "RELICS", "WEAPONS", "JEWELLERY", "ARMOR", "SCROLLS", "CONSUMABLES", "<--BACK--<"};
	int Input;
	// Item type filter
	if ((Input = GameplayStatics::InteractiveDisplay(Menu)) == -1) return nullptr;  
	const auto Type = static_cast<EItemType>(ITEM_TYPES - Input);

	Menu = {"ALL RARITIES", "COMMON", "RARE", "EPIC", "LEGENDARY", "GODLIKE", "UNIQUE", "<--BACK--<"};
	// Rarity filter
	if ((Input = GameplayStatics::InteractiveDisplay(Menu)) == -1) return nullptr;  
	const auto Rarity = static_cast<EItemRarity>(Input);

	std::map<int, int> ItemMap;
	int ItemIndex = 0;
	int nInv = 0;
	std::vector<Item*> Items;
	Menu.clear();
	
	// Display all inventory item selections
	// Inventory is auto-sorted upon any equips/un-equips, so taken selections will always be on top
	for (int i = 0; i < static_cast<int>(Inventory.size()); i++)
		// Display all (empty) inventory item selections
		if (!Inventory[i]) {
			if (Type == EItemType::MISC && Rarity == EItemRarity::MISC) {
				Menu.emplace_back("INVENTORY ---> (empty)");
				Items.push_back(nullptr);
				++nInv;
				ItemMap[ItemIndex++] = i;
			}
		}
		// Display all taken inventory item selections 
		else if ((Inventory[i]->ItemInfo.ItemType == Type || Type == EItemType::MISC)
			&& (Inventory[i]->ItemInfo.ItemRarity == Rarity || Rarity == EItemRarity::MISC)) {
			Menu.emplace_back("INVENTORY ---> " + Inventory[i]->ItemInfo.Name);
			Items.push_back(Inventory[i].get());
			++nInv;
			ItemMap[ItemIndex++] = i;
		}

	// Display all equipped (slot) items
	for (int i = 0; i < static_cast<int>(ItemSlots.size()); i++)
		if (((ItemSlots[i] && (ItemSlots[i]->ItemInfo.ItemType == Type || Type == EItemType::MISC)) || Type == EItemType::MISC)
			&& ((ItemSlots[i] && (ItemSlots[i]->ItemInfo.ItemRarity == Rarity || Rarity == EItemRarity::MISC)) || Rarity == EItemRarity::MISC)) {
			std::string SelectionName = GameplayStatics::GetEnumString(static_cast<EItemSlot>(i));
			if (i <= 8 || i == 11) SelectionName += "\t  *---> ";
			if (i == 9) SelectionName += " *---> ";
			if (i == 10) SelectionName += "  *---> ";
			// If there is an item equipped at this slot, append its name
			if (ItemSlots[i]) SelectionName += ItemSlots[i]->ItemInfo.Name;
			else SelectionName += "(empty)";
			Menu.push_back(SelectionName);
			Items.push_back(ItemSlots[i].get());
			ItemMap[ItemIndex++] = i;
		}
	Menu.emplace_back("<--BACK--<");

	if ((Input = GameplayStatics::InteractiveDisplay(Menu, 70, true, Items)) == -1) return nullptr;
	// Item is from inventory
	if (Input < nInv) return std::move(Inventory[ItemMap[Input]]);
	// Item is equipped
	else { bIsEquipped = true; return std::move(ItemSlots[ItemMap[Input]]); }
	return nullptr;
}

void PlayerCharacter::DisplayStats() const {
	CLS
	std::cout << "========     STATS    ============" << '\n';
	std::cout << "==================================" << '\n';
	std::cout << "STR: " << CharacterAttributes.Strength + Item_Strength << "\nAGI: " << CharacterAttributes.Agility + Item_Agility << "\nINT: " << CharacterAttributes.Intelligence + Item_Intelligence << '\n';
	std::cout << "VIT: " << CharacterAttributes.Vitality + Item_Vitality << "\nCON: " << CharacterAttributes.Consciousness + Item_Consciousness << "\nEND: " << CharacterAttributes.Endurance + Item_Endurance << '\n';
	std::cout << "MinDmg: " << MinDamage << "\nMaxDmg: " << MaxDamage << "\nAvgDmg: " << AvgDamage << '\n';

	std::cout << '\n' << "Press any key to go back.\n";
	_getch();
	CLS
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
	++Level;
	// print you have leveled up!
	nUnspentAttributes += 2;
}

void PlayerCharacter::SortInventory() {
	std::ranges::sort(Inventory, [](const std::unique_ptr<Item>& First, const std::unique_ptr<Item>& Second) { return First && !Second; });
}

void PlayerCharacter::CalculateItemSlots() {
	Health = Essence = Stamina = Armor = AttackPower = CritChance = CritDamage = SpellPower = SpellCritChance = SpellCritDamage = 0.f;
	Item_Strength = Item_Agility = Item_Intelligence = Item_Vitality = Item_Consciousness = Item_Endurance = 0;
	MinDamage = MaxDamage = 0;
	
	for (const auto& SlottedItem : ItemSlots)
		if (SlottedItem) {
			MinDamage += SlottedItem->ItemInfo.MinDmg;
			MaxDamage += SlottedItem->ItemInfo.MaxDmg;
			Armor.UpdateBase(static_cast<float>(SlottedItem->ItemInfo.Armor));   // check if base works fine in conjunction with max
		}

	AvgDamage = MinDamage && MaxDamage ? static_cast<int>((MinDamage + MaxDamage) / 2) : 0;

	InitStats();
}

void PlayerCharacter::CalculateInventorySlots() {
	nInventory = 0;
	for (const auto& InventoryItem : Inventory)
		if (InventoryItem) ++nInventory;
		else break;
}