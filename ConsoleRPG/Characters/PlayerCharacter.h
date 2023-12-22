#pragma once

#include "Character.h"
#include "../Items/Item.h"
#include "../Spells/ActiveSpell.h"
#include "../Spells/PassiveSpell.h"

class PlayerCharacter : public Character {

public:

	PlayerCharacter(ECharacterClass character_class);

	PlayerCharacter(const PlayerCharacter& other);

	PlayerCharacter(PlayerCharacter&& other) noexcept;

	~PlayerCharacter();

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

	void DestroyItem(unique_ptr<Item> item);
	
	Item* DisplayEquipedItems();
	Item* DisplayInventory();
	Item* DisplayConsumableSlots();
	ActiveSpell* DisplayActiveSpellSlots();
	PassiveSpell* DisplayPassiveSpellSlots();
	unique_ptr<Item> DisplayAllItems(OUT bool& bIsEquiped);
	void DisplayStats();

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

protected:
	void constexpr InitExperienceForLevel();

	void LevelUp();

	void SortInventory();

	void CalcPlayerItemSlots();

	void CalcInvSlots();
};
