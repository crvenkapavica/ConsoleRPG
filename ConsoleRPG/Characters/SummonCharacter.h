#pragma once

#include "Character.h"

class SummonCharacter : public Character {
public:
	SummonCharacter(const CharacterData& data, int team, vector<weak_ptr<Character>>& v);

	~SummonCharacter();

	virtual void TakeTurn() override;

	inline const ECharacterClass GetClass() const { return _class; }
	inline const EDamageType GetDamageType() const { return _dmg_type; }
	inline const ESpellID GetSpell() const { return _spell; }

public:

	const ECharacterClass	_class;
	const EDamageType	_dmg_type;
	const ESpellID		_spell;
};