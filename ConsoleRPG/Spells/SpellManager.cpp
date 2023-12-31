#include "../Spells/SpellManager.h"
#include "../Characters/Character.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Spells/EffectStructs.h"
#include "../Combat/CombatManager.h"
#include "../Spells/PassiveSpell.h"

SpellManager& SpellManager::GetInstance() {
	static SpellManager _instance;
	return _instance;
}

void SpellManager::CreateActiveSpell(Character* spell_owner, ESpellID id) {

	if (id == ESpellID::NONE) return;

	unique_ptr<ActiveSpell> spell = ActiveSpell::CreateActiveSpell(id);
	spell_owner->AddActiveSpell(spell);
}

void SpellManager::CreatePassiveSpell(Character* spell_owner, ESpellID id) {

	if (id == ESpellID::NONE) return;

	unique_ptr<PassiveSpell> spell = PassiveSpell::CreatePassiveSpell(id);
	spell_owner->AddPassiveSpell(spell);
}

void SpellManager::CastSpell(int spell_idx, shared_ptr<Character> instigator, vector<weak_ptr<Character>> targets) {

	ActiveSpell* spell = instigator->GetActiveSpells()[spell_idx].get();
	CombatManager& cm = CombatManager::GetInstance();

	if (spell->GetClass() == ESpellClass::MAGIC)
		cm.OnMagicBegin(instigator, targets);
	else if (spell->GetClass() == ESpellClass::MELEE)
		cm.OnMeleeBegin(instigator, targets);
	else if (spell->GetClass() == ESpellClass::RANGED)
		cm.OnRangedBegin(instigator, targets);

	spell->Apply(instigator, targets);

	if (spell->GetClass() == ESpellClass::MAGIC)
		cm.OnMagicEnd(instigator, targets);
	else if (spell->GetClass() == ESpellClass::MELEE)
		cm.OnMeleeEnd(instigator, targets);
	else if (spell->GetClass() == ESpellClass::RANGED)
		cm.OnRangedEnd(instigator, targets);

	cm.FlagDeadCharacters();
}