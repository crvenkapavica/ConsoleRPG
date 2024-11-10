#include "../Spells/SpellManager.h"
#include "../Characters/Character.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Spells/EffectStructs.h"
#include "../Combat/CombatManager.h"
#include "../Spells/PassiveSpell.h"

void SpellManager::CreateActiveSpell(Character* SpellOwner, const ESpellID Id) {
	if (Id == ESpellID::NONE) return;

	std::unique_ptr<ActiveSpell> spell = ActiveSpell::CreateActiveSpell(Id);
	SpellOwner->AddActiveSpell(std::move(spell));
}

void SpellManager::CreatePassiveSpell(Character* SpellOwner, const ESpellID Id) {
	if (Id == ESpellID::NONE) return;

	std::unique_ptr<PassiveSpell> spell = PassiveSpell::CreatePassiveSpell(Id);
	SpellOwner->AddPassiveSpell(std::move(spell));
}

void SpellManager::CastSpell(const int SpellIndex, const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	ActiveSpell* Spell = Instigator->GetActiveSpells()[SpellIndex].get();
	using cm = CombatManager;

	if (Spell->GetClass() == ESpellClass::MAGIC)
		cm::OnMagicBegin(Instigator, Targets);
	else if (Spell->GetClass() == ESpellClass::MELEE)
		cm::OnMeleeBegin(Instigator, Targets);
	else if (Spell->GetClass() == ESpellClass::RANGED)
		cm::OnRangedBegin(Instigator, Targets);

	Spell->Apply(Instigator, Targets);

	if (Spell->GetClass() == ESpellClass::MAGIC)
		cm::OnMagicEnd(Instigator, Targets);
	else if (Spell->GetClass() == ESpellClass::MELEE)
		cm::OnMeleeEnd(Instigator, Targets);
	else if (Spell->GetClass() == ESpellClass::RANGED)
		cm::OnRangedEnd(Instigator, Targets);

	cm::FlagDeadCharacters();
}