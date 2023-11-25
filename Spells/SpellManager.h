#pragma once
#include "../RPGTypes.h"
#include "../GameplayStatics.h"
#include "../Spells/CSpellData.h"
#include "../Spells/Spell.h"

class Character;
class PlayerCharacter;
class EnemyCharacter;
class PassiveEffect;
struct EffectParams;

class SpellManager {

	SpellManager() {}
	SpellManager(const SpellManager&) = delete;
	SpellManager& operator=(const SpellManager&) = delete;

	unordered_map<ESpellID, Spell*> _instanced_spells;

public:

	static SpellManager& GetInstance();

	shared_ptr<Spell> CreateSpell(Character* spell_onwer, ESpellID spell_id, int level);

	shared_ptr<PassiveEffect> CreatePassive(Character* spell_owner, EEffectID id);

	void CastSpell(int spell_idx, Character* instigator, vector<weak_ptr<PlayerCharacter>> players, vector<weak_ptr<EnemyCharacter>> enemies, vector<int> p_idx, vector<int> e_idx, int effect_idx);

	void UnequipSpell(ESpellID spell_id);

	void EquipSpell(ESpellID spell_id);

private:

	void AddSpellToCharacter(Character* spell_owner, shared_ptr<Spell> spell);

	void AddPassiveToCharacter(Character* spell_owner, shared_ptr<PassiveEffect> passive);
};
