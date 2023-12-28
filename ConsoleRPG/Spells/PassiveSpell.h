#pragma once

#include "../RPGTypes.h"
#include "../Spells/Spell.h"
#include "../Characters/Character.h"

class PassiveSpell : public Spell {

public:
	PassiveSpell(ESpellID id, int lvl = 0);

	static unique_ptr<PassiveSpell> CreatePassiveSpell(ESpellID id);

	// Apply the passive spell effect
	virtual void Apply() = 0;

	// Get the event on which the passive triggers
	inline const ECombatEvent GetOnEvent() const { return _combat_event; }

public:
	weak_ptr<Character> _instigator;
	vector<weak_ptr<Character>> _targets;

protected:
	ECombatEvent _combat_event;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VampiricTouch : public PassiveSpell {
public:
	VampiricTouch() : PassiveSpell(ESpellID::VAMPIRIC_TOUCH) {}
	
	virtual void Apply() override;

	virtual stringstream& GetTooltip() override;

private:
	float _value = 0.8f;	
};

class Thorns : public PassiveSpell {
public:
	Thorns() : PassiveSpell(ESpellID::THORNS) {}

	virtual void Apply() override;

	//virtual stringstream& GetTooltip() override;

private:
	float _value = 10.f;
};