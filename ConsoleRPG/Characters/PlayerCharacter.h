#pragma once

#include "Character.h"
#include "../Items/Item.h"
#include "../Spells/ActiveSpell.h"
#include "../Spells/PassiveSpell.h"

class PlayerCharacter : public Character {

public:
	explicit PlayerCharacter(const ECharacterClass CharacterClass);

	PlayerCharacter(const PlayerCharacter& Other);

	PlayerCharacter(PlayerCharacter&& Player) noexcept;

	~PlayerCharacter() override = default;

	PlayerCharacter& operator=(const PlayerCharacter& Other) {
		if (this != &Other) {
			Character::operator=(Other);
		}
		//++_n;
		return *this;
	}

	static int N;

public:
	/// remove
	inline float GetMagicFind() const { return MagicFind; }
	inline int GetLightRadius() const { return LightRadius; }
	////

	void ReceiveExperience(const int InExperience);

	virtual void TakeTurn() override;


	//// INVENTORY
	///////////////////////////////////////////////////////////////////////////////////////////////
public:
	void EquipItem(const std::unique_ptr<Item>&& InItem);

	// Return immediately if inventory is full
	void UnEquipItem(const std::unique_ptr<Item>& InItem);

	// Return true if the item was added, false otherwise
	bool AddItemToInventory(std::unique_ptr<Item>&& InItem);

	[[nodiscard]] int GetInventorySpace() const;

	// Displays all information about the item and its affixes
	static void InspectItem(std::unique_ptr<Item> Item);

	void DestroyItem(const std::unique_ptr<Item>&& InItem);
	
	void DisplayEquippedItems() const;
	[[nodiscard]] Item* DisplayInventory() const;
	[[nodiscard]] static const Item* DisplayConsumableSlots();
	static void DisplayActiveSpellSlots();
	static void DisplayPassiveSpellSlots();
	std::unique_ptr<Item> DisplayAllItems(OUT bool& bIsEquipped);
	void DisplayStats() const;

public:
	std::vector<std::unique_ptr<Item>> ItemSlots;
	std::vector<std::unique_ptr<Item>> Inventory;
	std::vector<std::unique_ptr<Item>> ConsumableSlots;
	std::vector<std::unique_ptr<ActiveSpell>> ActiveSlots;
	std::vector<std::unique_ptr<PassiveSpell>> PassiveSlots;
	///////////////////////////////////////////////////////////////////////////////////////////////

protected:
	int nInventory = 0;
	int nUnspentAttributes = 0;
	int Experience = 0;
	int ExperienceNextLevel[MAX_LVL] = { 0 };

protected:
	void constexpr InitExperienceForLevel();

	void LevelUp();

	void SortInventory();

	void CalculatePlayerItemSlots();

	void CalculateInventorySlots();
};
