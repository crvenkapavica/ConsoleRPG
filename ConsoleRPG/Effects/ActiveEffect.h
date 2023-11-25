#pragma once

#include "../RPGTypes.h"
#include "../Effects/Effect.h"
#include "../Characters/Character.h"

class PlayerCharacter;
class EnemyCharacter;
class Spell;
struct EffectParams;

class ActiveEffect : public Effect {
public:
    ActiveEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx);

    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) = 0;

    inline const Spell* GetSpell() const { return _spell; }

    inline const EDamageType GetDamageType() const { return _damage_type; }

    inline const ESpellType GetSpellType() const { return _spell_type; }

protected:

    const Spell* _spell;

    const EDamageType _damage_type;

    const ESpellType _spell_type;

    int _idx;

protected:
    float GetRandFloat(float a, float b);

    // Get random damage between spell min and max value at current level
    float GetRandEffectMinMax();
    // Get random damage between spell min and max value for on aplly effect at current level
    float GetRandOnApplyMinMax();

    // Add random targets to the target vector (when multi-striking, or when spell targets more than one character)
    int AddRandomTargets(int r, const vector<weak_ptr<Character>>& enemies, vector<int>& index, const string& name);
};

//------------------------------------     FIREBALL      ---------------------------------------
class FireballEffect : public ActiveEffect {
public:
    FireballEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};
class BurningEffect : public ActiveEffect {
public:
    BurningEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class MoltenArmorEffect : public ActiveEffect {
public:
    MoltenArmorEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class ExposureEffect : public ActiveEffect {
public:
    ExposureEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};
//================================================================================================
//------------------------------------     STONESKIN       ---------------------------------------
class StoneskinEffect : public ActiveEffect {
public:
    StoneskinEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};
class DisarmEffect : public ActiveEffect {
public:
    DisarmEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class ThornsEffect : public ActiveEffect {
public:
    ThornsEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class BloodbathEffect : public ActiveEffect {
public:
    BloodbathEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
//================================================================================================
//--------------------------------     ARCANE INFUSION     ---------------------------------------
class ArcaneInfusionEffect : public ActiveEffect {
public:
    ArcaneInfusionEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
class AI_TEMP1 : public ActiveEffect {
public:
    AI_TEMP1(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class AI_TEMP2 : public ActiveEffect {
public:
    AI_TEMP2(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class AI_TEMP3 : public ActiveEffect {
public:
    AI_TEMP3(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
//================================================================================================
//------------------------------------     BLOOD RAIN      ---------------------------------------
class BloodRainEffect : public ActiveEffect {
public:
    BloodRainEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
class BR_TEMP1 : public ActiveEffect {
public:
    BR_TEMP1(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class BR_TEMP2 : public ActiveEffect {
public:
    BR_TEMP2(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class BR_TEMP3 : public ActiveEffect {
public:
    BR_TEMP3(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
//================================================================================================
//------------------------------------     VISCOUS ACID    ---------------------------------------
class ViscousAcidEffect : public ActiveEffect {
public:
    ViscousAcidEffect(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};
class VA_TEMP1 : public ActiveEffect {
public:
    VA_TEMP1(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class VA_TEMP2 : public ActiveEffect {
public:
    VA_TEMP2(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class VA_TEMP3 : public ActiveEffect {
public:
    VA_TEMP3(EEffectID id, const Spell* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveEffect(id, spell, damage_type, spell_type, idx) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
