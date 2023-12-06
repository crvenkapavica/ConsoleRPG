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

shared_ptr<SpellBook> SpellBook::CreateSpellBook(ESpellBookID spellID, int level) {

	//shared_ptr<SpellBook> spell_book;
	//vector<shared_ptr<ActiveSpell>> spells;

	//switch (spellID) {
	//	case ESpellBookID::FIREBALL: {
	//		spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::FIREBALL, ESpellActivity::ACTIVE), level);
	//		spells.push_back(make_shared<FireballEffect>(ESpellID::FIREBALL, spell_book.get(), EDamageType::FIRE, ESpellType::PROJECTILE, 0));
	//		spells.push_back(make_shared<BurningEffect>(ESpellID::BURNING, spell_book.get(), EDamageType::FIRE, ESpellType::DEBUFF, 1));
	//		spells.push_back(make_shared<MoltenArmorEffect>(ESpellID::MOLTEN_ARMOR, spell_book.get(), EDamageType::FIRE, ESpellType::DEBUFF, 2));
	//		spells.push_back(make_shared<ExposureEffect>(ESpellID::EXPOSURE, spell_book.get(), EDamageType::FIRE, ESpellType::DEBUFF, 3));
	//		spell_book->SetActiveSpells(spells);
	//	} break;
	//	case ESpellBookID::STONESKIN: {
	//		spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::STONESKIN, ESpellActivity::ACTIVE), level);
	//		spells.push_back(make_shared<StoneskinEffect>(ESpellID::STONESKIN, spell_book.get(), EDamageType::FIRE, ESpellType::BUFF, 0));
	//		spells.push_back(make_shared<DisarmEffect>(ESpellID::DISARM, spell_book.get(), EDamageType::FIRE, ESpellType::DEBUFF, 1));
	//		spells.push_back(make_shared<ThornsEffect>(ESpellID::THRONS, spell_book.get(), EDamageType::FIRE, ESpellType::AURA, 2));
	//		spells.push_back(make_shared<BloodbathEffect>(ESpellID::BLOODBATH, spell_book.get(), EDamageType::FIRE, ESpellType::BUFF, 3));
	//		spell_book->SetActiveSpells(spells);
	//	} break;
	//	case ESpellBookID::ARCANE_INFUSION: {
	//		spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::ARCANE_INFUSION, ESpellActivity::ACTIVE), level);
	//		spells.push_back(make_shared<ArcaneInfusionEffect>(ESpellID::ARCANE_INFUSION, spell_book.get(), EDamageType::ARCANE, ESpellType::BUFF, 0));
	//		spells.push_back(make_shared<AI_TEMP1>(ESpellID::AI_TEMP1, spell_book.get(), EDamageType::ARCANE, ESpellType::BUFF, 1));
	//		spells.push_back(make_shared<AI_TEMP2>(ESpellID::AI_TEMP2, spell_book.get(), EDamageType::ARCANE, ESpellType::BUFF, 2));
	//		spells.push_back(make_shared<AI_TEMP3>(ESpellID::AI_TEMP3, spell_book.get(), EDamageType::ARCANE, ESpellType::BUFF, 3));
	//		spell_book->SetActiveSpells(spells);
	//	} break;
	//	case ESpellBookID::BLOOD_RAIN: {
	//		spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::BLOOD_RAIN, ESpellActivity::ACTIVE), level);
	//		spells.push_back(make_shared<BloodRainEffect>(ESpellID::BLOOD_RAIN, spell_book.get(), EDamageType::NECROTIC, ESpellType::DEBUFF, 0));
	//		spells.push_back(make_shared<BR_TEMP1>(ESpellID::BR_TEMP1, spell_book.get(), EDamageType::NECROTIC, ESpellType::DEBUFF, 1));
	//		spells.push_back(make_shared<BR_TEMP2>(ESpellID::BR_TEMP2, spell_book.get(), EDamageType::NECROTIC, ESpellType::DEBUFF, 2));
	//		spells.push_back(make_shared<BR_TEMP3>(ESpellID::BR_TEMP3, spell_book.get(), EDamageType::NECROTIC, ESpellType::DEBUFF, 3));
	//		spell_book->SetActiveSpells(spells);
	//	} break;
	//	case ESpellBookID::VISCOUS_ACID: {
	//		spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::VISCOUS_ACID, ESpellActivity::ACTIVE), level);
	//		spells.push_back(make_shared<ViscousAcidEffect>(ESpellID::VISCOUS_ACID, spell_book.get(), EDamageType::POISON, ESpellType::DEBUFF, 0));
	//		spells.push_back(make_shared<VA_TEMP1>(ESpellID::VA_TEMP1, spell_book.get(), EDamageType::POISON, ESpellType::DEBUFF, 1));
	//		spells.push_back(make_shared<VA_TEMP2>(ESpellID::VA_TEMP2, spell_book.get(), EDamageType::POISON, ESpellType::DEBUFF, 2));
	//		spells.push_back(make_shared<VA_TEMP3>(ESpellID::VA_TEMP3, spell_book.get(), EDamageType::POISON, ESpellType::DEBUFF, 3));
	//		spell_book->SetActiveSpells(spells);
	//	} break;
	//	default:
	//		throw std::invalid_argument("Invalid spell ID");
	//}

	//return spell_book;

	return nullptr;
}

void SpellBook::InvokeEffect(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<int>& t1_idx, vector<int>& t2_idx, int effect_idx) {

	// Casting the spell with default effect (no bonus) - this always casts
	_active_effects[0]->Apply(instigator, team1, team2, t1_idx, t2_idx); 

	// If effect_idx > 0 means we have chosen an additional effect and we cast it by the index
	if (effect_idx > 0)
		_active_effects[effect_idx]->Apply(instigator, team1, team2, t1_idx, t2_idx);
}