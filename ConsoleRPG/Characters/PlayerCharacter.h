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
	std::vector<Spell*> _spell_slots;

	// EXTRA STATS
	///////////////////////////////////////////
	// combat
	int _multi_strike = 0;
	float _fire_damage = 0.f;
	int _buff_duration = 0;
	int _debuff_duration = 0;

	float critical_damage = 0.f;

	// out of combat
	float _magic_find = 0.f;
	int _light_radius = 0;
	int _num_cons_slots = 0;
	int _num_spell_slots = 0;
	///////////////////////////////////////////

protected:

	int _unspent_attributes = 0;
	int _experience = 0;

	int _experience_next_level[MAX_LVL] = { 0 };

private:
	void InitExperienceForLevel();

	void LevelUp();
};
