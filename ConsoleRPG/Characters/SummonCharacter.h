#pragma once

#include "Character.h"

class SummonCharacter : public Character {
public:
	SummonCharacter(ECharacterClass character_class, int team, vector<weak_ptr<Character>>& v);

	~SummonCharacter();

	virtual void TakeTurn() override;
};