#pragma once

#include "../RPGTypes.h"
#include "Character.h"
#include "PlayerCharacter.h"

class EnemyCharacter : public Character { 

public:
	EnemyCharacter(const CharacterData::EnemyStats& data);

	EnemyCharacter& operator=(const EnemyCharacter& other) {
		if (this != &other) {
			Character::operator=(other);
		}
		return *this;
	}

	virtual void TakeTurn() override;

private:

	vector<weak_ptr<PlayerCharacter>> _player_characters;
	vector<weak_ptr<EnemyCharacter>> _enemy_characters;

	int _level;
	int _count;

private:

	void CastSpell();
};