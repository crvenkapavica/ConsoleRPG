#pragma once

#include "../RPGTypes.h"
#include "../Characters/Character.h"
#include "../Spells/Spell.h"

class PassiveSpell : public Spell {

public:
	explicit PassiveSpell(const ESpellID Id, const int InLevel = 0);

	static std::unique_ptr<PassiveSpell> CreatePassiveSpell(ESpellID SpellID);

	// Apply the passive spell effect
	virtual void Apply() = 0;

	// Get the event on which the passive triggers
	inline ECombatEvent GetOnEvent() const { return CombatEvent; }

public:
	std::weak_ptr<Character> Instigator;
	std::vector<std::weak_ptr<Character>> Targets;

protected:
	ECombatEvent CombatEvent;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class VampiricTouch final : public PassiveSpell {
public:
	VampiricTouch() : PassiveSpell(ESpellID::VAMPIRIC_TOUCH) {}
	
	virtual void Apply() override;
	virtual std::stringstream& GetTooltip() override;

private:
	float Modifier = 1.f;	
};

class Thorns final : public PassiveSpell {
public:
	Thorns() : PassiveSpell(ESpellID::THORNS) {}

	virtual void Apply() override;
	//virtual stringstream& GetTooltip() override;

private:
	float Modifier = 10.f;
};