#include "PassiveSpell.h"
#include "../GameplayStatics.h"

unordered_map<ESpellID, pair<ECombatEvent, EEffectType>> PassiveSpell::_spell_map;

PassiveSpell::SpellMapConstructor::SpellMapConstructor() {
	_spell_map[ESpellID::VAMPIRIC_TOUCH] = make_pair(ECombatEvent::ON_CAST_END, EEffectType::PASSIVE); // dodaj min Ilvl, dodaj ESpellRarity i passaj ESpellActivity = PASSIVE v base classu
};
PassiveSpell::SpellMapConstructor PassiveSpell::_spell_map_constructor;

PassiveSpell::PassiveSpell(ESpellID id)
	: Spell(id)
	, _on_event(_spell_map.at(id).first)
	, _effect_type(_spell_map.at(id).second)
	, _instigator(nullptr)
{}

shared_ptr<PassiveSpell> PassiveSpell::CreatePassiveSpell(ESpellID id) {

	switch (id) {
	case ESpellID::VAMPIRIC_TOUCH:
		return make_shared<VampiricTouch>(id);
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