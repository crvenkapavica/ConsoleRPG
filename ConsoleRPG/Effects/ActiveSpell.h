#pragma once

#include "../RPGTypes.h"
#include "../Effects/Spell.h"
#include "../Characters/Character.h"

class PlayerCharacter;
class EnemyCharacter;
class SpellBook;
struct EffectParams;

class ActiveSpell : public Spell {
public:
    ActiveSpell(ESpellID id, int lvl = 0);

    static unique_ptr<ActiveSpell> CreateActiveSpell(ESpellID id);

    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) = 0;

    //	// Invoke the default spell effect + an additional effect (idx) that the spell supports (if chosen)
	void InvokeEffect(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<int>& t1_idx, vector<int>& t2_idx);

    inline const EDamageType GetDamageType() const { return _damage_type; }
    inline const ESpellType GetSpellType() const { return _spell_type; }

protected:

    const EDamageType _damage_type;
    const ESpellType _spell_type;

protected:
    // Get random damage between spell min and max value at current level
    float GetRandEffectMinMax(Character* character);
    // Get random damage between spell min and max value for on aplly effect at current level
    float GetRandOnApplyMinMax(Character* character);

    float AdjustDamage(float damage, Character* character);

    // Add random targets to the target vector (when multi-striking, or when spell targets more than one character)
    int AddRandomTargets(int r, const vector<weak_ptr<Character>>& enemies, vector<int>& index, const string& name);
};

//============================================================================== MAGIC =============================================================================================
//==================================================================================================================================================================================
class Fireball : public ActiveSpell {
public:
    Fireball() : ActiveSpell(ESpellID::FIREBALL) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class Burning : public ActiveSpell {
public:
    Burning() : ActiveSpell(ESpellID::BURNING) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class MoltenArmor : public ActiveSpell {
public:
    MoltenArmor() : ActiveSpell(ESpellID::MOLTEN_ARMOR) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class Exposure : public ActiveSpell {
public:
    Exposure() : ActiveSpell(ESpellID::EXPOSURE) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class Stoneskin : public ActiveSpell {
public:
    Stoneskin() : ActiveSpell(ESpellID::STONESKIN) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class Disarm : public ActiveSpell {
public:
    Disarm() : ActiveSpell(ESpellID::DISARM) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class Thorns : public ActiveSpell {
public:
    Thorns() : ActiveSpell(ESpellID::THRONS) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class Bloodbath : public ActiveSpell {
public:
    Bloodbath() : ActiveSpell(ESpellID::BLOODBATH) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class ArcaneInfusion : public ActiveSpell {
public:
    ArcaneInfusion() : ActiveSpell(ESpellID::ARCANE_INFUSION) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class AI_TEMP1 : public ActiveSpell {
public:
    AI_TEMP1() : ActiveSpell(ESpellID::AI_TEMP1) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class AI_TEMP2 : public ActiveSpell {
public:
    AI_TEMP2() : ActiveSpell(ESpellID::AI_TEMP2) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class AI_TEMP3 : public ActiveSpell {
public:
    AI_TEMP3() : ActiveSpell(ESpellID::AI_TEMP3) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class BloodRain : public ActiveSpell {
public:
    BloodRain() : ActiveSpell(ESpellID::BLOOD_RAIN) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class BR_TEMP1 : public ActiveSpell {
public:
    BR_TEMP1() : ActiveSpell(ESpellID::BR_TEMP1) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class BR_TEMP2 : public ActiveSpell {
public:
    BR_TEMP2() : ActiveSpell(ESpellID::BR_TEMP2) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class BR_TEMP3 : public ActiveSpell {
public:
    BR_TEMP3() : ActiveSpell(ESpellID::BR_TEMP3) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class ViscousAcid : public ActiveSpell {
public:
    ViscousAcid() : ActiveSpell(ESpellID::VISCOUS_ACID) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class VA_TEMP1 : public ActiveSpell {
public:
    VA_TEMP1() : ActiveSpell(ESpellID::VA_TEMP1) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class VA_TEMP2 : public ActiveSpell {
public:
    VA_TEMP2() : ActiveSpell(ESpellID::VA_TEMP2) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class VA_TEMP3 : public ActiveSpell {
public:
    VA_TEMP3() :ActiveSpell(ESpellID::VA_TEMP3) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};





//============================================================================== MELEE =============================================================================================
//==================================================================================================================================================================================

class Melee : public ActiveSpell {
public:
    Melee() : ActiveSpell(ESpellID::MELEE) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};





//============================================================================== RANGED =============================================================================================
//===================================================================================================================================================================================

class Ranged : public ActiveSpell {
public:
    Ranged() : ActiveSpell(ESpellID::RANGED) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};