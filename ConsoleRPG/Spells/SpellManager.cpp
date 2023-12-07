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

unique_ptr<ActiveSpell> SpellManager::CreateActiveSpell(Character* spell_owner, ESpellID id) {

	unique_ptr<ActiveSpell> spell = ActiveSpell::CreateActiveSpell(id);
	AddSpellToCharacter(spell_owner, move(spell));

	return spell;
}

shared_ptr<PassiveSpell> SpellManager::CreatePassiveSpell(Character* spell_owner, ESpellID id) {

	shared_ptr<PassiveSpell> spell = PassiveSpell::CreatePassiveSpell(id);
	AddSpellToCharacter(spell_owner, spell);

	return spell;
}

void SpellManager::AddSpellToCharacter(Character* spell_owner, unique_ptr<ActiveSpell> spell) {
	spell_owner->AddActiveSpell(spell);
}

void SpellManager::AddSpellToCharacter(Character* spell_owner, shared_ptr<PassiveSpell> spell) {
	spell_owner->AddPassiveSpell(spell);
}

void SpellManager::CastSpell(int spell_idx, Character* instigator, vector<weak_ptr<PlayerCharacter>> players, vector<weak_ptr<EnemyCharacter>> enemies, vector<int> p_idx, vector<int> e_idx) {

	bool bEnemy = false;
	if (instigator->GetAlias() >= 'A' && instigator->GetAlias() <= 'Z')
		bEnemy = true;

	vector<weak_ptr<Character>> team1;
	for (auto& p : players) {
		team1.push_back(p);
	}
	vector<weak_ptr<Character>> team2;
	for (auto& e : enemies) {
		team2.push_back(e);
	}

	CombatManager& cm = CombatManager::GetInstance();
	vector<weak_ptr<Character>> targets = { team2[e_idx[0]] };
	cm.OnCastBegin(instigator, team1, team2, targets);

	if (bEnemy) instigator->GetActiveSpells()[spell_idx]->InvokeEffect(instigator, team2, team1, p_idx, e_idx);
	else instigator->GetActiveSpells()[spell_idx]->InvokeEffect(instigator, team1, team2, p_idx, e_idx);

	for (int i = 0; i < e_idx.size() - 1; i++)
		targets.push_back(team2[e_idx[i + 1]]);

	cm.OnCastEnd(instigator, team1, team2, targets);
}


//void SpellManager::UnequipSpell(ESpellBookID spell_id) {
//	//dok se unequipa spell
//	// push spella na _instanced_spells
//}
//
//void SpellManager::EquipSpell(ESpellBookID spell_id) {
//
//}