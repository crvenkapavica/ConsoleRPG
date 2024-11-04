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

PlayerCharacter::PlayerCharacter(PlayerCharacter&& Other) noexcept
	: Character(std::move(Other))
{}

void PlayerCharacter::ReceiveExperience(const int InExperience) {
	Experience += InExperience;
	if (Experience >= ExperienceNextLevel[Level - 1]) {
		Experience -= ExperienceNextLevel[Level - 1];
		LevelUp();
	}
}

void PlayerCharacter::TakeTurn() {
	if (bIsInCombat) {
		GameplayStatics::RedrawGameScreen();
		const int input = GameplayStatics::DisplayCombatMenu(this);
		GameplayStatics::HandleCombatInput(this, input);
	}
}

// TODO : Add Auto-Equip for empty slots
void PlayerCharacter::EquipItem(std::unique_ptr<Item>& InItem) {
	//if (nInventory == INV_SLOTS) return; // treba promeniti da se pita ak je inventory pun samo ako je slot zauzet

	if (InItem->ItemInfo.ItemType == EItemType::WEAPON) {
		if (InItem->ItemInfo.ItemSlot == EItemSlot::WPN_BOTH)
			InItem->ItemInfo.ItemSlot = EItemSlot::WPN_MAIN;
	}

	ItemSlots[static_cast<int>(InItem->ItemInfo.ItemSlot)].swap(InItem);
	if (InItem) AddItemToInventory(InItem);

	SortInventory();
	CalculatePlayerItemSlots();
	CalculateInventorySlots();
}

// TODO: Add Item Swapping
void PlayerCharacter::UnEquipItem(std::unique_ptr<Item>& InItem) {
	if (nInventory == INV_SLOTS) return;
	AddItemToInventory(InItem);
	CalculatePlayerItemSlots();
	CalculateInventorySlots();
}

bool PlayerCharacter::AddItemToInventory(std::unique_ptr<Item>& InItem) {
	for (auto& InvItem : Inventory) {
		if (!InvItem) {
			InvItem = std::move(InItem);
			CalculateInventorySlots();
			return true;
		}
	}
	return false;
}

int PlayerCharacter::GetInventorySpace() const {
	int size = 0;
	for (const auto& item : Inventory)
		if (item) ++size;
	return INV_SLOTS - size;
}

void PlayerCharacter::InspectItem(std::unique_ptr<Item> Item) {}

void PlayerCharacter::DestroyItem(const std::unique_ptr<Item>&& InItem) {
	// TODO : Check reset? (timely InItem destruction)
	CalculatePlayerItemSlots();
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

std::unique_ptr<Item>& PlayerCharacter::DisplayAllItems(OUT bool& bIsEquipped) {

	std::vector<std::string> v = { "ALL ITEMS","RELICS","WEAPONS","JEWELLERY","ARMOR","SCROLLS","CONSUMABLES","<--BACK--<" };
	int Input;
	if ((Input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;
	const auto type = static_cast<EItemType>(ITEM_TYPES - Input);

	v = { "ALL RARITIES","COMMON","RARE","EPIC","LEGENDARY","GODLIKE","UNIQUE","<--BACK--<" };
	if ((Input = GameplayStatics::InteractiveDisplay(v)) == -1) return nullptr;
	const auto rarity = static_cast<EItemRarity>(Input);

	std::map<int, int> ItemMap;
	int ItemIndex = 0;
	int nInv = 0;
	std::vector<Item*> items;
	v.clear();

	// treba auto sortirati da su svi v inventoriju po redu a na kraju nullptr
	for (int i = 0; i < static_cast<int>(Inventory.size()); i++)
		if (!Inventory[i]) {
			if (type == EItemType::MISC && rarity == EItemRarity::MISC) {
				v.emplace_back("INVENTORY ---> (empty)");
				items.push_back(nullptr);
				++nInv;
				ItemMap[ItemIndex++] = i;
			}
		}
		else if ((Inventory[i]->ItemInfo.ItemType == type || type == EItemType::MISC) && (Inventory[i]->ItemInfo.ItemRarity == rarity || rarity == EItemRarity::MISC)) {
			v.push_back("INVENTORY ---> " + Inventory[i]->ItemInfo.Name);
			items.push_back(Inventory[i].get());
			ItemMap[ItemIndex++] = i;
			++nInv;
		}

	for (int i = 0; i < static_cast<int>(ItemSlots.size()); i++)
		if (((ItemSlots[i] && (ItemSlots[i]->ItemInfo.ItemType == type || type == EItemType::MISC)) || type == EItemType::MISC)
			&& ((ItemSlots[i] && (ItemSlots[i]->ItemInfo.ItemRarity == rarity || rarity == EItemRarity::MISC)) || rarity == EItemRarity::MISC)) {
			std::string s = GameplayStatics::GetEnumString(static_cast<EItemSlot>(i));
			if (i <= 8 || i == 11) s += "\t  *---> ";
			if (i == 9) s += " *---> ";
			if (i == 10) s += "  *---> ";
			if (ItemSlots[i]) s += ItemSlots[i]->ItemInfo.Name;
			else s += "(empty)";
			v.push_back(s);
			items.push_back(ItemSlots[i].get());
			ItemMap[ItemIndex++] = i;
		}
	v.emplace_back("<--BACK--<");

	if ((Input = GameplayStatics::InteractiveDisplay(v, 70, true, items)) == -1) return nullptr;

	// item is from inventory
	if (Input < nInv) {
		return std::move(Inventory[ItemMap[Input]]);
	}
	// item is equipped
	else {
		bIsEquipped = true;
		return std::move(ItemSlots[ItemMap[Input]]);
	}

	return nullptr;
}

void PlayerCharacter::DisplayStats() const {
	CLS;
	std::cout << "========     STATS    ============" << '\n';
	std::cout << "==================================" << '\n';
	std::cout << "STR: " << CharacterAttributes.Strength + Item_Strength << "\nAGI: " << CharacterAttributes.Agility + Item_Agility << "\nINT: " << CharacterAttributes.Intelligence + Item_Intelligence << '\n';
	std::cout << "VIT: " << CharacterAttributes.Vitality + Item_Vitality << "\nCON: " << CharacterAttributes.Consciousness + Item_Consciousness << "\nEND: " << CharacterAttributes.Endurance + Item_Endurance << '\n';
	std::cout << "MinDmg: " << MinDamage << "\nMaxDmg: " << MaxDamage << "\nAvgDmg: " << AvgDamage << '\n';

	std::cout << '\n' << "Press any key to go back.\n";
	auto input = _getch();
	CLS;
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
	std::ranges::sort(Inventory, [&](const std::unique_ptr<Item>& First, const std::unique_ptr<Item>& Second) { return First && !Second; });
}

void PlayerCharacter::CalculatePlayerItemSlots() {
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