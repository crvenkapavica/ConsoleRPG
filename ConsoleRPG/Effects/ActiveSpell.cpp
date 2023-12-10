#include "../Effects/ActiveSpell.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Spells/SpellBook.h"
#include "../Spells/SpellData.h"
#include "../GameplayStatics.h"
#include "../Effects/EffectStructs.h"

ActiveSpell::ActiveSpell(ESpellID id, int lvl)
	: Spell(id, ESpellActivity::ACTIVE, SpellDB::_active_const_map.at(id)._rarity, SpellDB::_active_const_map.at(id)._class, SpellDB::_active_const_map.at(id)._min_req_lvl, lvl)
	, _damage_type(SpellDB::_active_const_map.at(id)._damage_type)
	, _spell_type(SpellDB::_active_const_map.at(id)._spell_type)
{}

unique_ptr<ActiveSpell> ActiveSpell::CreateActiveSpell(ESpellID id) {
	switch (id) {
		// MAGIC
	case ESpellID::FIREBALL:
		return make_unique<Fireball>(id);
	case ESpellID::BURNING:
		return make_unique<Burning>(id);
	case ESpellID::MOLTEN_ARMOR:
		return make_unique<MoltenArmor>(id);
	case ESpellID::EXPOSURE:
		return make_unique<Exposure>(id);
	case ESpellID::STONESKIN:
		return make_unique<Stoneskin>(id);
	case ESpellID::ARCANE_INFUSION:
		return make_unique<ArcaneInfusion>(id);
	case ESpellID::BLOOD_RAIN:
		return make_unique<BloodRain>(id);
	case ESpellID::VISCOUS_ACID:
		return make_unique<ViscousAcid>(id);


		// MELEE
	case ESpellID::MELEE:
		return make_unique<Melee>(id);

		// RANGED
	case ESpellID::RANGED:
		return make_unique<Ranged>(id);

	default:
		return nullptr;
	}
}

void ActiveSpell::InvokeEffect(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	Apply(instigator, team1, team2, t1_idx, t2_idx);
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
		damage += damage * character->_necrotic_damage;
		damage += character->GetAP().GetActual();
	case EDamageType::HEALING:
		damage += damage * character->_healing;
		break;
	default:
		break;
	}

	if (_damage_type != EDamageType::PHYSICAL)
		damage += character->GetSP().GetActual();

	// Critical Strike
	int rnd = GameplayStatics::GetRandInt(0, 100000);
	if (_spell_type == ESpellType::PROJECTILE) {
		float chance = character->GetSpellCritChance().GetActual() * 100000;
		if (rnd <= chance)
			damage *= character->GetSpellCritDmg().GetActual();
	}
	else if (_spell_type == ESpellType::MELEE || _spell_type == ESpellType::RANGED) {
		float chance = character->GetCritChance().GetActual() * 100000; 
		if (rnd <= chance)
			damage *= character->GetCritDmg().GetActual();
	}

	return damage;
}

int ActiveSpell::AddRandomTargets(int r, const vector<weak_ptr<Character>>& enemies, vector<int>& index, const string& name) {

	int expired = static_cast<int>(count_if(enemies.begin(), enemies.end(), [](const weak_ptr<Character>& wptr) { return wptr.expired(); }));
	int size = static_cast<int>(enemies.size()) - expired;
	if (size == 1) return 0;
	r = size == r ? r - 1 : r;
	
	for (int i = 0; i < r; i++) {
		int rnd;
		do {
			rnd = rand() % enemies.size();
		} while (any_of(index.begin(), index.end(), [&](const int idx) { return enemies[rnd].expired() || enemies[rnd].lock().get() == enemies[idx].lock().get(); }));
		index.push_back(GameplayStatics::GetEnemyIdx(enemies[rnd].lock()->GetAlias()));
	}
	sort(index.begin(), index.end());

	auto& s = GameplayStatics::GetCombatLogStream();
	static string C = GameplayStatics::GetAliasColor(enemies[index[0]].lock()->GetAlias());
	s << "Characters: " << C << enemies[index[0]].lock()->GetAlias() << COLOR_COMBAT_LOG << ", " << C;
	for (int i = 0; i < r; i++) {
		s << enemies[index[i + 1]].lock()->GetAlias();
		if (i != r - 1) s << COLOR_COMBAT_LOG << ", " << C;
	}
	s << COLOR_COMBAT_LOG << " got hit by " << COLOR_EFFECT << name << COLOR_COMBAT_LOG << ".\n";
	return r;
}

void Fireball::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	vector<CharacterStat> enemy_apply_stats;
	auto stat = static_cast<Character::Stat*>(&team2[t2_idx[0]].lock().get()->GetHealth());
	auto delta = [&](Character* character) { return -GetRandOnApplyMinMax(character); };
	enemy_apply_stats.push_back(CharacterStat{ team2[t2_idx[0]].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, stat, delta });
	OnApplyParams apply_params;
	apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_stats), EStatValueAction::UPDATE_ACTUAL);

	EffectParams effect_params;

	vector<weak_ptr<Character>> targets = { team2[t2_idx[0]] };

	unique_ptr<Fireball> spell = make_unique<Fireball>(_ID);
	GameplayStatics::ApplyEffect(instigator, targets, move(spell), apply_params, effect_params);
}

stringstream& Fireball::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << COLOR_INFO << "Hits the target for " << COLOR_VALUE <<  << COLOR_INFO << " to " << COLOR_VALUE << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << COLOR_INFO << " damage.\n";
	//}
	return _tooltip;
}

