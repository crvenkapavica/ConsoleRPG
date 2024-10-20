#include "Spell.h"

Spell::Spell(ESpellID id, ESpellActivity activity, ESpellRarity rarity, ESpellClass spell_class, int min_req_lvl, int lvl)
	: _id(id)
	, _activity(activity)
	, _rarity(rarity)
	, _class(spell_class)
	, _minReqLvl(min_req_lvl)
	, _lvl(lvl)
{}