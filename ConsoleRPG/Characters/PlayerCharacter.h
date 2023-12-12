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
	unique_ptr<Item> EquipItem(unique_ptr<Item> item);
	void UnEquipItem(unique_ptr<Item> item);
	
	void DisplayEquipedItems();
	void DisplayInventory();
	void DisplaySpellSlots();
	void DisplayConsumableSlots();
	unique_ptr<Item> DisplayAllItems(EItemType type, EItemRarity rarity);
	void InspectItem(unique_ptr<Item> item);


	std::vector<unique_ptr<Item>> _item_slots; //= std::vector<unique_ptr<Item>>(ITEM_SLOTS, nullptr);
	std::vector<unique_ptr<Item>> _inventory; //= std::vector<unique_ptr<Item>>(INV_SLOTS, nullptr);
	std::vector<unique_ptr<Item>> _consumable_slots;
	std::vector<unique_ptr<ActiveSpell>> _active_slots;
	std::vector<unique_ptr<PassiveSpell>> _passive_slots;
	///////////////////////////////////////////////////////////////////////////////////////////////
protected:

	int _unspent_attributes = 0;
	int _experience = 0;

	int _experience_next_level[MAX_LVL] = { 0 };

private:
	void InitExperienceForLevel();

	void LevelUp();
};
