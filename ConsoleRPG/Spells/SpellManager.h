#pragma once
#include "../RPGTypes.h"
#include "../GameplayStatics.h"
#include "../Spells/CSpellData.h"
#include "../Spells/SpellBook.h"

class Character;
class PlayerCharacter;
class EnemyCharacter;
class PassiveSpell;
struct EffectParams;

class SpellManager {

	SpellManager() {}
	SpellManager(const SpellManager&) = delete;
	SpellManager& operator=(const SpellManager&) = delete;

	unordered_map<ESpellBookID, SpellBook*> _instanced_spells;

public:

	static SpellManager& GetInstance();

	shared_ptr<SpellBook> CreateSpellBook(Character* spell_onwer, ESpellBookID spellbook_id, int level);

	shared_ptr<PassiveSpell> CreatePassiveSpell(Character* spell_owner, ESpellID id);



	void CastSpell(int spell_idx, Character* instigator, vector<weak_ptr<PlayerCharacter>> players, vector<weak_ptr<EnemyCharacter>> enemies, vector<int> p_idx, vector<int> e_idx, int effect_idx);

	void UnequipSpell(ESpellBookID spell_id);

	void EquipSpell(ESpellBookID spell_id);

private:

	void AddSpellToCharacter(Character* spell_owner, shared_ptr<SpellBook> spell);

	void AddPassiveToCharacter(Character* spell_owner, shared_ptr<PassiveSpell> passive);
};
