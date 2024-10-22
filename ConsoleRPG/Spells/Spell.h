#pragma once

#include "../RPGTypes.h"

class Spell {
public:
    Spell(const ESpellID id, const ESpellActivity activity, const ESpellRarity rarity, const ESpellClass spell_class, const int min_req_lvl, const int lvl);

    virtual ~Spell() = default;

    inline ESpellID GetId() const { return _id; }
    inline ESpellActivity GetActivity() const { return _activity; }
    inline ESpellRarity GetRarity() const { return _rarity; }
    inline ESpellClass GetClass() const { return _class; }
    // Get minimum required level to use the spell
    inline int GetMinReqLvl() const { return _minReqLvl; }
    // Get Spell level
    inline int GetLvl() const { return _lvl; }
    // Get Spell Experience
    inline int GetExperience() const { return _experience; }

    inline void SetLvl(const int lvl) { _lvl = lvl; }

    // Get all information about the spell; values and functionality
    virtual std::stringstream& GetTooltip() { return _tooltip; }

        

protected:
    const ESpellID _id;
    const ESpellActivity _activity;
    const ESpellRarity _rarity;
    const ESpellClass _class;
    int _minReqLvl;

    int _lvl = 0;
    int _experience = 0;
    
    std::stringstream _tooltip;
};