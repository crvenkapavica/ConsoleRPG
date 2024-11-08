#pragma once

#include "Character.h"

class EnemyCharacter final : public Character {
	
public:
	explicit EnemyCharacter(const ECharacterClass Other);
	EnemyCharacter(const EnemyCharacter& Other);
	EnemyCharacter(EnemyCharacter&& Other) noexcept;
	~EnemyCharacter() override = default;

	inline EnemyCharacter& operator=(const EnemyCharacter& Other) {
		if (this != &Other) {
			Character::operator=(Other);
			Players = Other.Players;
			Enemies = Other.Enemies;
		}
		return *this;
	}
	inline EnemyCharacter& operator=(EnemyCharacter&& Other) noexcept {
		if (this != &Other) {
			Players = std::move(Other.Players);
			Enemies = std::move(Other.Enemies);
			Character::operator =(std::move(Other));
		}
		return *this;
	}

	static int nEnemyCharacters;

	virtual void TakeTurn() override;

	//friend class SummonCharacter;

private:
	std::vector<std::weak_ptr<Character>> Players;
	std::vector<std::weak_ptr<Character>> Enemies;

private:
	void Move() const;
	void CastSpell();
};