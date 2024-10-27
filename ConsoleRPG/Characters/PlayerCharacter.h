#pragma once

#include "Character.h"
#include "../Items/Item.h"
#include "../Spells/ActiveSpell.h"
#include "../Spells/PassiveSpell.h"

class PlayerCharacter : public Character {

public:
	
	explicit PlayerCharacter(ECharacterClass character_class);

	PlayerCharacter(const PlayerCharacter& Other);

	PlayerCharacter(PlayerCharacter&& Player) noexcept;

	~PlayerCharacter() override;

	PlayerCharacter& operator=(const PlayerCharacter& other) {
		if (this != &other) {
			Character::operator=(other);
		}
		//++_n;
		return *this;
	}

	static int _n;;

public:

	/// remove
	inline float GetMagicFind() const { return _magic_find; }
	inline int GetLightRadius() const { return _light_radius; }
	////

	void ReceiveExperience(const int Experience);

	virtual void TakeTurn() override;


	//// INVENTORY
	///////////////////////////////////////////////////////////////////////////////////////////////
public:
	void EquipItem(unique_ptr<Item>& Item);

	// Return immediately if inventory is full
	void UnEquipItem(unique_ptr<Item>&& InItem);

	// Return true if the item was added, false otherwise
	bool AddItemToInventory(unique_ptr<Item>&& Item);

	[[nodiscard]] int GetInventorySpace() const;

	// Displays all information about the item and its affixes
	void InspectItem(Item* item);

	void DestroyItem(unique_ptr<Item>* Item);
	
	void DisplayEquippedItems() const;
	[[nodiscard]] Item* DisplayInventory() const;
	[[nodiscard]] const Item* DisplayConsumableSlots(Item* Item1) const;
	[[nodiscard]] ActiveSpell* DisplayActiveSpellSlots() const;
	[[nodiscard]] PassiveSpell* DisplayPassiveSpellSlots() const;
	unique_ptr<Item> DisplayAllItems(OUT bool& bIsEquipped);
	void DisplayStats();

public:
	std::vector<unique_ptr<Item>> _item_slots;
	std::vector<unique_ptr<Item>> _inventory;
	std::vector<unique_ptr<Item>> _consumable_slots;
	std::vector<unique_ptr<ActiveSpell>> _active_slots;
	std::vector<unique_ptr<PassiveSpell>> _passive_slots;
	///////////////////////////////////////////////////////////////////////////////////////////////

protected:

	int nInventory = 0;

	int UnspentAttributes = 0;
	int Experience = 0;

	int ExperienceNextLevel[MAX_LVL] = { 0 };

protected:
	void constexpr InitExperienceForLevel();

	void LevelUp();

	void SortInventory();

	void CalculatePlayerItemSlots();

	void CalculateInventorySlots();
};
