#include "../Effects/ActiveSpell.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Spells/SpellBook.h"
#include "../GameplayStatics.h"
#include "../Effects/EffectStructs.h"

std::unordered_map<ESpellID, pair <EDamageType, ESpellType>> ActiveSpell::_spell_map;

ActiveSpell::SpellMapConstructor::SpellMapConstructor() {
	_spell_map[ESpellID::FIREBALL] = make_pair(EDamageType::FIRE, ESpellType::PROJECTILE);
	_spell_map[ESpellID::BURNING] = make_pair(EDamageType::FIRE, ESpellType::DEBUFF);
	_spell_map[ESpellID::MOLTEN_ARMOR] = make_pair(EDamageType::FIRE, ESpellType::DEBUFF);
	_spell_map[ESpellID::EXPOSURE] = make_pair(EDamageType::FIRE, ESpellType::DEBUFF);
	_spell_map[ESpellID::STONESKIN] = make_pair(EDamageType::NONE, ESpellType::BUFF);
	_spell_map[ESpellID::DISARM] = make_pair(EDamageType::NONE, ESpellType::DEBUFF);
	_spell_map[ESpellID::THRONS] = make_pair(EDamageType::PHYSICAL, ESpellType::AURA);
	_spell_map[ESpellID::BLOODBATH] = make_pair(EDamageType::FIRE, ESpellType::PROJECTILE);
	_spell_map[ESpellID::ARCANE_INFUSION] = make_pair(EDamageType::ARCANE, ESpellType::BUFF);
	_spell_map[ESpellID::AI_TEMP1] = make_pair(EDamageType::ARCANE, ESpellType::BUFF);
	_spell_map[ESpellID::AI_TEMP2] = make_pair(EDamageType::ARCANE, ESpellType::BUFF);
	_spell_map[ESpellID::AI_TEMP3] = make_pair(EDamageType::ARCANE, ESpellType::BUFF);
	_spell_map[ESpellID::BLOOD_RAIN] = make_pair(EDamageType::NECROTIC, ESpellType::DEBUFF);
	_spell_map[ESpellID::BR_TEMP1] = make_pair(EDamageType::NECROTIC, ESpellType::DEBUFF);
	_spell_map[ESpellID::BR_TEMP2] = make_pair(EDamageType::NECROTIC, ESpellType::DEBUFF);
	_spell_map[ESpellID::BR_TEMP3] = make_pair(EDamageType::NECROTIC, ESpellType::DEBUFF);
	_spell_map[ESpellID::VISCOUS_ACID] = make_pair(EDamageType::POISON, ESpellType::DEBUFF);
	_spell_map[ESpellID::VA_TEMP1] = make_pair(EDamageType::POISON, ESpellType::DEBUFF);
	_spell_map[ESpellID::VA_TEMP2] = make_pair(EDamageType::POISON, ESpellType::DEBUFF);
	_spell_map[ESpellID::VA_TEMP3] = make_pair(EDamageType::POISON, ESpellType::DEBUFF);
}
ActiveSpell::SpellMapConstructor ActiveSpell::_spell_map_constructor;

ActiveSpell::ActiveSpell(ESpellID id, int level)
	: Spell(id)
	, _damage_type(_spell_map.at(id).first)
	, _spell_type(_spell_map.at(id).second)
{}

unique_ptr<ActiveSpell> ActiveSpell::CreateSpellBook(ESpellID id) {
	switch (id) {
	case ESpellID::FIREBALL:
		return make_unique<Fireball>(id);
		break;
	}
}

float ActiveSpell::GetRandEffectMinMax(Character* character) {
	//return AdjustDamage(GameplayStatics::GetRandFloat(_spell->GetEffectMin(_idx, _spell->GetLevel()), _spell->GetEffectMax(_idx, _spell->GetLevel())), character);
	return 0.f;
}

float ActiveSpell::GetRandOnApplyMinMax(Character* character) {
	//return AdjustDamage(GameplayStatics::GetRandFloat(_spell->GetOnApplyMin(_idx, _spell->GetLevel()), _spell->GetOnApplyMax(_idx, _spell->GetLevel())), character);
	return 0.f;
}

