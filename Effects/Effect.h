#pragma once
#include "../RPGTypes.h"

class Effect {
public:

    Effect(EEffectID id);

    inline const EEffectID GetID() const { return _ID; }

    virtual std::stringstream& GetTooltip() { return _tooltip; }

protected:

    const EEffectID _ID;

    std::stringstream _tooltip;
};