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
    ActiveSpell(ESpellID id, int level = 0);

    static unique_ptr<ActiveSpell> CreateSpellBook(ESpellID id);

    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) = 0;

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

protected:
    static std::unordered_map<ESpellID, pair<EDamageType, ESpellType>> _spell_map;
    static struct SpellMapConstructor {
        SpellMapConstructor();
    } _spell_map_constructor;
};

//------------------------------------     FIREBALL      ---------------------------------------
class Fireball : public ActiveSpell {
public:
    Fireball(ESpellID id) : ActiveSpell(id) {}
    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
    //virtual stringstream& GetTooltip() override;
};

//class BurningEffect : public ActiveSpell {
//public:
//    BurningEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//    virtual stringstream& GetTooltip() override;
//};
//
//class MoltenArmorEffect : public ActiveSpell {
//public:
//    MoltenArmorEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//    virtual stringstream& GetTooltip() override;
//};
//
//class ExposureEffect : public ActiveSpell {
//public:
//    ExposureEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//    virtual stringstream& GetTooltip() override;
//};
////================================================================================================
////------------------------------------     STONESKIN       ---------------------------------------
//class StoneskinEffect : public ActiveSpell {
//public:
//    StoneskinEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//    virtual stringstream& GetTooltip() override;
//};
//class DisarmEffect : public ActiveSpell {
//public:
//    DisarmEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//
//class ThornsEffect : public ActiveSpell {
//public:
//    ThornsEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//    virtual stringstream& GetTooltip() override;
//};
//
//class BloodbathEffect : public ActiveSpell {
//public:
//    BloodbathEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
////================================================================================================
////--------------------------------     ARCANE INFUSION     ---------------------------------------
//class ArcaneInfusionEffect : public ActiveSpell {
//public:
//    ArcaneInfusionEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//class AI_TEMP1 : public ActiveSpell {
//public:
//    AI_TEMP1(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//
//class AI_TEMP2 : public ActiveSpell {
//public:
//    AI_TEMP2(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//
//class AI_TEMP3 : public ActiveSpell {
//public:
//    AI_TEMP3(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
////================================================================================================
////------------------------------------     BLOOD RAIN      ---------------------------------------
//class BloodRainEffect : public ActiveSpell {
//public:
//    BloodRainEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//class BR_TEMP1 : public ActiveSpell {
//public:
//    BR_TEMP1(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//
//class BR_TEMP2 : public ActiveSpell {
//public:
//    BR_TEMP2(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//
//class BR_TEMP3 : public ActiveSpell {
//public:
//    BR_TEMP3(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
////================================================================================================
////------------------------------------     VISCOUS ACID    ---------------------------------------
//class ViscousAcidEffect : public ActiveSpell {
//public:
//    ViscousAcidEffect(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//    virtual stringstream& GetTooltip() override;
//};
//class VA_TEMP1 : public ActiveSpell {
//public:
//    VA_TEMP1(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//
//class VA_TEMP2 : public ActiveSpell {
//public:
//    VA_TEMP2(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
//
//class VA_TEMP3 : public ActiveSpell {
//public:
//    VA_TEMP3(ESpellID id, const SpellBook* spell, EDamageType damage_type, ESpellType spell_type, int idx) : ActiveSpell(id, spell, damage_type, spell_type, idx) {}
//    virtual void Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) override;
//};
