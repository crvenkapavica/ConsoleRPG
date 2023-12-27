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

public:
	SpellManager() {}
	SpellManager(const SpellManager&) = delete;
	SpellManager& operator=(const SpellManager&) = delete;

	static SpellManager& GetInstance();

	// Create an active spell and add it to the owner
	void CreateActiveSpell(Character* spell_owner, ESpellID id);
	// Create a passive spell and add it to the owner
	void CreatePassiveSpell(Character* spell_owner, ESpellID id);

	// Cast the spell with spell_idx which corresponds to the owners spell vector index
	void CastSpell(int spell_idx, shared_ptr<Character> instigator, vector<weak_ptr<Character>> targets);

	//void UnequipSpell(ESpellBookID spell_id);

	//void EquipSpell(ESpellBookID spell_id);
};
