#pragma once

#include "../RPGTypes.h"
#include "Character.h"

class Item;

class PlayerCharacter : public Character {

public:

	PlayerCharacter(const CharacterData::PlayerStats& data, const CharacterData::PlayerAttributes& attributes);

	PlayerCharacter(const PlayerCharacter& other) : Character(other) {}

	PlayerCharacter& operator=(const PlayerCharacter& other) {
		if (this != &other) {
			Character::operator=(other);
		}
		return *this;
	}

public:

	void ReceiveExperience(const int experience);

	int GetLevel() const { return _level; }

	virtual void TakeTurn() override;

	std::vector<Item*> _item_slots = std::vector<Item*>(ITEM_SLOTS, nullptr);
	std::vector<Item*> _inventory = std::vector<Item*>(INV_SLOTS, nullptr);
	std::vector<Item*> _cons_slots;
	std::vector<Spell*> _spell_slots;

protected:

	// Player additional stats
	int _multi_strike = 0;

	float _fire_damage = 0.f;


	int _buff_duration = 0;
	int _debuff_dureation = 0;

	float _magic_find = 0.f;
	int _light_radius = 0;
	//////////////////////////////////////////////////////////

	int _unspent_attributes;
	int _experience;

	int _experience_next_level[MAX_LVL] = { 0 };

private:
	void InitExperienceForLevel();

	void LevelUp();
};
