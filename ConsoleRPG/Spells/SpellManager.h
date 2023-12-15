#pragma once
#include "../RPGTypes.h"
#include "../GameplayStatics.h"
#include "../Spells/SpellData.h"

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

	void CreateActiveSpell(Character* spell_owner, ESpellID id);
	void CreatePassiveSpell(Character* spell_owner, ESpellID id);

	void CastSpell(int spell_idx, Character* instigator, vector<weak_ptr<Character>> targets);

	//void UnequipSpell(ESpellBookID spell_id);

	//void EquipSpell(ESpellBookID spell_id);
};
