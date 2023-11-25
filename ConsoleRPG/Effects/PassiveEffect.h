#pragma once
#include "../RPGTypes.h"
#include "../Effects/Effect.h"
#include "../Characters/Character.h"

class PassiveEffect : public Effect {

public:
	PassiveEffect(EEffectID id, EEffectEvent on_event, EEffectType effect_type);

	static shared_ptr<PassiveEffect> CreatePassive(EEffectID id);

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
		PassiveMapConstructor() {
			_passive_map[EEffectID::VAMPIRIC_TOUCH] = make_pair(EEffectEvent::ON_CAST_END, EEffectType::PASSIVE);
		}
	} _passive_map_constructor;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VampiricTouch : public PassiveEffect {
public:
	VampiricTouch(EEffectID id, EEffectEvent on_event, EEffectType effect_type) : PassiveEffect(id, on_event, effect_type) {}
	
	virtual void Apply() override;

	virtual stringstream& GetTooltip() override;

private:
	float _value = 0.1f;	
};