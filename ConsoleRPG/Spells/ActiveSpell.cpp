#include "../Spells/ActiveSpell.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Spells/SpellData.h"
#include "../GameplayStatics.h"
#include "../Spells/EffectStructs.h"

ActiveSpell::ActiveSpell(ESpellID id, int lvl)
	: Spell(id, ESpellActivity::ACTIVE, SpellDB::_active_const_map.at(id)._rarity, SpellDB::_active_const_map.at(id)._class, SpellDB::_active_const_map.at(id)._min_req_lvl, lvl)
	, _damage_type(SpellDB::_active_const_map.at(id)._damage_type)
	, _spell_type(SpellDB::_active_const_map.at(id)._spell_type)
{}

unique_ptr<ActiveSpell> ActiveSpell::CreateActiveSpell(ESpellID id) {
	switch (id) {
		// MAGIC
	case ESpellID::FIREBALL:
		return make_unique<Fireball>();
	case ESpellID::BURNING:
		return make_unique<Burning>();
	case ESpellID::MOLTEN_ARMOR:
		return make_unique<MoltenArmor>();
	case ESpellID::EXPOSURE:
		return make_unique<Exposure>();
	case ESpellID::STONESKIN:
		return make_unique<Stoneskin>();
	case ESpellID::ARCANE_INFUSION:
		return make_unique<ArcaneInfusion>();
	case ESpellID::BLOOD_RAIN:
		return make_unique<BloodRain>();
	case ESpellID::VISCOUS_ACID:
		return make_unique<ViscousAcid>();

		//SUMMON
	case ESpellID::SUM_FIRE_ELE:
		return make_unique<SummonFireElemental>();


		// MELEE
	case ESpellID::MELEE:
		return make_unique<Melee>();

		// RANGED
	case ESpellID::RANGED:
		return make_unique<Ranged>();

	default:
		return nullptr;
	}
}

float ActiveSpell::GetRandEffectMinMax(Character* character) {
	return AdjustDamage(GameplayStatics::GetRandFloat(SpellDB::_data[_ID][_lvl]._effect_min, SpellDB::_data[_ID][_lvl]._effect_max), character);
}

float ActiveSpell::GetRandOnApplyMinMax(Character* character) {
	return AdjustDamage(GameplayStatics::GetRandFloat(SpellDB::_data[_ID][_lvl]._apply_min, SpellDB::_data[_ID][_lvl]._apply_max), character);
}

float ActiveSpell::AdjustDamage(float damage, Character* character) {	
	switch (_damage_type) {
	case EDamageType::ARCANE:
		damage += damage * character->_arcane_damage;
		break;
	case EDamageType::FIRE:
		damage += damage * character->_fire_damage;
		break;
	case EDamageType::LIGHTNING:
		damage += damage * character->_lightning_damage;
		break;
	case EDamageType::COLD:
		damage += damage * character->_cold_damage;
		break;
	case EDamageType::POISON:
		damage += damage * character->_poison_damage;
		break;
	case EDamageType::NECROTIC:
		damage += damage * character->_necrotic_damage;
		break;
	case EDamageType::PHYSICAL:
		damage += damage * character->_physical_damage;
		damage += character->GetAP().GetActual();
	case EDamageType::HEALING:
		damage += damage * character->_healing;
		break;
	default:
		break;
	}

	if (_damage_type != EDamageType::PHYSICAL && _damage_type != EDamageType::NONE)
		damage += character->GetSP().GetActual();

	// Critical Strike
	int rnd = GameplayStatics::GetRandInt(0, 100000);
	if (GetClass() == ESpellClass::MAGIC) {
		float chance = character->GetSpellCritChance().GetActual() * 100000;
		if (rnd <= chance)
			damage *= character->GetSpellCritDmg().GetActual();
	}
	else if (GetClass() == ESpellClass::MELEE || GetClass() == ESpellClass::RANGED) {
		float chance = character->GetCritChance().GetActual() * 100000; 
		if (rnd <= chance)
			damage *= character->GetCritDmg().GetActual();
	}

	return damage;
}

