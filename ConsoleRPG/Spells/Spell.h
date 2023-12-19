#pragma once

#include "../RPGTypes.h"

class Spell {
public:
    Spell(ESpellID id, ESpellActivity activity, ESpellRarity rarity, ESpellClass spell_class, int min_req_lvl, int lvl);

    inline const ESpellID GetID() const { return _ID; }
    inline const ESpellActivity GetActivity() const { return _activity; }
    inline const ESpellRarity GetRarity() const { return _rarity; }
    inline const ESpellClass GetClass() const { return _class; }
    // Get minimum required level to use the spell
    inline const int GetMinReqLvl() const { return _min_req_lvl; }

    // Get Spell level
    inline const int GetLvl() const { return _lvl; }
    // Get Spell Experience
    inline const int GetExperience() const { return _experience; }

    inline void SetLvl(int lvl) { _lvl = lvl; }

    // Get all information about the spell; values and functionality
    virtual std::stringstream& GetTooltip() { return _tooltip; }

protected:
    const ESpellID _ID;
    const ESpellActivity _activity;
    const ESpellRarity _rarity;
    const ESpellClass _class;
    int _min_req_lvl;

    int _lvl = 0;
    int _experience = 0;

    std::stringstream _tooltip;
};