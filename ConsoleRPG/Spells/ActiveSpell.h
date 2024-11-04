#pragma once

#include "../RPGTypes.h"
#include "../Characters/Character.h"
#include "../Spells/Spell.h"

class PlayerCharacter;
class EnemyCharacter;
struct EffectParams;

class ActiveSpell : public Spell {
public:
    ActiveSpell(ESpellID id, int lvl = 0);

    static std::unique_ptr<ActiveSpell> CreateActiveSpell(ESpellID id);

    // Apply the effect of the spell
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) = 0;

    inline EDamageType GetDamageType() const { return _damage_type; }
    inline ESpellType GetSpellType() const { return _spell_type; }

protected:
    // Get random damage between spell min and max value at current level
    float GetRandEffectMinMax(const std::shared_ptr<Character>& character);

    // Get random damage between spell min and max value for on aplly effect at current level
    float GetRandOnApplyMinMax(const std::shared_ptr<Character>& character);

    // Apply resistances, armor reduction, critial chance and other procs and return final damage
    float AdjustDamage(float Damage, const std::shared_ptr<Character>& character);

    // Add random targets to the target vector (when multi-striking, or when spell targets more than one character)
    int AddRandomTargets(int R, std::vector<std::weak_ptr<Character>>& Targets, const std::shared_ptr<Character>& character, const std::string& name);

    bool Summon(ECharacterClass character_class, const std::shared_ptr<Character>& instigator);

protected:
    const EDamageType _damage_type;
    const ESpellType _spell_type;
};

//============================================================================== MAGIC =============================================================================================
//==================================================================================================================================================================================
class Fireball : public ActiveSpell {
public:
    Fireball() : ActiveSpell(ESpellID::FIREBALL) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class Burning : public ActiveSpell {
public:
    Burning() : ActiveSpell(ESpellID::BURNING) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class MoltenArmor : public ActiveSpell {
public:
    MoltenArmor() : ActiveSpell(ESpellID::MOLTEN_ARMOR) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class Exposure : public ActiveSpell {
public:
    Exposure() : ActiveSpell(ESpellID::EXPOSURE) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class Stoneskin : public ActiveSpell {
public:
    Stoneskin() : ActiveSpell(ESpellID::STONESKIN) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class Disarm : public ActiveSpell {
public:
    Disarm() : ActiveSpell(ESpellID::DISARM) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};

class Bloodbath : public ActiveSpell {
public:
    Bloodbath() : ActiveSpell(ESpellID::BLOODBATH) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};

class ArcaneInfusion : public ActiveSpell {
public:
    ArcaneInfusion() : ActiveSpell(ESpellID::ARCANE_INFUSION) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};

class BloodRain : public ActiveSpell {
public:
    BloodRain() : ActiveSpell(ESpellID::BLOOD_RAIN) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};

class ViscousAcid : public ActiveSpell {
public:
    ViscousAcid() : ActiveSpell(ESpellID::VISCOUS_ACID) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};






//==============================================================================  MISC  ============================================================================================
//==================================================================================================================================================================================

class Blind : public ActiveSpell {
public:
    Blind() : ActiveSpell(ESpellID::BLIND) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};


//=============================================================================  SUMMON  ===========================================================================================
//==================================================================================================================================================================================

class SummonFireElemental : public ActiveSpell {
public:
    SummonFireElemental() : ActiveSpell(ESpellID::SUM_FIRE_ELE) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};

class SummonFireImp : public ActiveSpell {
public:
    SummonFireImp() : ActiveSpell(ESpellID::SUM_FIRE_IMP) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};




//============================================================================== MELEE =============================================================================================
//==================================================================================================================================================================================

class Melee : public ActiveSpell {
public:
    Melee() : ActiveSpell(ESpellID::MELEE) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};





//============================================================================== RANGED =============================================================================================
//===================================================================================================================================================================================

class Ranged : public ActiveSpell {
public:
    Ranged() : ActiveSpell(ESpellID::RANGED) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) override;
};