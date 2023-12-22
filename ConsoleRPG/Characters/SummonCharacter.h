#pragma once

#include "Character.h"

class SummonCharacter : public Character {
public:
	SummonCharacter(ECharacterClass character_class, int team);

	SummonCharacter(const SummonCharacter& other);
	
	SummonCharacter(SummonCharacter&& other) noexcept;

	~SummonCharacter();

	virtual void TakeTurn() override;


	static int _p_n;
	static int _e_n;

private:

};