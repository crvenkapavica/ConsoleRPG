#include "Spell.h"

Spell::Spell(ESpellID id, ESpellActivity activity, ESpellRarity rarity, int min_req_lvl, int lvl)
	: _ID(id)
	, _activity(activity)
	, _rarity(rarity)
	, _min_req_lvl(min_req_lvl)
	, _lvl(lvl)
{}