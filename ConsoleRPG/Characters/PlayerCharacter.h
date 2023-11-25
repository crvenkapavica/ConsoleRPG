#pragma once

#include "Character.h"

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

	inline Stat GetMagicFind() const { return _magic_find; }
	inline Stat GetLighRadius() const { return _light_radius; }

	virtual void TakeTurn() override;

protected:

	Stat _magic_find;
	Stat _light_radius;

	int _unspent_attributes;
	int _experience;

	int _experience_next_level[MAX_LVL] = { 0 };

private:
	void InitExperienceForLevel();

	void LevelUp();
};