int ActiveSpell::AddRandomTargets(int r, vector<weak_ptr<Character>>& targets, Character* character, const string& name) {
	vector<weak_ptr<Character>> enemies;
	if (character->GetTeam() == 1)
		enemies = GameplayStatics::GetEnemyCharacters();
	else
		enemies = GameplayStatics::GetPlayerCharacters();

	int expired = static_cast<int>(count_if(enemies.begin(), enemies.end(), [](const weak_ptr<Character>& wptr) { return wptr.expired(); }));
	int size = static_cast<int>(enemies.size()) - expired;
	if (size == 1) return 0;
	r = size == r ? r - 1 : r;
	
	for (int i = 0; i < r; i++) {
		int rnd;
		do {
			rnd = rand() % enemies.size();
		} while (any_of(targets.begin(), targets.end(), [&](const std::weak_ptr<Character>& wptr) { return enemies[rnd].expired() || enemies[rnd].lock().get() == wptr.lock().get(); }));
		targets.push_back(enemies[rnd]);
	}
	sort(targets.begin(), targets.end(), [&](const std::weak_ptr<Character>& a, const std::weak_ptr<Character>& b) { 
		if (a.lock() && b.lock())
			if (a.lock()->GetAlias() < b.lock()->GetAlias()) return true;
		return false;
		});

	auto& s = GameplayStatics::GetCombatLogStream();
	static string C = GameplayStatics::GetAliasColor(targets[0].lock()->GetAlias());
	s << "Characters: " << C << targets[0].lock()->GetAlias() << COLOR_COMBAT_LOG << ", " << C;
	for (int i = 0; i < r; i++) {
		s << targets[i + 1].lock()->GetAlias(); // POGLEDATI KAJ JE TAJ WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (i != r - 1) s << COLOR_COMBAT_LOG << ", " << C;
	}
	s << COLOR_COMBAT_LOG << " got hit by " << COLOR_EFFECT << name << COLOR_COMBAT_LOG << ".\n";
	return r;
}

void Fireball::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

	vector<CharacterStat> enemy_apply_stats;
	auto stat = &targets[0].lock().get()->GetHealth().GetActual();
	auto delta = [&](Character* character) { return -GetRandOnApplyMinMax(character); };
	enemy_apply_stats.push_back(CharacterStat{ targets[0].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, stat, delta});
	ApplyParams apply_params;
	apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_stats));

	unique_ptr<Fireball> spell = make_unique<Fireball>();
	GameplayStatics::ApplyEffect(instigator, targets, move(spell), apply_params, {});
}

stringstream& Fireball::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << COLOR_INFO << "Hits the target for " << COLOR_VALUE <<  << COLOR_INFO << " to " << COLOR_VALUE << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << COLOR_INFO << " damage.\n";
	//}
	return _tooltip;
}

void Burning::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

	//ApplyParams apply_params;

	//int rand_targets = AddRandomTargets(2, team2, t2_idx, "BURNING EFFECT");
	//vector<CharacterStat> enemy_effect_stats;
	//auto damage = [&](Character* character) { return -GetRandEffectMinMax(character); };
	//for (int i = 0; i <= rand_targets; i++)
	//	enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[i]].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, &team2[t2_idx[i]].lock()->GetHealth(), damage });

	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat({}, move(enemy_effect_stats), EStatValueAction::UPDATE_ACTUAL);

	//vector<weak_ptr<Character>> targets;
	//for (int i = 0; i <= rand_targets; i++)
	//	targets.push_back(team2[t2_idx[i]]);

	//shared_ptr<Burning> effect = make_shared<Burning>(_ID);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect);
}

stringstream& Burning::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Applies burn to the target and 2 random targets that deals " << COLOR_VALUE << _spell->GetEffectMin(_idx, _spell->GetLevel()) << COLOR_INFO << " to " << COLOR_VALUE << _spell->GetEffectMax(_idx, _spell->GetLevel()) << COLOR_INFO << " damage.\n";
	//	_tooltip << "The burn effect lasts for " << COLOR_VALUE << _spell->GetDuration(_idx, _spell->GetLevel()) << COLOR_INFO << " turn(s).";
	//}
	return _tooltip;
}

//============================================================================== MAGIC =============================================================================================
//==================================================================================================================================================================================

