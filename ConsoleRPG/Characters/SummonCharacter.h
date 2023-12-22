#pragma once

#include "Character.h"

class SummonCharacter : public Character {
public:
	SummonCharacter(ECharacterClass character_class, int team);

	SummonCharacter(const SummonCharacter& other);
	
	SummonCharacter(SummonCharacter&& other) noexcept;

	//SummonCharacter& operator*() {
	//	_team == 1 ? --_p_n : --_e_n;
	//	return *this;
	//}

	~SummonCharacter();

	virtual void TakeTurn() override;


	static int _p_n;
	static int _e_n;

private:

};