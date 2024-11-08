#pragma once

#include "Character.h"

class SummonCharacter final : public Character {
public:
	SummonCharacter(ECharacterClass InCharacterClass, int InTeam);
	SummonCharacter(const SummonCharacter& Other) = default;
	SummonCharacter(SummonCharacter&& Other) noexcept = default;
	~SummonCharacter() override = default;
	
	SummonCharacter& operator=(const SummonCharacter& Other) {
		if (this != &Other)
			Character::operator =(Other);
		return *this;
	}
	SummonCharacter& operator=(SummonCharacter&& Other) noexcept {
		if (this != &Other)
			Character::operator =(std::move(Other));
		return *this;
	}
	
	virtual void TakeTurn() override;
	
	static int nPlayerSummons;
	static int nEnemySummons;
};