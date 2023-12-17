#pragma once

#include "Character.h"

class EnemyCharacter : public Character { 

public:
	EnemyCharacter(const CharacterData& data);

	EnemyCharacter& operator=(const EnemyCharacter& other) {
		if (this != &other) {
			Character::operator=(other);
		}
		return *this;
	}

	virtual void TakeTurn() override;

private:

	vector<weak_ptr<Character>> _players;
	vector<weak_ptr<Character>> _enemies;

	int _level;
	int _count;

private:

	void Move();

	void CastSpell();

private:

	static int _n;
};