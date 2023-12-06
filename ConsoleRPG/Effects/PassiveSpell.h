#pragma once
#include "../RPGTypes.h"
#include "../Effects/Spell.h"
#include "../Characters/Character.h"

class PassiveSpell : public Spell {

public:
	PassiveSpell(EEffectID id, EEffectEvent on_event, EEffectType effect_type);

	static shared_ptr<PassiveSpell> CreatePassive(EEffectID id);

	virtual void Apply() = 0;

	inline const EEffectEvent GetOnEvent() const { return _on_event; }
	inline const EEffectType GetEffectType() const { return _effect_type; }

	static unordered_map<EEffectID, pair<EEffectEvent, EEffectType>> _passive_map;

public:

	Character* _instigator;
	vector<weak_ptr<Character>> _team1;
	vector<weak_ptr<Character>> _team2;
	vector<weak_ptr<Character>> _targets;

protected:
	const EEffectEvent _on_event;
	const EEffectType _effect_type;

private:
	static struct PassiveMapConstructor {
		PassiveMapConstructor();
	} _passive_map_constructor;
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VampiricTouch : public PassiveSpell {
public:
	VampiricTouch(EEffectID id, EEffectEvent on_event, EEffectType effect_type) : PassiveSpell(id, on_event, effect_type) {}
	
	virtual void Apply() override;

	virtual stringstream& GetTooltip() override;

private:
	float _value = 0.1f;	
};