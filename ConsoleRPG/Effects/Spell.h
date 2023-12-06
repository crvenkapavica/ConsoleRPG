#pragma once
#include "../RPGTypes.h"

class Spell {
public:

    Spell(ESpellID id);

    inline const ESpellID GetID() const { return _ID; }

    virtual std::stringstream& GetTooltip() { return _tooltip; }

protected:

    const ESpellID _ID;

    std::stringstream _tooltip;
};