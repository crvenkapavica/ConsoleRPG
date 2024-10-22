#include "Spell.h"

Spell::Spell(const ESpellID id, const ESpellActivity activity, const ESpellRarity rarity, const ESpellClass spell_class, const int min_req_lvl, const int lvl)
	: _id(id)
	, _activity(activity)
	, _rarity(rarity)
	, _class(spell_class)
	, _minReqLvl(min_req_lvl)
	, _lvl(lvl)
{}