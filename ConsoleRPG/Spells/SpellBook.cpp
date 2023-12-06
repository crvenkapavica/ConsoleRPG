#include "SpellBook.h"
#include "../Characters/Character.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Effects/EffectStructs.h"


SpellBook::SpellBook(CSpellData&& data, int level)
	: _level(level)
	, _experience(0)
	, _ID(data.GetSpellConstData()->_ID)
	, _spell_activity(data.GetSpellConstData()->_spell_activity)
	, _spell_rarity(data.GetSpellConstData()->_spell_rarity)
{
	if (_spell_activity == ESpellActivity::ACTIVE) {
		_effect_levels = data.GetEffectLevels();
		_effect_data = move(data.GetEffectData());
	}
}

shared_ptr<SpellBook> SpellBook::CreateSpell(ESpellID spellID, int level) {

	shared_ptr<SpellBook> spell;
	vector<shared_ptr<ActiveSpell>> effects;

	switch (spellID) {
		case ESpellID::FIREBALL: {
			spell = make_shared<SpellBook>(CSpellData(ESpellID::FIREBALL, ESpellActivity::ACTIVE), level);
			effects.push_back(make_shared<FireballEffect>(EEffectID::FIREBALL, spell.get(), EDamageType::FIRE, ESpellType::PROJECTILE, 0));
			effects.push_back(make_shared<BurningEffect>(EEffectID::BURNING, spell.get(), EDamageType::FIRE, ESpellType::DEBUFF, 1));
			effects.push_back(make_shared<MoltenArmorEffect>(EEffectID::MOLTEN_ARMOR, spell.get(), EDamageType::FIRE, ESpellType::DEBUFF, 2));
			effects.push_back(make_shared<ExposureEffect>(EEffectID::EXPOSURE, spell.get(), EDamageType::FIRE, ESpellType::DEBUFF, 3));
			spell->SetActiveEffects(effects);
		} break;
		case ESpellID::STONESKIN: {
			spell = make_shared<SpellBook>(CSpellData(ESpellID::STONESKIN, ESpellActivity::ACTIVE), level);
			effects.push_back(make_shared<StoneskinEffect>(EEffectID::STONESKIN, spell.get(), EDamageType::FIRE, ESpellType::BUFF, 0));
			effects.push_back(make_shared<DisarmEffect>(EEffectID::DISARM, spell.get(), EDamageType::FIRE, ESpellType::DEBUFF, 1));
			effects.push_back(make_shared<ThornsEffect>(EEffectID::THRONS, spell.get(), EDamageType::FIRE, ESpellType::AURA, 2));
			effects.push_back(make_shared<BloodbathEffect>(EEffectID::BLOODBATH, spell.get(), EDamageType::FIRE, ESpellType::BUFF, 3));
			spell->SetActiveEffects(effects);
		} break;
		case ESpellID::ARCANE_INFUSION: {
			spell = make_shared<SpellBook>(CSpellData(ESpellID::ARCANE_INFUSION, ESpellActivity::ACTIVE), level);
			effects.push_back(make_shared<ArcaneInfusionEffect>(EEffectID::ARCANE_INFUSION, spell.get(), EDamageType::ARCANE, ESpellType::BUFF, 0));
			effects.push_back(make_shared<AI_TEMP1>(EEffectID::AI_TEMP1, spell.get(), EDamageType::ARCANE, ESpellType::BUFF, 1));
			effects.push_back(make_shared<AI_TEMP2>(EEffectID::AI_TEMP2, spell.get(), EDamageType::ARCANE, ESpellType::BUFF, 2));
			effects.push_back(make_shared<AI_TEMP3>(EEffectID::AI_TEMP3, spell.get(), EDamageType::ARCANE, ESpellType::BUFF, 3));
			spell->SetActiveEffects(effects);
		} break;
		case ESpellID::BLOOD_RAIN: {
			spell = make_shared<SpellBook>(CSpellData(ESpellID::BLOOD_RAIN, ESpellActivity::ACTIVE), level);
			effects.push_back(make_shared<BloodRainEffect>(EEffectID::BLOOD_RAIN, spell.get(), EDamageType::NECROTIC, ESpellType::DEBUFF, 0));
			effects.push_back(make_shared<BR_TEMP1>(EEffectID::BR_TEMP1, spell.get(), EDamageType::NECROTIC, ESpellType::DEBUFF, 1));
			effects.push_back(make_shared<BR_TEMP2>(EEffectID::BR_TEMP2, spell.get(), EDamageType::NECROTIC, ESpellType::DEBUFF, 2));
			effects.push_back(make_shared<BR_TEMP3>(EEffectID::BR_TEMP3, spell.get(), EDamageType::NECROTIC, ESpellType::DEBUFF, 3));
			spell->SetActiveEffects(effects);
		} break;
		case ESpellID::VISCOUS_ACID: {
			spell = make_shared<SpellBook>(CSpellData(ESpellID::VISCOUS_ACID, ESpellActivity::ACTIVE), level);
			effects.push_back(make_shared<ViscousAcidEffect>(EEffectID::VISCOUS_ACID, spell.get(), EDamageType::POISON, ESpellType::DEBUFF, 0));
			effects.push_back(make_shared<VA_TEMP1>(EEffectID::VA_TEMP1, spell.get(), EDamageType::POISON, ESpellType::DEBUFF, 1));
			effects.push_back(make_shared<VA_TEMP2>(EEffectID::VA_TEMP2, spell.get(), EDamageType::POISON, ESpellType::DEBUFF, 2));
			effects.push_back(make_shared<VA_TEMP3>(EEffectID::VA_TEMP3, spell.get(), EDamageType::POISON, ESpellType::DEBUFF, 3));
			spell->SetActiveEffects(effects);
		} break;
		default:
			throw std::invalid_argument("Invalid spell ID");
	}

	return spell;
}

void SpellBook::InvokeEffect(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<int>& t1_idx, vector<int>& t2_idx, int effect_idx) {

	// Casting the spell with default effect (no bonus) - this always casts
	_active_effects[0]->Apply(instigator, team1, team2, t1_idx, t2_idx); 

	// If effect_idx > 0 means we have chosen an additional effect and we cast it by the index
	if (effect_idx > 0)
		_active_effects[effect_idx]->Apply(instigator, team1, team2, t1_idx, t2_idx);
}