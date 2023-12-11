#include "../Spells/SpellManager.h"
#include "../Characters/Character.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Effects/EffectStructs.h"
#include "../Combat/CombatManager.h"
#include "../Effects/PassiveSpell.h"

SpellManager& SpellManager::GetInstance() {
	static SpellManager _instance;
	return _instance;
}

//shared_ptr<SpellBook> SpellManager::CreateSpellBook(Character* spell_owner, ESpellBookID spellbook_id, int level) {
//
//	shared_ptr<SpellBook> spell = SpellBook::CreateSpellBook(spellbook_id, level);
//	AddSpellToCharacter(spell_owner, spell);
//
//	return spell;
//}

void SpellManager::CreateActiveSpell(Character* spell_owner, ESpellID id) {

	unique_ptr<ActiveSpell> spell = ActiveSpell::CreateActiveSpell(id);
	spell_owner->AddActiveSpell(spell);
}

void SpellManager::CreatePassiveSpell(Character* spell_owner, ESpellID id) {

	unique_ptr<PassiveSpell> spell = PassiveSpell::CreatePassiveSpell(id);
	spell_owner->AddPassiveSpell(spell);
}

void SpellManager::CastSpell(int spell_idx, Character* instigator, vector<weak_ptr<Character>> targets) {

	CombatManager& cm = CombatManager::GetInstance();

	cm.OnCastBegin(instigator, targets);
	instigator->GetActiveSpells()[spell_idx]->Apply(instigator, targets);
	cm.OnCastEnd(instigator, targets);
}


//void SpellManager::UnequipSpell(ESpellBookID spell_id) {
//	//dok se unequipa spell
//	// push spella na _instanced_spells
//}
//
//void SpellManager::EquipSpell(ESpellBookID spell_id) {
//
//}