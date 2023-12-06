#pragma once
#include "../RPGTypes.h"

class Spell {
public:

    Spell(EEffectID id);

    inline const EEffectID GetID() const { return _ID; }

    virtual std::stringstream& GetTooltip() { return _tooltip; }

protected:

    const EEffectID _ID;

    std::stringstream _tooltip;
};