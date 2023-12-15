#pragma once
#include "../RPGTypes.h"
#include "../Spells/Spell.h"
#include "../Characters/Character.h"

class PassiveSpell : public Spell {

public:
	PassiveSpell(ESpellID id, int lvl = 0);

	static unique_ptr<PassiveSpell> CreatePassiveSpell(ESpellID id);

	virtual void Apply() = 0;

	inline const ECombatEvent GetOnEvent() const { return _combat_event; }

public:

	Character* _instigator;
	vector<weak_ptr<Character>> _team1;
	vector<weak_ptr<Character>> _team2;
	vector<weak_ptr<Character>> _targets;

protected:
	const ECombatEvent _combat_event;
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VampiricTouch : public PassiveSpell {
public:
	VampiricTouch(ESpellID id) : PassiveSpell(id) {}
	
	virtual void Apply() override;

	virtual stringstream& GetTooltip() override;

private:
	float _value = 0.1f;	
};