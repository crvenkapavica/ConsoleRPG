#pragma once

#include "../GameplayStatics.h"
#include "../RPGTypes.h"
#include "../Spells/SpellData.h"

class Character;
class PlayerCharacter;
class EnemyCharacter;
class PassiveSpell;
struct EffectParams;

class SpellManager final {

public:
	SpellManager() = default;
	SpellManager(const SpellManager&) = delete;
	SpellManager(SpellManager&&) = delete;
	~SpellManager() = default;
	
	SpellManager& operator=(const SpellManager&) = default;
	SpellManager& operator=(SpellManager&&) = delete;
	
	static SpellManager& GetInstance();

	// Create an active spell and add it to the owner
	static void CreateActiveSpell(Character* SpellOwner, const ESpellID Id);
	// Create a passive spell and add it to the owner
	static void CreatePassiveSpell(Character* SpellOwner, const ESpellID Id);

	// Cast the spell with SpellIdx which corresponds to the owners spell vector index
	static void CastSpell(const int SpellIndex, const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets);

	//void UnequipSpell(ESpellBookID spell_id);

	//void EquipSpell(ESpellBookID spell_id);
};
