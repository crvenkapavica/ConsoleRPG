#pragma once

#include "Character.h"

class SummonCharacter;

class EnemyCharacter : public Character { 

public:
	explicit EnemyCharacter(ECharacterClass InCharacterClass);
	EnemyCharacter(EnemyCharacter&& Other) noexcept;
	~EnemyCharacter() override = default;

	EnemyCharacter& operator=(const EnemyCharacter& Other) {
		if (this != &Other) {
			Character::operator=(Other);
		}
		return *this;
	}

	static int nEnemyCharacters;

	virtual void TakeTurn() override;

	friend class SummonCharacter;

private:

	std::vector<std::weak_ptr<Character>> Players;
	std::vector<std::weak_ptr<Character>> Enemies;

private:

	void Move();

	void CastSpell();
};