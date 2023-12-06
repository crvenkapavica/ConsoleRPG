#pragma once
#include "../RPGTypes.h"
#include "../Effects/Spell.h"
#include "../Characters/Character.h"

class PassiveSpell : public Spell {

public:
	PassiveSpell(ESpellID id);

	static shared_ptr<PassiveSpell> CreatePassiveSpell(ESpellID id);

	virtual void Apply() = 0;

	inline const ECombatEvent GetOnEvent() const { return _on_event; }
	inline const EEffectType GetEffectType() const { return _effect_type; }

public:

	Character* _instigator;
	vector<weak_ptr<Character>> _team1;
	vector<weak_ptr<Character>> _team2;
	vector<weak_ptr<Character>> _targets;

protected:
	const ECombatEvent _on_event;
	const EEffectType _effect_type;

private:
	static unordered_map<ESpellID, pair<ECombatEvent, EEffectType>> _spell_map;
	static struct SpellMapConstructor {
		SpellMapConstructor();
	} _spell_map_constructor;
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