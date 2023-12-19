#pragma once

#include "Character.h"

class SummonCharacter : public Character {
public:
	SummonCharacter(const CharacterData& data, int team, vector<weak_ptr<Character>>& v);

	~SummonCharacter();

	virtual void TakeTurn() override;

	inline const ESummonClass GetClass() const { return _class; }
	inline const ESummonType GetType() const { return _summon_type; }
	inline const EDamageType GetDamageType() const { return _dmg_type; }
	inline const ESpellID GetSpell() const { return _spell; }

public:

	const ESummonClass	_class;
	const ESummonType	_summon_type;
	const EDamageType	_dmg_type;
	const ESpellID		_spell;
};