void Burning::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	//OnApplyParams apply_params;

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

void MoltenArmor::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	
	int rand_targets = AddRandomTargets(2, team2, t2_idx, "MOLTEN ARMOR EFFECT");
	vector<CharacterStat> enemy_apply_stats;
	for (int i = 0; i <= rand_targets; i++) {
		auto stat = static_cast<Character::Stat*>(&team2[t2_idx[i]].lock()->GetArmor());
		auto delta = [&](Character* character) { return -GetRandOnApplyMinMax(character); };
		enemy_apply_stats.push_back(CharacterStat{ team2[t2_idx[i]].lock().get(), EStatType::ANY, EStatMod::CONSTANT, stat, delta});
	}
	OnApplyParams apply_params;
	apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_stats), EStatValueAction::UPDATE_ACTUAL);

	vector<CharacterStat> enemy_effect_stats;
	for (int i = 0; i <= rand_targets; i++) {
		auto stat = static_cast<Character::Stat*>(&team2[t2_idx[i]].lock()->GetArmor());
		auto delta = [&](Character* character) { return -GetRandEffectMinMax(character); };
		enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[i]].lock().get(), EStatType::ANY, EStatMod::ADDITIVE, stat, delta});
	}
	EffectParams effect_params;
	effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	effect_params._effect_stat = Effect_Stat({}, move(enemy_effect_stats), EStatValueAction::UPDATE_ACTUAL);

	vector<weak_ptr<Character>> targets;
	for (int i = 0; i <= rand_targets; i++)
		targets.push_back(team2[t2_idx[i]]);

	unique_ptr<MoltenArmor> spell = make_unique<MoltenArmor>(_ID);
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

void Exposure::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	vector<CharacterStat> enemy_apply_res;
	auto stat = static_cast<float*>(&team2[t2_idx[0]].lock()->GetResistances().GetFireRes());
	auto delta = [&](Character* character) { return -GetRandOnApplyMinMax(character); };
	enemy_apply_res.push_back(CharacterStat{ team2[t2_idx[0]].lock().get(), EStatType::ANY, EStatMod::CONSTANT, stat, delta });
	OnApplyParams apply_params;
	apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	apply_params._struct_flags |= EStructFlags::EFFECT_RES;
	apply_params._effect_res = Effect_Res({}, move(enemy_apply_res));

	EffectParams effect_params;

	vector<weak_ptr<Character>> targets = { team2[t2_idx[0]] };

	unique_ptr<Exposure> spell = make_unique<Exposure>(_ID);
	GameplayStatics::ApplyEffect(instigator, targets, move(spell), apply_params, effect_params);
}

stringstream& Exposure::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Cast a potent blaze of fire at the target, exposing them, \n";
	//	_tooltip << "decreasing their resistance to fire and burn spells by " << CV << _spell->GetEffectMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetEffectMax(_idx, _spell->GetLevel()) << CI ".\n";
	//	_tooltip << "The effect lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}

void Stoneskin::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	//vector<CharacterStat> ally_apply_stats;
	//ally_apply_stats.push_back(CharacterStat{ team1[t1_idx[0]], EStatType::ANY, EStatMod::ADDITIVE, &team1[t1_idx[0]]->GetArmor(), GetRandOnApplyMinMax() });
	//OnApplyParams apply_params;
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

void Disarm::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "SS_disarm" << endl;
}

void Thorns::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	// TO BE APPLIED WHEN WE HAVE ON DAMAGE RECEIVED EVENTS AND PHYSICAL ATTACKS

}

stringstream& Thorns::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Surounds the targets skin with deadly thron which return a portion of damage received back to the attacker.\n";
	//	_tooltip << "Only physical damage is returned. Returns \n";
	//}
	return _tooltip;
}

void Bloodbath::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "SS_bloodath" << endl;
}

void ArcaneInfusion::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	//vector<CharacterStat> ally_apply_stats;
	//ally_apply_stats.push_back(CharacterStat{ team1[t1_idx[0]], EStatType::ANY, EStatMod::CONSTANT, &team1[t1_idx[0]]->GetDmgMelee(), GetRandOnApplyMinMax() });
	//OnApplyParams apply_params;
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

void AI_TEMP1::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "AI_TEMP1" << endl;
}

void AI_TEMP2::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "AI_TEMP2" << endl;
}

void AI_TEMP3::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "AI_TEMP3" << endl;
}

void BloodRain::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	//float drain = GetRandEffectMinMax();

	//OnApplyParams apply_params;

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

void BR_TEMP1::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "BR_TEMP1 EFFECT" << endl;
}

void BR_TEMP2::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "BR_TEMP2 ARMOR EFFECT" << endl;
}

void BR_TEMP3::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "BR_TEMP3 EFFECT" << endl;
}

void ViscousAcid::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	//vector<CharacterStat> enemy_apply_stats;
	//enemy_apply_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::ANY, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetArmor(), -GetRandOnApplyMinMax() });
	//OnApplyParams apply_params;
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

void VA_TEMP1::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "BR_TEMP1 EFFECT" << endl;
}

void VA_TEMP2::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "BR_TEMP2 ARMOR EFFECT" << endl;
}

void VA_TEMP3::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	cout << "BR_TEMP3 EFFECT" << endl;
}





//============================================================================== MELEE =============================================================================================
//==================================================================================================================================================================================

void Melee::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {
	
}





//============================================================================== RANGED ============================================================================================
//==================================================================================================================================================================================

void Ranged::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

}