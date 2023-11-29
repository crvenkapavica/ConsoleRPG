#include "../Spells/SpellManager.h"
#include "../Characters/Character.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Effects/EffectStructs.h"
#include "../Combat/CombatManager.h"
#include "../Effects/PassiveEffect.h"

SpellManager& SpellManager::GetInstance() {
	static SpellManager _instance;
	return _instance;
}

shared_ptr<Spell> SpellManager::CreateSpell(Character* spell_owner, ESpellID spell_id, int level) {

	shared_ptr<Spell> spell = Spell::CreateSpell(spell_id, level);
	AddSpellToCharacter(spell_owner, spell);

	return spell;
}

shared_ptr<PassiveEffect> SpellManager::CreatePassive(Character* spell_owner, EEffectID id) {

	shared_ptr<PassiveEffect> passive = PassiveEffect::CreatePassive(id);
	AddPassiveToCharacter(spell_owner, passive);

	return passive;
}

void SpellManager::AddSpellToCharacter(Character* spell_owner, shared_ptr<Spell> spell) {
	spell_owner->AddSpell(spell);
}

void SpellManager::AddPassiveToCharacter(Character* spell_owner, shared_ptr<PassiveEffect> passive) {
	spell_owner->AddPassive(passive);
}

void SpellManager::CastSpell(int spell_idx, Character* instigator, vector<weak_ptr<PlayerCharacter>> players, vector<weak_ptr<EnemyCharacter>> enemies, vector<int> p_idx, vector<int> e_idx, int effect_idx) {

	// provjeri dal je dosta resursi
	// ako nesi dosta veliki level za efekt, wrong input i ispocetka print menu

	//ono kaj je instantno - kao instant Character->TakeDamage, bu se instantno desilo,
	// za ostale effekte, prije ili nakon castanja, stvara se CombatEffect unique_ptr na strukturu, i dodaju se parametri passani u CastSpell
	// tak moremo saki turn applajati te efekte i dok turnovi isteknu, ti efekti se brisu


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
	vector<weak_ptr<Character>> targets = { team2[e_idx[0]] }; // promeniti u weak_ptr ili mozda shared (ali weak)
	cm.OnCastBegin(instigator, team1, team2, targets);

	if (bEnemy) instigator->GetSpells()[spell_idx]->InvokeEffect(instigator, team2, team1, p_idx, e_idx, effect_idx);
	else instigator->GetSpells()[spell_idx]->InvokeEffect(instigator, team1, team2, p_idx, e_idx, effect_idx);

	for (int i = 0; i < e_idx.size() - 1; i++)
		targets.push_back(team2[e_idx[i + 1]]);

	cm.OnCastEnd(instigator, team1, team2, targets);
}


void SpellManager::UnequipSpell(ESpellID spell_id) {
	//dok se unequipa spell
	// push spella na _instanced_spells
}

void SpellManager::EquipSpell(ESpellID spell_id) {

}