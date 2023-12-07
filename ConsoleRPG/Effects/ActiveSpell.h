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

    //inline const float GetEffectMax(int idx, int level) const { return _effect_data[idx]->_e[level]._effect_max; }
	//inline const float GetEffectMin(int idx, int level) const { return _effect_data[idx]->_e[level]._effect_min; }
	//inline const float GetOnApplyMax(int idx, int level) const { return _effect_data[idx]->_e[level]._apply_max; }
	//inline const float GetOnApplyMin(int idx, int level) const { return _effect_data[idx]->_e[level]._apply_min; }
	//inline const int GetDuration(int idx, int level) const { return _effect_data[idx]->_e[level]._duration; }
	//inline const int GetCooldown(int idx, int level) const { return _effect_data[idx]->_e[level]._cooldown; }
	//inline const int GetCost(int idx, int level) const { return _effect_data[idx]->_e[level]._cost; }

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

//------------------------------------     FIREBALL      ---------------------------------------
class Fireball : public ActiveSpell {
public:
    Fireball(ESpellID id) : ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class Burning : public ActiveSpell {
public:
    Burning(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class MoltenArmor : public ActiveSpell {
public:
    MoltenArmor(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class Exposure : public ActiveSpell {
public:
    Exposure(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};
//================================================================================================
//------------------------------------     STONESKIN       ---------------------------------------
class Stoneskin : public ActiveSpell {
public:
    Stoneskin(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};
class Disarm : public ActiveSpell {
public:
    Disarm(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class Thorns : public ActiveSpell {
public:
    Thorns(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};

class Bloodbath : public ActiveSpell {
public:
    Bloodbath(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
//================================================================================================
//--------------------------------     ARCANE INFUSION     ---------------------------------------
class ArcaneInfusion : public ActiveSpell {
public:
    ArcaneInfusion(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
class AI_TEMP1 : public ActiveSpell {
public:
    AI_TEMP1(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class AI_TEMP2 : public ActiveSpell {
public:
    AI_TEMP2(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class AI_TEMP3 : public ActiveSpell {
public:
    AI_TEMP3(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
//================================================================================================
//------------------------------------     BLOOD RAIN      ---------------------------------------
class BloodRain : public ActiveSpell {
public:
    BloodRain(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
class BR_TEMP1 : public ActiveSpell {
public:
    BR_TEMP1(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class BR_TEMP2 : public ActiveSpell {
public:
    BR_TEMP2(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class BR_TEMP3 : public ActiveSpell {
public:
    BR_TEMP3(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
//================================================================================================
//------------------------------------     VISCOUS ACID    ---------------------------------------
class ViscousAcid : public ActiveSpell {
public:
    ViscousAcid(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    virtual stringstream& GetTooltip() override;
};
class VA_TEMP1 : public ActiveSpell {
public:
    VA_TEMP1(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class VA_TEMP2 : public ActiveSpell {
public:
    VA_TEMP2(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};

class VA_TEMP3 : public ActiveSpell {
public:
    VA_TEMP3(ESpellID id) :ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
};
