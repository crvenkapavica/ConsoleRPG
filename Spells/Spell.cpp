#include "Spell.h"

Spell::Spell(const ESpellID ID, const ESpellActivity Activity, const ESpellRarity Rarity, const ESpellClass SpellClass, const int RequiredLevel, const int Level)
	: ID(ID)
	, Activity(Activity)
	, Rarity(Rarity)
	, Class(SpellClass)
	, RequiredLevel(RequiredLevel)
	, Level(Level)
{}