float ActiveSpell::AdjustDamage(float damage, Character* character) {
	auto damage_type = EDamageType::FIRE;
	auto spell_type = ESpellType::AURA;
	
	switch (damage_type) {
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

	if (damage_type != EDamageType::PHYSICAL)
		damage += character->GetSP().GetActual();

	int rnd = GameplayStatics::GetRandInt(0, 100000);
	if (spell_type == ESpellType::PROJECTILE) {
		float chance = character->GetSpellCritChance().GetActual() * 100000;
		if (rnd <= chance)
			damage *= character->GetSpellCritDmg().GetActual();
	}
	else if (spell_type == ESpellType::MELEE || spell_type == ESpellType::RANGED) {
		float chance = character->GetCritChance().GetActual() * 100000; 
		if (rnd <= chance)
			damage *= character->GetCritDmg().GetActual();
	}

	return damage;
}

int ActiveSpell::AddRandomTargets(int r, const vector<weak_ptr<Character>>& enemies, vector<int>& index, const string& name) {

	//int expired = count_if(enemies.begin(), enemies.end(), [](const weak_ptr<Character>& wptr) { return wptr.expired(); });
	//int size = static_cast<int>(enemies.size()) - expired;
	//if (size == 1) return 0;
	//r = size == r ? r - 1 : r;
	//
	//for (int i = 0; i < r; i++) {
	//	int rnd;
	//	do {
	//		rnd = rand() % enemies.size();
	//	} while (any_of(index.begin(), index.end(), [&](const int idx) { return enemies[rnd].expired() || enemies[rnd].lock().get() == enemies[idx].lock().get(); }));
	//	index.push_back(GameplayStatics::GetEnemyIdx2(enemies[rnd].lock()->GetAlias())); // promeniti bez 2
	//}
	//sort(index.begin(), index.end());

	//auto& s = GameplayStatics::GetCombatLogStream();
	//static string C = GameplayStatics::GetAliasColor(enemies[index[0]].lock()->GetAlias());
	//s << "Characters: " << C << enemies[index[0]].lock()->GetAlias() << COLOR_COMBAT_LOG << ", " << C;
	//for (int i = 0; i < r; i++) {
	//	s << enemies[index[i + 1]].lock()->GetAlias();
	//	if (i != r - 1) s << COLOR_COMBAT_LOG << ", " << C;
	//}
	//s << COLOR_COMBAT_LOG << " got hit by " << COLOR_EFFECT << name << COLOR_COMBAT_LOG << ".\n";
	//return r;
	return 0;
}

void Fireball::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	//vector<CharacterStat> enemy_apply_stats;
	//float damage = -GetRandOnApplyMinMax(instigator);
	//enemy_apply_stats.push_back(CharacterStat{ team2[t2_idx[0]].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, &team2[t2_idx[0]].lock().get()->GetHealth(), damage});
	//OnApplyParams apply_params;
	//apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_stats), EStatValueAction::UPDATE_ACTUAL);

	//EffectParams effect_params;

	//vector<weak_ptr<Character>> targets = { team2[t2_idx[0]] };

	//shared_ptr<FireballEffect> effect = make_shared<FireballEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect, _idx);
}

stringstream& Fireball::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << COLOR_INFO << "Hits the target for " << COLOR_VALUE << _spell->GetOnApplyMin(_idx, _spell->GetLevel()) << COLOR_INFO << " to " << COLOR_VALUE << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << COLOR_INFO << " damage.\n";
	//}
	return _tooltip;
}

void Burning::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	//OnApplyParams apply_params;

	//int rand_targets = AddRandomTargets(2, team2, t2_idx, "BURNING EFFECT");
	//vector<CharacterStat> enemy_effect_stats;
	//for (int i = 0; i <= rand_targets; i++)
	//	enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[i]].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, &team2[t2_idx[i]].lock()->GetHealth(), -GetRandEffectMinMax()});

	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat({}, move(enemy_effect_stats), EStatValueAction::UPDATE_ACTUAL);

	//vector<weak_ptr<Character>> targets;
	//for (int i = 0; i <= rand_targets; i++)
	//	targets.push_back(team2[t2_idx[i]]);

	//shared_ptr<BurningEffect> effect = make_shared<BurningEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect, _idx);
}