void MoltenArmor::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	
	int rand_targets = AddRandomTargets(2, targets, instigator, "MOLTEN ARMOR EFFECT");

	vector<CharacterStat> enemy_apply_stats;
	for (int i = 0; i <= rand_targets; i++) {
		auto stat = &targets[i].lock()->GetArmor().GetActual();
		auto const_delta = -GetRandOnApplyMinMax(instigator);
		auto delta = [=](Character* character) { return const_delta; };
		enemy_apply_stats.push_back(CharacterStat{ targets[i].lock().get(), EStatType::ANY, EStatMod::CONSTANT, stat, delta});
	}
	ApplyParams apply_params;
	apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_stats));

	vector<CharacterStat> enemy_effect_stats;
	for (int i = 0; i <= rand_targets; i++) {
		auto stat = &targets[i].lock()->GetArmor().GetActual();
		auto delta = [&](Character* character) { return -GetRandEffectMinMax(character); };
		enemy_effect_stats.push_back(CharacterStat{ targets[i].lock().get(), EStatType::ANY, EStatMod::ADDITIVE, stat, delta});
	}
	EffectParams effect_params;
	effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	effect_params._effect_stat = Effect_Stat({}, move(enemy_effect_stats));

	unique_ptr<MoltenArmor> spell = make_unique<MoltenArmor>();
	GameplayStatics::ApplyEffect(instigator, targets, move(spell), apply_params, effect_params);
}

stringstream& MoltenArmor::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Blast the enemy with extreme temperature which melts \n";
	//	_tooltip << "the targets armor, decreasing it by " << CV << _spell->GetOnApplyMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << CI << ".\n";
	//	_tooltip << "Additionaly, decreases the armor by " CV << _spell->GetEffectMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetEffectMax(_idx, _spell->GetLevel()) << CI << " at the end of their turn. \n";
	//	_tooltip << "Additionaly, hits 2 random enemies and applies the effect to them! \n";
	//	_tooltip << "The effect lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}

void Exposure::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

	vector<CharacterStat> enemy_apply_res;
	auto stat = &targets[0].lock()->GetResistances().GetFireRes();
	auto delta = [&](Character* character) { return -GetRandOnApplyMinMax(character); };
	enemy_apply_res.push_back(CharacterStat{ targets[0].lock().get(), EStatType::RESISTANCE, EStatMod::CONSTANT, stat, delta });
	ApplyParams apply_params;
	apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_res));

	unique_ptr<Exposure> spell = make_unique<Exposure>();
	GameplayStatics::ApplyEffect(instigator, targets, move(spell), apply_params, {});
}

stringstream& Exposure::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Cast a potent blaze of fire at the target, exposing them, \n";
	//	_tooltip << "decreasing their resistance to fire and burn spells by " << CV << _spell->GetEffectMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetEffectMax(_idx, _spell->GetLevel()) << CI ".\n";
	//	_tooltip << "The effect lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}

void Stoneskin::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

	//vector<CharacterStat> ally_apply_stats;
	//ally_apply_stats.push_back(CharacterStat{ team1[t1_idx[0]], EStatType::ANY, EStatMod::ADDITIVE, &team1[t1_idx[0]]->GetArmor(), GetRandOnApplyMinMax() });
	//ApplyParams apply_params;
	//apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//apply_params._effect_stat = Effect_Stat(move(ally_apply_stats), {}, EStatValueAction::UPDATE_ACTUAL);

	//EffectParams effect_params;

	//vector<Character*> targets = { team1[t1_idx[0]] };

	//shared_ptr<StoneskinEffect> effect = make_shared<StoneskinEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect, _idx);
}

stringstream& Stoneskin::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Hardens the targets skin, increasing armor by " << CV << _spell->GetOnApplyMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << CI << " each turn.\n";
	//	_tooltip << "The effect lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}

void Disarm::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "SS_disarm" << endl;
}

void Bloodbath::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "SS_bloodath" << endl;
}

void ArcaneInfusion::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

	//vector<CharacterStat> ally_apply_stats;
	//ally_apply_stats.push_back(CharacterStat{ team1[t1_idx[0]], EStatType::ANY, EStatMod::CONSTANT, &team1[t1_idx[0]]->GetDmgMelee(), GetRandOnApplyMinMax() });
	//ApplyParams apply_params;
	//apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//apply_params._effect_stat = Effect_Stat(move(ally_apply_stats), {}, EStatValueAction::UPDATE_ACTUAL);


	//vector<CharacterStat> ally_effect_stats;
	//ally_effect_stats.push_back(CharacterStat{ team1[t1_idx[0]], EStatType::ANY, EStatMod::ADDITIVE, &team1[t1_idx[0]]->GetDmgMelee(), GetRandEffectMinMax() });
	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;;
	//effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat(move(ally_effect_stats), {}, EStatValueAction::UPDATE_ACTUAL);

	//vector<Character*> targets = { team1[t1_idx[0]] };

	//shared_ptr<ArcaneInfusionEffect> effect = make_shared<ArcaneInfusionEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect, _idx);
}

