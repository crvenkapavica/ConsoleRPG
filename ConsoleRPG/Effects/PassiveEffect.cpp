#include "PassiveEffect.h"
#include "../GameplayStatics.h"

unordered_map<EEffectID, pair<EEffectEvent, EEffectType>> PassiveEffect::_passive_map;

PassiveEffect::PassiveMapConstructor::PassiveMapConstructor() {
	_passive_map[EEffectID::VAMPIRIC_TOUCH] = make_pair(EEffectEvent::ON_CAST_END, EEffectType::PASSIVE);
};
PassiveEffect::PassiveMapConstructor PassiveEffect::_passive_map_constructor;

PassiveEffect::PassiveEffect(EEffectID id, EEffectEvent on_event, EEffectType effect_type)
	: Effect(id)
	, _on_event(on_event)
	, _effect_type(effect_type)
	, _instigator(nullptr)
{}

shared_ptr<PassiveEffect> PassiveEffect::CreatePassive(EEffectID id) {

	auto on_event = _passive_map.at(id).first;
	auto effect_type = _passive_map.at(id).second;

	switch (id) {
	case EEffectID::VAMPIRIC_TOUCH:
		return make_shared<VampiricTouch>(id, on_event, effect_type);
	}

	return nullptr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VampiricTouch::Apply() {
	for (auto& target : _targets) {
		float value = _value * target.lock()->GetHealth().GetMax();
		target.lock()->GetHealth().UpdateActual(-value, target.lock().get());
		target.lock()->AddEffectId(_ID);
		_instigator->GetHealth().UpdateActual(value, _instigator);
	}
}

stringstream& VampiricTouch::GetTooltip() {
	if (_tooltip.str().empty()) {
		_tooltip << CI << "Drains all afflicted targets for " << CV << GameplayStatics::float2(_value) << CI << " damage and transfers it to the caster.\n";
	}
	return _tooltip;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////