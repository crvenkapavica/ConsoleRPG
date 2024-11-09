#pragma once

#include "../RPGTypes.h"

class Spell {
public:
    Spell() = delete;
    Spell(const ESpellID ID, const ESpellActivity Activity, const ESpellRarity Rarity, const ESpellClass SpellClass, const int RequiredLevel, const int Level);
    Spell(const Spell& Other) = delete;
    Spell(Spell&& Other) noexcept = default;
    virtual ~Spell() = default;
    
    Spell& operator=(const Spell& Other) = delete;
    Spell& operator=(Spell&& Other) noexcept {
        if (this != &Other) {
            ID = Other.ID;
            Activity = Other.Activity;
            Rarity = Other.Rarity;
            Class = Other.Class;
            RequiredLevel = Other.RequiredLevel;
            Level = Other.Level;
            Experience = Other.Experience;
            Tooltip = std::move(Other.Tooltip);
        }
        return *this;
    }
    
    inline ESpellID GetID() const { return ID; }
    inline ESpellActivity GetActivity() const { return Activity; }
    inline ESpellRarity GetRarity() const { return Rarity; }
    inline ESpellClass GetClass() const { return Class; }
    // Get minimum required level to use the spell
    inline int GetRequiredLevel() const { return RequiredLevel; }
    // Get Spell level
    inline int GetLevel() const { return Level; }
    // Set new spell level
    inline void SetLevel(const int NewLevel) { Level = NewLevel; }
    // Get Spell Experience
    inline int GetExperience() const { return Experience; }
    // Get all information about the spell; values and functionality
    virtual std::stringstream& GetTooltip() { return Tooltip; }

protected:
    // TODO : const?
    ESpellID ID;
    ESpellActivity Activity;
    ESpellRarity Rarity;
    ESpellClass Class;
    int RequiredLevel;
    //////////////////////////////
    
    int Level = 0;
    int Experience = 0;
    std::stringstream Tooltip;
};