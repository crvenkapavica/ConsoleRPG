#include "PassiveSpell.h"
#include "../GameplayStatics.h"
#include "../Spells/SpellData.h"

PassiveSpell::PassiveSpell(ESpellID id, int lvl)
	: Spell(id, ESpellActivity::PASSIVE, SpellDb::PassiveConstMap.at(id).Rarity, SpellDb::PassiveConstMap.at(id).Class, SpellDb::PassiveConstMap.at(id).MinReqLvl, lvl)
	, _combat_event(SpellDb::PassiveConstMap.at(id).CombatEvent)
	, _instigator(weak_ptr<Character>())
{}

unique_ptr<PassiveSpell> PassiveSpell::CreatePassiveSpell(ESpellID id) {

	switch (id) {
	case ESpellID::VAMPIRIC_TOUCH:
		return make_unique<VampiricTouch>();
	case ESpellID::THORNS:
		return make_unique<Thorns>();
	}

	return nullptr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VampiricTouch::Apply() {
	for (auto& target : _targets) {
		if (!target.expired()) {
			float value = _value * target.lock()->GetHealth().GetMax();
			target.lock()->GetHealth().GetActual() -= value;
			target.lock()->AddEffectId(_id);
			_instigator.lock().get()->GetHealth().GetActual() += value;
		}
	}
}

stringstream& VampiricTouch::GetTooltip() {
	if (_tooltip.str().empty()) {
		_tooltip << CI << "Drains all afflicted targets for " << CV << GameplayStatics::float2(_value) << CI << " damage and transfers it to the caster.\n";
	}
	return _tooltip;
}

void Thorns::Apply() {
	_instigator.lock()->GetHealth().GetActual() -= _value;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////