#pragma once

#include "../RPGTypes.h"
#include "../Spells/Spell.h"
#include "../Characters/Character.h"

class PlayerCharacter;
class EnemyCharacter;
struct EffectParams;

class ActiveSpell : public Spell {
public:
    ActiveSpell(ESpellID id, int lvl = 0);

    static unique_ptr<ActiveSpell> CreateActiveSpell(ESpellID id);

    // Apply the effect of the spell
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) = 0;

    inline const EDamageType GetDamageType() const { return _damage_type; }
    inline const ESpellType GetSpellType() const { return _spell_type; }

protected:
    // Get random damage between spell min and max value at current level
    float GetRandEffectMinMax(const shared_ptr<Character>& character);

    // Get random damage between spell min and max value for on aplly effect at current level
    float GetRandOnApplyMinMax(const shared_ptr<Character>& character);

    // Apply resistances, armor reduction, critial chance and other procs and return final damage
    float AdjustDamage(float damage, const shared_ptr<Character>& character);

    // Add random targets to the target vector (when multi-striking, or when spell targets more than one character)
    int AddRandomTargets(int r, vector<weak_ptr<Character>>& targets, const shared_ptr<Character>& character, const string& name);

    bool Summon(ECharacterClass character_class, const shared_ptr<Character>& instigator);

protected:
    const EDamageType _damage_type;
    const ESpellType _spell_type;
};

//============================================================================== MAGIC =============================================================================================
//==================================================================================================================================================================================
class Fireball : public ActiveSpell {
public:
    Fireball() : ActiveSpell(ESpellID::FIREBALL) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
    virtual stringstream& GetTooltip() override;
};

class Burning : public ActiveSpell {
public:
    Burning() : ActiveSpell(ESpellID::BURNING) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
    virtual stringstream& GetTooltip() override;
};

class MoltenArmor : public ActiveSpell {
public:
    MoltenArmor() : ActiveSpell(ESpellID::MOLTEN_ARMOR) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
    virtual stringstream& GetTooltip() override;
};

class Exposure : public ActiveSpell {
public:
    Exposure() : ActiveSpell(ESpellID::EXPOSURE) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
    virtual stringstream& GetTooltip() override;
};

class Stoneskin : public ActiveSpell {
public:
    Stoneskin() : ActiveSpell(ESpellID::STONESKIN) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
    virtual stringstream& GetTooltip() override;
};

class Disarm : public ActiveSpell {
public:
    Disarm() : ActiveSpell(ESpellID::DISARM) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};

class Bloodbath : public ActiveSpell {
public:
    Bloodbath() : ActiveSpell(ESpellID::BLOODBATH) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};

class ArcaneInfusion : public ActiveSpell {
public:
    ArcaneInfusion() : ActiveSpell(ESpellID::ARCANE_INFUSION) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};

class BloodRain : public ActiveSpell {
public:
    BloodRain() : ActiveSpell(ESpellID::BLOOD_RAIN) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};

class ViscousAcid : public ActiveSpell {
public:
    ViscousAcid() : ActiveSpell(ESpellID::VISCOUS_ACID) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
    virtual stringstream& GetTooltip() override;
};






//==============================================================================  MISC  ============================================================================================
//==================================================================================================================================================================================

class Blind : public ActiveSpell {
public:
    Blind() : ActiveSpell(ESpellID::BLIND) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};


//=============================================================================  SUMMON  ===========================================================================================
//==================================================================================================================================================================================

class SummonFireElemental : public ActiveSpell {
public:
    SummonFireElemental() : ActiveSpell(ESpellID::SUM_FIRE_ELE) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};

class SummonFireImp : public ActiveSpell {
public:
    SummonFireImp() : ActiveSpell(ESpellID::SUM_FIRE_IMP) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};




//============================================================================== MELEE =============================================================================================
//==================================================================================================================================================================================

class Melee : public ActiveSpell {
public:
    Melee() : ActiveSpell(ESpellID::MELEE) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};





//============================================================================== RANGED =============================================================================================
//===================================================================================================================================================================================

class Ranged : public ActiveSpell {
public:
    Ranged() : ActiveSpell(ESpellID::RANGED) {}
    virtual void Apply(shared_ptr<Character> instigator, vector<weak_ptr<Character>>& targets) override;
};