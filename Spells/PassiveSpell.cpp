#include "PassiveSpell.h"
#include "../GameplayStatics.h"
#include "../Spells/SpellData.h"

PassiveSpell::PassiveSpell(const ESpellID Id, const int InLevel)
	: Spell(Id, ESpellActivity::PASSIVE, SpellDb::PassiveConstMap.at(Id).Rarity, SpellDb::PassiveConstMap.at(Id).Class, SpellDb::PassiveConstMap.at(Id).RequiredLevel, InLevel)
	, Instigator(std::weak_ptr<Character>())
	, CombatEvent(SpellDb::PassiveConstMap.at(Id).CombatEvent)
{}

std::unique_ptr<PassiveSpell> PassiveSpell::CreatePassiveSpell(const ESpellID SpellID) {
	switch (SpellID) {
	case ESpellID::VAMPIRIC_TOUCH:
		return std::make_unique<VampiricTouch>();
	case ESpellID::THORNS:
		return std::make_unique<Thorns>();
	}

	return nullptr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VampiricTouch::Apply() {
	for (auto& Target : Targets)
		if (!Target.expired()) {
			const float Value = Modifier * Target.lock()->GetHealth().GetMax();
			Target.lock()->GetHealth().GetActual() -= Value;
			Target.lock()->AddEffectId(ID);
			Instigator.lock()->GetHealth().GetActual() += Value;
		}
}

std::stringstream& VampiricTouch::GetTooltip() {
	if (Tooltip.str().empty())
		Tooltip << CI << "Drains all afflicted targets for " << CV << GameplayStatics::Float2(Modifier) << CI << " damage and transfers it to the caster.\n";
	return Tooltip;
}

void Thorns::Apply() {
	Instigator.lock()->GetHealth().GetActual() -= Modifier;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////