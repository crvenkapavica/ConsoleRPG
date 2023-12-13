#pragma once

#include "../RPGTypes.h"
#include "Character.h"
#include "../Inventory/Item.h"
#include "../Effects/ActiveSpell.h"
#include "../Effects/PassiveSpell.h"


class PlayerCharacter : public Character {

public:

	PlayerCharacter(const CharacterData::PlayerAttributes& attributes);

	PlayerCharacter(const PlayerCharacter& other) : Character(other) {}

	PlayerCharacter& operator=(const PlayerCharacter& other) {
		if (this != &other) {
			Character::operator=(other);
		}
		return *this;
	}

public:

	/// remove
	inline float GetMagicFind() const { return _magic_find; }
	inline int GetLighRadius() const { return _light_radius; }
	////

	void ReceiveExperience(const int experience);

	virtual void TakeTurn() override;


	//// INVENTORY
	///////////////////////////////////////////////////////////////////////////////////////////////
public:
	void EquipItem(unique_ptr<Item> item);

	// Return immediately if inventory is full
	void UnEquipItem(unique_ptr<Item> item);

	// Return true if the item was added, false otherwise
	bool AddItemToInventory(unique_ptr<Item> item);

	// Displays all information about the item and its affixes
	void InspectItem(Item* item);
	
	Item* DisplayEquipedItems();
	Item* DisplayInventory();
	Item* DisplayConsumableSlots();
	ActiveSpell* DisplayActiveSpellSlots();
	PassiveSpell* DisplayPassiveSpellSlots();
	unique_ptr<Item> DisplayAllItems(bool& bIsEquiped);

public:
	std::vector<unique_ptr<Item>> _item_slots;
	std::vector<unique_ptr<Item>> _inventory;
	std::vector<unique_ptr<Item>> _consumable_slots;
	std::vector<unique_ptr<ActiveSpell>> _active_slots;
	std::vector<unique_ptr<PassiveSpell>> _passive_slots;
	///////////////////////////////////////////////////////////////////////////////////////////////

protected:

	int _n_inventory = 0;

	int _unspent_attributes = 0;
	int _experience = 0;

	int _experience_next_level[MAX_LVL] = { 0 };

private:
	void InitExperienceForLevel();

	void LevelUp();
};
