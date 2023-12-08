#include "PassiveSpell.h"
#include "../GameplayStatics.h"
#include "../Spells/SpellData.h"

PassiveSpell::PassiveSpell(ESpellID id, int lvl)
	: Spell(id, ESpellActivity::PASSIVE, SpellDB::_active_const_map.at(id)._rarity, SpellDB::_active_const_map.at(id)._min_req_lvl, lvl)
	, _combat_event(SpellDB::_passive_const_map.at(id)._combat_event)
	, _passive_type(SpellDB::_passive_const_map.at(id)._passive_type)
	, _instigator(nullptr)
{}

unique_ptr<PassiveSpell> PassiveSpell::CreatePassiveSpell(ESpellID id) {

	switch (id) {
	case ESpellID::VAMPIRIC_TOUCH:
		return make_unique<VampiricTouch>(id);
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