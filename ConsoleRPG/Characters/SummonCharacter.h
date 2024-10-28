#pragma once

#include "Character.h"

class SummonCharacter : public Character {
public:
	SummonCharacter(ECharacterClass InCharacterClass, int InTeam);
	SummonCharacter(const SummonCharacter& Other);
	SummonCharacter(SummonCharacter&& Other) noexcept;
	~SummonCharacter() override = default;

	virtual void TakeTurn() override;
	
	static int nPlayerSummons;
	static int nEnemySummons;
};