stringstream& Burning::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Applies burn to the target and 2 random targets that deals " << COLOR_VALUE << _spell->GetEffectMin(_idx, _spell->GetLevel()) << COLOR_INFO << " to " << COLOR_VALUE << _spell->GetEffectMax(_idx, _spell->GetLevel()) << COLOR_INFO << " damage.\n";
	//	_tooltip << "The burn effect lasts for " << COLOR_VALUE << _spell->GetDuration(_idx, _spell->GetLevel()) << COLOR_INFO << " turn(s).";
	//}
	return _tooltip;
}

void MoltenArmor::Apply(Character* instigator, const vector<weak_ptr<Character>>& team1, const vector<weak_ptr<Character>>& team2, vector<int>& t1_idx, vector<int>& t2_idx) {

	// just needs value re-adjusting
	//----------------------------------------
	
	//int rand_targets = AddRandomTargets(2, team2, t2_idx, "MOLTEN ARMOR EFFECT");

	//vector<CharacterStat> enemy_apply_stats;
	//enemy_apply_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::ANY, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetArmor(), -GetRandOnApplyMinMax() });

	//for (int i = 1; i <= rand_targets; i++)
	//	enemy_apply_stats.push_back(CharacterStat{ team2[t2_idx[i]], EStatType::ANY, EStatMod::CONSTANT, &team2[t2_idx[i]]->GetArmor(), -GetRandOnApplyMinMax() });

	//OnApplyParams apply_params;
	//apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_stats), EStatValueAction::UPDATE_ACTUAL);

	//vector<CharacterStat> enemy_effect_stats;
	//enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::ANY, EStatMod::ADDITIVE, &team2[t2_idx[0]]->GetArmor(), -GetRandEffectMinMax() });
	//for (int i = 1; i <= rand_targets; i++)
	//	enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[i]], EStatType::ANY, EStatMod::ADDITIVE, &team2[t2_idx[i]]->GetArmor(), -GetRandEffectMinMax() });
	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat({}, move(enemy_effect_stats), EStatValueAction::UPDATE_ACTUAL);

	//vector<Character*> targets = { team2[t2_idx[0]] };

	//for (int i = 0; i < rand_targets; i++)
	//	targets.push_back(team2[t2_idx[i + 1]]);

	//shared_ptr<MoltenArmorEffect> effect = make_shared<MoltenArmorEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect, _idx);
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

	//vector<CharacterRes> enemy_apply_res;
	//enemy_apply_res.push_back(CharacterRes{ team2[t2_idx[0]], EStatType::ANY, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetResistances().GetFireRes(), -GetRandOnApplyMinMax() });
	//OnApplyParams apply_params;
	//apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//apply_params._struct_flags |= EStructFlags::EFFECT_RES;
	//apply_params._effect_res = Effect_Res({}, move(enemy_apply_res));

	//EffectParams effect_params;

	//vector<Character*> targets = { team2[t2_idx[0]] };

	//shared_ptr<ExposureEffect> effect = make_shared<ExposureEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(instigator, targets, effect_params, apply_params, effect, _idx);
}

stringstream& Exposure::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Cast a potent blaze of fire at the target, exposing them, \n";
	//	_tooltip << "decreasing their resistance to fire and burn spells by " << CV << _spell->GetEffectMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetEffectMax(_idx, _spell->GetLevel()) << CI ".\n";
	//	_tooltip << "The effect lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}
//==========================================================================================================================================
//==========================================================================================================================================
//==========================================================================================================================================
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
//==========================================================================================================================================
//==========================================================================================================================================
//==========================================================================================================================================
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
//==========================================================================================================================================
//==========================================================================================================================================
//==========================================================================================================================================
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
//==========================================================================================================================================
//==========================================================================================================================================
//==========================================================================================================================================
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
//==========================================================================================================================================
//==========================================================================================================================================
//==========================================================================================================================================