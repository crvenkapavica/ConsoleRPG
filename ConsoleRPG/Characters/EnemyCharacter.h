#pragma once

#include "Character.h"

class SummonCharacter;

class EnemyCharacter : public Character { 

public:
	EnemyCharacter(ECharacterClass character_class);

	EnemyCharacter(const EnemyCharacter& other);

	EnemyCharacter(EnemyCharacter&& enemy) noexcept;

	~EnemyCharacter();

	EnemyCharacter& operator=(const EnemyCharacter& other) {
		if (this != &other) {
			Character::operator=(other);
		}
		return *this;
	}

	static int _n;

	virtual void TakeTurn() override;

	friend class SummonCharacter;

private:

	vector<weak_ptr<Character>> _players;
	vector<weak_ptr<Character>> _enemies;

	int _level;
	int _count;

private:

	void Move();

	void CastSpell();
};