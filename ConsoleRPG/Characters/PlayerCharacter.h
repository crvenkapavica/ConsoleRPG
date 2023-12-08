#pragma once

#include "../RPGTypes.h"
#include "Character.h"

class Item;

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

	std::vector<Item*> _item_slots = std::vector<Item*>(ITEM_SLOTS, nullptr);
	std::vector<Item*> _inventory = std::vector<Item*>(INV_SLOTS, nullptr);
	std::vector<Item*> _cons_slots;
	std::vector<ActiveSpell*> _active_slots;
	std::vector<PassiveSpell*> _passive_slots;

protected:

	int _unspent_attributes = 0;
	int _experience = 0;

	int _experience_next_level[MAX_LVL] = { 0 };

private:
	void InitExperienceForLevel();

	void LevelUp();
};
