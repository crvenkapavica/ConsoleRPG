#pragma once

#include "../RPGTypes.h"
#include "../Characters/Character.h"
#include "../Spells/Spell.h"

//class Character;
class PlayerCharacter;
class EnemyCharacter;
struct EffectParams;

class ActiveSpell : public Spell {
public:
    explicit ActiveSpell(const ESpellID SpellID, const int InLevel = 0);
    
    static std::unique_ptr<ActiveSpell> CreateActiveSpell(ESpellID SpellID);

    // Apply the effect of the spell
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) = 0;

    inline EDamageType GetDamageType() const { return DamageType; }
    inline ESpellType GetSpellType() const { return SpellType; }

protected:
    // Get random damage between spell min and max value at current level
    float GetRandEffectMinMax(const std::shared_ptr<Character>& InCharacter);

    // Get random damage between spell min and max value for on apply effect at current level
    float GetRandOnApplyMinMax(const std::shared_ptr<Character>& InCharacter);

    // Apply resistances, armor reduction, critical chance and other procs and return final damage
    float AdjustDamage(float Damage, const std::shared_ptr<Character>& InCharacter);

    // Add random targets to the target vector (when multi-striking, or when spell targets more than one character)
    int AddRandomTargets(int R, std::vector<std::weak_ptr<Character>>& Targets, const std::shared_ptr<Character>& character, const std::string& name);

    bool Summon(ECharacterClass CharacterClass, const std::shared_ptr<Character>& Instigator);

protected:
    const EDamageType DamageType;
    const ESpellType SpellType;
};

//============================================================================== MAGIC =============================================================================================
//==================================================================================================================================================================================
class Fireball final : public ActiveSpell {
public:
    Fireball() : ActiveSpell(ESpellID::FIREBALL) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class Burning final : public ActiveSpell {
public:
    Burning() : ActiveSpell(ESpellID::BURNING) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class MoltenArmor final : public ActiveSpell {
public:
    MoltenArmor() : ActiveSpell(ESpellID::MOLTEN_ARMOR) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class Exposure final : public ActiveSpell {
public:
    Exposure() : ActiveSpell(ESpellID::EXPOSURE) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class Stoneskin final : public ActiveSpell {
public:
    Stoneskin() : ActiveSpell(ESpellID::STONESKIN) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};

class Disarm final : public ActiveSpell {
public:
    Disarm() : ActiveSpell(ESpellID::DISARM) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};

class Bloodbath final : public ActiveSpell {
public:
    Bloodbath() : ActiveSpell(ESpellID::BLOODBATH) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};

class ArcaneInfusion final : public ActiveSpell {
public:
    ArcaneInfusion() : ActiveSpell(ESpellID::ARCANE_INFUSION) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};

class BloodRain final : public ActiveSpell {
public:
    BloodRain() : ActiveSpell(ESpellID::BLOOD_RAIN) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};

class ViscousAcid final : public ActiveSpell {
public:
    ViscousAcid() : ActiveSpell(ESpellID::VISCOUS_ACID) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
    virtual std::stringstream& GetTooltip() override;
};






//==============================================================================  MISC  ============================================================================================
//==================================================================================================================================================================================

class Blind final : public ActiveSpell {
public:
    Blind() : ActiveSpell(ESpellID::BLIND) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};


//=============================================================================  SUMMON  ===========================================================================================
//==================================================================================================================================================================================

class SummonFireElemental final : public ActiveSpell {
public:
    SummonFireElemental() : ActiveSpell(ESpellID::SUM_FIRE_ELE) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};

class SummonFireImp final : public ActiveSpell {
public:
    SummonFireImp() : ActiveSpell(ESpellID::SUM_FIRE_IMP) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};




//============================================================================== MELEE =============================================================================================
//==================================================================================================================================================================================

class Melee final : public ActiveSpell {
public:
    Melee() : ActiveSpell(ESpellID::MELEE) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};





//============================================================================== RANGED =============================================================================================
//===================================================================================================================================================================================

class Ranged final : public ActiveSpell {
public:
    Ranged() : ActiveSpell(ESpellID::RANGED) {}
    virtual void Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) override;
};