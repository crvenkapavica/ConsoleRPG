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

	shared_ptr<SpellBook> spell_book;
	vector<shared_ptr<ActiveSpell>> spells;

	switch (spellID) {
		case ESpellBookID::FIREBALL: {
			spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::FIREBALL, ESpellActivity::ACTIVE), level);
			spells.push_back(make_shared<Fireball>(ESpellID::FIREBALL));
			spells.push_back(make_shared<Burning>(ESpellID::BURNING));
			spells.push_back(make_shared<MoltenArmor>(ESpellID::MOLTEN_ARMOR));
			spells.push_back(make_shared<Exposure>(ESpellID::EXPOSURE));
			spell_book->SetActiveSpells(spells);
		} break;
		case ESpellBookID::STONESKIN: {
			spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::STONESKIN, ESpellActivity::ACTIVE), level);
			spells.push_back(make_shared<Stoneskin>(ESpellID::STONESKIN));
			spells.push_back(make_shared<Disarm>(ESpellID::DISARM));
			spells.push_back(make_shared<Thorns>(ESpellID::THRONS));
			spells.push_back(make_shared<Bloodbath>(ESpellID::BLOODBATH));
			spell_book->SetActiveSpells(spells);
		} break;
		case ESpellBookID::ARCANE_INFUSION: {
			spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::ARCANE_INFUSION, ESpellActivity::ACTIVE), level);
			spells.push_back(make_shared<ArcaneInfusion>(ESpellID::ARCANE_INFUSION));
			spells.push_back(make_shared<AI_TEMP1>(ESpellID::AI_TEMP1));
			spells.push_back(make_shared<AI_TEMP2>(ESpellID::AI_TEMP2));
			spells.push_back(make_shared<AI_TEMP3>(ESpellID::AI_TEMP3));
			spell_book->SetActiveSpells(spells);
		} break;
		case ESpellBookID::BLOOD_RAIN: {
			spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::BLOOD_RAIN, ESpellActivity::ACTIVE), level);
			spells.push_back(make_shared<BloodRain>(ESpellID::BLOOD_RAIN));
			spells.push_back(make_shared<BR_TEMP1>(ESpellID::BR_TEMP1));
			spells.push_back(make_shared<BR_TEMP2>(ESpellID::BR_TEMP2));
			spells.push_back(make_shared<BR_TEMP3>(ESpellID::BR_TEMP3));
			spell_book->SetActiveSpells(spells);
		} break;
		case ESpellBookID::VISCOUS_ACID: {
			spell_book = make_shared<SpellBook>(CSpellData(ESpellBookID::VISCOUS_ACID, ESpellActivity::ACTIVE), level);
			spells.push_back(make_shared<ViscousAcid>(ESpellID::VISCOUS_ACID));
			spells.push_back(make_shared<VA_TEMP1>(ESpellID::VA_TEMP1));
			spells.push_back(make_shared<VA_TEMP2>(ESpellID::VA_TEMP2));
			spells.push_back(make_shared<VA_TEMP3>(ESpellID::VA_TEMP3));
			spell_book->SetActiveSpells(spells);
		} break;
		default:
			throw std::invalid_argument("Invalid spell ID");
	}

	return spell_book;

	return nullptr;
}

void SpellBook::InvokeEffect(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<int>& t1_idx, vector<int>& t2_idx, int effect_idx) {

	// Casting the spell with default effect (no bonus) - this always casts
	_active_effects[0]->Apply(instigator, team1, team2, t1_idx, t2_idx); 

	// If effect_idx > 0 means we have chosen an additional effect and we cast it by the index
	if (effect_idx > 0)
		_active_effects[effect_idx]->Apply(instigator, team1, team2, t1_idx, t2_idx);
}