void AI_TEMP1::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "AI_TEMP1" << endl;
}

void AI_TEMP2::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "AI_TEMP2" << endl;
}

void AI_TEMP3::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "AI_TEMP3" << endl;
}

void BloodRain::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

	//float drain = GetRandEffectMinMax();

	//ApplyParams apply_params;

	//vector<CharacterStat> ally_effect_stats;
	//ally_effect_stats.push_back(CharacterStat{ instigator, EStatType::HEALTH, EStatMod::CONSTANT, &instigator->GetHealth(), drain });
	//vector<CharacterStat> enemy_effect_stats;
	//enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::HEALTH, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetHealth(), -drain });
	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat(move(ally_effect_stats), move(enemy_effect_stats), EStatValueAction::UPDATE_ACTUAL);

	//vector<Character*> targets = { team2[t2_idx[0]] };

	//shared_ptr<BloodRainEffect> effect = make_shared<BloodRainEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect, _idx);
}

void BR_TEMP1::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "BR_TEMP1 EFFECT" << endl;
}

void BR_TEMP2::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "BR_TEMP2 ARMOR EFFECT" << endl;
}

void BR_TEMP3::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "BR_TEMP3 EFFECT" << endl;
}

void ViscousAcid::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

	//vector<CharacterStat> enemy_apply_stats;
	//enemy_apply_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::ANY, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetArmor(), -GetRandOnApplyMinMax() });
	//ApplyParams apply_params;
	//apply_params._on_apply = ECombatEvent::ON_APPLY;
	//apply_params._apply_params_struct = EEffectParamsStruct::EFFECT_STAT;
	//apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_stats), EStatValueAction::UPDATE_ACTUAL);

	//vector<CharacterStat> enemy_effect_stats;
	//enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::HEALTH, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetHealth(), -GetRandEffectMinMax() });
	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//effect_params._effect_params_struct = EEffectParamsStruct::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat({}, move(enemy_effect_stats), EStatValueAction::UPDATE_ACTUAL);

	//vector<Character*> targets = { team2[t2_idx[0]] };

	//shared_ptr<FireballEffect> effect = make_shared<FireballEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect, _idx);
}

stringstream& ViscousAcid::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Spill acid on the target reducing their armor by " << CV << _spell->GetOnApplyMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << CI << ".\n";
	//	_tooltip << "Additionaly, the target will suffer " << CV << _spell->GetEffectMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetEffectMax(_idx, _spell->GetLevel()) << CI << " poison damage at the start of their turn.\n";
	//	_tooltip << "Lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}

void VA_TEMP1::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "BR_TEMP1 EFFECT" << endl;
}

void VA_TEMP2::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "BR_TEMP2 ARMOR EFFECT" << endl;
}

void VA_TEMP3::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {
	cout << "BR_TEMP3 EFFECT" << endl;
}






//============================================================================== SUMMON =============================================================================================
//==================================================================================================================================================================================


void SummonFireElemental::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

}




//============================================================================== MELEE =============================================================================================
//==================================================================================================================================================================================

void Melee::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

	vector<CharacterStat> enemy_apply_stats;
	auto stat = &targets[0].lock()->GetHealth().GetActual();
	auto delta = [&](Character* character) { return static_cast<float>(-GameplayStatics::GetRandInt(character->_min_damage, character->_max_damage)); };
	enemy_apply_stats.push_back(CharacterStat{ targets[0].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, stat, delta });
	ApplyParams apply_params;
	apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	apply_params._effect_stat = Effect_Stat{ {}, enemy_apply_stats };

	unique_ptr<Melee> spell = make_unique<Melee>();
	GameplayStatics::ApplyEffect(instigator, targets, move(spell), apply_params, {});
}





//============================================================================== RANGED ============================================================================================
//==================================================================================================================================================================================

void Ranged::Apply(Character* instigator, vector<weak_ptr<Character>>& targets) {

}