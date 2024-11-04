#include "../Spells/ActiveSpell.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/EnemyCharacter.h"
#include "../Characters/SummonCharacter.h"
#include "../Spells/SpellData.h"
#include "../GameplayStatics.h"
#include "../Spells/EffectStructs.h"
#include "../Combat/CombatManager.h"

ActiveSpell::ActiveSpell(ESpellID id, int lvl)
	: Spell(id, ESpellActivity::ACTIVE, SpellDb::ActiveConstMap.at(id).Rarity, SpellDb::ActiveConstMap.at(id).Class, SpellDb::ActiveConstMap.at(id).MinReqLevel, lvl)
	, _damage_type(SpellDb::ActiveConstMap.at(id).DamageType)
	, _spell_type(SpellDb::ActiveConstMap.at(id).SpellType)
{}

std::unique_ptr<ActiveSpell> ActiveSpell::CreateActiveSpell(ESpellID id) {
	switch (id) {
		// MAGIC
	case ESpellID::FIREBALL:
		return std::make_unique<Fireball>();
	case ESpellID::BURNING:
		return std::make_unique<Burning>();
	case ESpellID::MOLTEN_ARMOR:
		return std::make_unique<MoltenArmor>();
	case ESpellID::EXPOSURE:
		return std::make_unique<Exposure>();
	case ESpellID::STONESKIN:
		return std::make_unique<Stoneskin>();
	case ESpellID::ARCANE_INFUSION:
		return std::make_unique<ArcaneInfusion>();
	case ESpellID::BLOOD_RAIN:
		return std::make_unique<BloodRain>();
	case ESpellID::VISCOUS_ACID:
		return std::make_unique<ViscousAcid>();

		// MISC
	case ESpellID::BLIND:
		return std::make_unique<Blind>();

		// SUMMON
	case ESpellID::SUM_FIRE_ELE:
		return std::make_unique<SummonFireElemental>();
	case ESpellID::SUM_FIRE_IMP:
		return std::make_unique<SummonFireImp>();

		// MELEE
	case ESpellID::MELEE:
		return std::make_unique<Melee>();

		// RANGED
	case ESpellID::RANGED:
		return std::make_unique<Ranged>();

	default:
		throw new std::invalid_argument("SpellID not found.");
	}
}

float ActiveSpell::GetRandEffectMinMax(const std::shared_ptr<Character>& character) {
	return AdjustDamage(GameplayStatics::GetRandFloat(SpellDb::Data[_id][_lvl].EffectMin, SpellDb::Data[_id][_lvl].EffectMax), character);
}

float ActiveSpell::GetRandOnApplyMinMax(const std::shared_ptr<Character>& character) {
	return AdjustDamage(GameplayStatics::GetRandFloat(SpellDb::Data[_id][_lvl].ApplyMin, SpellDb::Data[_id][_lvl].ApplyMax), character);
}

float ActiveSpell::AdjustDamage(float Damage, const std::shared_ptr<Character>& character) {
	switch (_damage_type) {
	case EDamageType::ARCANE:
		Damage += Damage * character->ArcaneDamage;
		break;
	case EDamageType::FIRE:
		Damage += Damage * character->FireDamage;
		break;
	case EDamageType::LIGHTNING:
		Damage += Damage * character->LightningDamage;
		break;
	case EDamageType::COLD:
		Damage += Damage * character->ColdDamage;
		break;
	case EDamageType::POISON:
		Damage += Damage * character->PoisonDamage;
		break;
	case EDamageType::NECROTIC:
		Damage += Damage * character->NecroticDamage;
		break;
	case EDamageType::PHYSICAL:
		Damage += Damage * character->PhysicalDamage;
		Damage += character->GetAP().GetActual();
	case EDamageType::HEALING:
		Damage += Damage * character->Healing;
		break;
	default:
		break;
	}

	if (_damage_type != EDamageType::PHYSICAL && _damage_type != EDamageType::NONE)
		Damage += character->GetSP().GetActual();

	// Critical Strike
	const int Rnd = GameplayStatics::GetRandInt(0, 100000);
	if (GetClass() == ESpellClass::MAGIC) {
		if (const float Chance = character->GetSpellCritChance().GetActual() * 100000; Rnd <= Chance)
			Damage *= character->GetSpellCritDmg().GetActual() / 100;
	}
	else if (GetClass() == ESpellClass::MELEE || GetClass() == ESpellClass::RANGED) {
		if (const float Chance = character->GetCritChance().GetActual() * 100000; Rnd <= Chance)
			Damage *= character->GetCritDmg().GetActual();
	}

	return Damage;
}

int ActiveSpell::AddRandomTargets(int r, std::vector<std::weak_ptr<Character>>& Targets, const std::shared_ptr<Character>& character, const std::string& name) {
	std::vector<std::weak_ptr<Character>> Enemies;
	if (character->GetTeam() == 1)
		Enemies = GameplayStatics::GetEnemyCharacters();
	else
		Enemies = GameplayStatics::GetPlayerCharacters();

	const int expired = static_cast<int>(std::ranges::count_if(Enemies, [](const std::weak_ptr<Character>& WPtr) { return WPtr.expired(); }));
	const int size = static_cast<int>(Enemies.size()) - expired;
	if (size == 1) return 0;
	r = size == r ? r - 1 : r;
	 
	for (int i = 0; i < r; i++) {
		int Rnd;
		do {
			Rnd = rand() % Enemies.size();
		} while (std::ranges::any_of(Targets, [&](const std::weak_ptr<Character>& WPtr) { return Enemies[Rnd].expired() || Enemies[Rnd].lock().get() == WPtr.lock().get(); }));
		Targets.push_back(Enemies[Rnd]);
	}
	
	std::ranges::sort(Targets,[&](const std::weak_ptr<Character>& CharacterA, const std::weak_ptr<Character>& CharacterB) { 
         if (CharacterA.lock() && CharacterB.lock())
             if (CharacterA.lock()->GetAlias() < CharacterB.lock()->GetAlias()) return true;
         return false;
    });

	auto& s = GameplayStatics::GetCombatLogStream();
	static std::string C = GameplayStatics::GetAliasColor(Targets[0].lock()->GetAlias());
	s << "Characters: " << C << Targets[0].lock()->GetAlias() << COLOR_COMBAT_LOG << ", " << C;
	for (int i = 0; i < r; i++) {
		s << Targets[i + 1].lock()->GetAlias(); // POGLEDATI KAJ JE TAJ WARNING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (i != r - 1) s << COLOR_COMBAT_LOG << ", " << C;
	}
	s << COLOR_COMBAT_LOG << " got hit by " << COLOR_EFFECT << name << COLOR_COMBAT_LOG << ".\n";
	return r;
}

bool ActiveSpell::Summon(ECharacterClass character_class, const std::shared_ptr<Character>& Instigator) {
	auto dltr = [](const SummonCharacter* ptr) { ptr->GetTeam() == 1 ? SummonCharacter::nPlayerSummons-- : SummonCharacter::nEnemySummons--; delete ptr; };
	std::shared_ptr<SummonCharacter> summon(new SummonCharacter(character_class, Instigator->GetTeam()), dltr);

	if (GameplayStatics::AddCharacterToCharGrid(Instigator, summon)) { // replace with direct map_gen call after making map_GEN singleton
		CombatManager::AddSummonToCombat(summon);
		return true;
	}

	auto& s = GameplayStatics::GetCombatLogStream();
	s << COLOR_ERROR << "Summon failed. No space." << ANSI_COLOR_RESET << "\n";
	return false;
}

void Fireball::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	std::vector<CharacterStat> enemy_apply_stats;
	const auto stat = &Targets[0].lock()->GetHealth().GetActual();
	auto delta = [&](const std::shared_ptr<Character>& character) { return -GetRandOnApplyMinMax(character); };
	enemy_apply_stats.push_back(CharacterStat{ Targets[0].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, stat, delta});
	ApplyParams apply_params;
	apply_params.Flags |= EStructFlags::EFFECT_STAT;
	apply_params.EffectStat = EffectStat({}, std::move(enemy_apply_stats));

	std::unique_ptr<Fireball> spell = std::make_unique<Fireball>();
	GameplayStatics::ApplyEffect(Instigator, Targets, std::move(spell), apply_params, {});
}

std::stringstream& Fireball::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << COLOR_INFO << "Hits the target for " << COLOR_VALUE <<  << COLOR_INFO << " to " << COLOR_VALUE << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << COLOR_INFO << " damage.\n";
	//}
	return _tooltip;
}

void Burning::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

	int rand_targets = AddRandomTargets(2, Targets, Instigator, "BURNING");

	std::vector<CharacterStat> enemy_effect_stats;
	for (int i = 0; i <= rand_targets; i++) {
		auto stat = &Targets[i].lock()->GetHealth().GetActual();
		auto delta = [&](const std::shared_ptr<Character>& character) { return -GetRandEffectMinMax(character); };
		enemy_effect_stats.push_back(CharacterStat{ Targets[i].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, stat, delta });
	}

	EffectParams effect_params;
	effect_params.OnEvent = ECombatEvent::ON_TURN_BEGIN;
	effect_params.StructFlags |= EStructFlags::EFFECT_STAT;
	effect_params.EffectStat = EffectStat({}, move(enemy_effect_stats));

	std::unique_ptr<Burning> spell = std::make_unique<Burning>();
	GameplayStatics::ApplyEffect(Instigator, Targets, move(spell), {}, effect_params);
}

std::stringstream& Burning::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Applies burn to the target and 2 random Targets that deals " << COLOR_VALUE << _spell->GetEffectMin(_idx, _spell->GetLevel()) << COLOR_INFO << " to " << COLOR_VALUE << _spell->GetEffectMax(_idx, _spell->GetLevel()) << COLOR_INFO << " damage.\n";
	//	_tooltip << "The burn effect lasts for " << COLOR_VALUE << _spell->GetDuration(_idx, _spell->GetLevel()) << COLOR_INFO << " turn(s).";
	//}
	return _tooltip;
}

//============================================================================== MAGIC =============================================================================================
//==================================================================================================================================================================================

void MoltenArmor::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	
	int rand_targets = AddRandomTargets(2, Targets, Instigator, "MOLTEN ARMOR");

	std::vector<CharacterStat> enemy_apply_stats;
	for (int i = 0; i <= rand_targets; i++) {
		auto stat = &Targets[i].lock()->GetArmor().GetActual();
		auto const_delta = -GetRandOnApplyMinMax(Instigator);
		auto delta = [=](const std::shared_ptr<Character>& character) { return const_delta; };
		enemy_apply_stats.push_back(CharacterStat{
			.PtrCharacter = Targets[i].lock().get(),
			.StatType = EStatType::ANY,
			.StatMod = EStatMod::CONSTANT,
			.Stat = stat,.GetDelta = delta
		});
	}
	ApplyParams apply_params;
	apply_params.Flags |= EStructFlags::EFFECT_STAT;
	apply_params.EffectStat = EffectStat({}, std::move(enemy_apply_stats));

	std::vector<CharacterStat> enemy_effect_stats;
	for (int i = 0; i <= rand_targets; i++) {
		auto stat = &Targets[i].lock()->GetArmor().GetActual();
		auto delta = [&](const std::shared_ptr<Character>& character) { return -GetRandEffectMinMax(character); };
		enemy_effect_stats.push_back(CharacterStat{
			.PtrCharacter = Targets[i].lock().get(),
			.StatType = EStatType::ANY,
			.StatMod = EStatMod::ADDITIVE,
			.Stat = stat,.GetDelta = delta
		});
	}
	EffectParams effect_params;
	effect_params.OnEvent = ECombatEvent::ON_TURN_BEGIN;
	effect_params.StructFlags |= EStructFlags::EFFECT_STAT;
	effect_params.EffectStat = EffectStat({}, std::move(enemy_effect_stats));

	std::unique_ptr<MoltenArmor> spell = std::make_unique<MoltenArmor>();
	GameplayStatics::ApplyEffect(Instigator, Targets, std::move(spell), apply_params, effect_params);
}

std::stringstream& MoltenArmor::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Blast the enemy with extreme temperature which melts \n";
	//	_tooltip << "the Targets armor, decreasing it by " << CV << _spell->GetOnApplyMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << CI << ".\n";
	//	_tooltip << "Additionaly, decreases the armor by " CV << _spell->GetEffectMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetEffectMax(_idx, _spell->GetLevel()) << CI << " at the end of their turn. \n";
	//	_tooltip << "Additionaly, hits 2 random enemies and applies the effect to them! \n";
	//	_tooltip << "The effect lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}

void Exposure::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

	std::vector<CharacterStat> enemy_apply_res;
	auto stat = &Targets[0].lock()->GetResistances().GetFireRes();
	auto delta = [=](const std::shared_ptr<Character>& character) { return -GetRandOnApplyMinMax(character); };
	enemy_apply_res.push_back(CharacterStat{ Targets[0].lock().get(), EStatType::RESISTANCE, EStatMod::CONSTANT, stat, delta });
	ApplyParams apply_params;
	apply_params.Flags |= EStructFlags::EFFECT_STAT;
	apply_params.EffectStat = EffectStat({}, move(enemy_apply_res));

	std::unique_ptr<Exposure> spell = std::make_unique<Exposure>();
	GameplayStatics::ApplyEffect(Instigator, Targets, move(spell), apply_params, {});
}

std::stringstream& Exposure::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Cast a potent blaze of fire at the target, exposing them, \n";
	//	_tooltip << "decreasing their resistance to fire and burn spells by " << CV << _spell->GetEffectMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetEffectMax(_idx, _spell->GetLevel()) << CI ".\n";
	//	_tooltip << "The effect lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}

void Stoneskin::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

	//std::vector<CharacterStat> ally_apply_stats;
	//ally_apply_stats.push_back(CharacterStat{ team1[t1_idx[0]], EStatType::ANY, EStatMod::ADDITIVE, &team1[t1_idx[0]]->GetArmor(), GetRandOnApplyMinMax() });
	//ApplyParams apply_params;
	//apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//apply_params._effect_stat = Effect_Stat(move(ally_apply_stats), {}, EStatValueAction::UPDATE_ACTUAL);

	//EffectParams effect_params;

	//std::vector<Character*> Targets = { team1[t1_idx[0]] };

	//std::shared_ptr<StoneskinEffect> effect = std::make_shared<StoneskinEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(Instigator, Targets, effect_params, apply_params, effect, _idx);
}

std::stringstream& Stoneskin::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Hardens the Targets skin, increasing armor by " << CV << _spell->GetOnApplyMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << CI << " each turn.\n";
	//	_tooltip << "The effect lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}

void Disarm::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	std::cout << "SS_disarm" << '\n';
}

void Bloodbath::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	std::cout << "SS_bloodath" << '\n';
}

void ArcaneInfusion::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

	//std::vector<CharacterStat> ally_apply_stats;
	//ally_apply_stats.push_back(CharacterStat{ team1[t1_idx[0]], EStatType::ANY, EStatMod::CONSTANT, &team1[t1_idx[0]]->GetDmgMelee(), GetRandOnApplyMinMax() });
	//ApplyParams apply_params;
	//apply_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//apply_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//apply_params._effect_stat = Effect_Stat(move(ally_apply_stats), {}, EStatValueAction::UPDATE_ACTUAL);


	//std::vector<CharacterStat> ally_effect_stats;
	//ally_effect_stats.push_back(CharacterStat{ team1[t1_idx[0]], EStatType::ANY, EStatMod::ADDITIVE, &team1[t1_idx[0]]->GetDmgMelee(), GetRandEffectMinMax() });
	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;;
	//effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat(move(ally_effect_stats), {}, EStatValueAction::UPDATE_ACTUAL);

	//std::vector<Character*> Targets = { team1[t1_idx[0]] };

	//std::shared_ptr<ArcaneInfusionEffect> effect = std::make_shared<ArcaneInfusionEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(Instigator, Targets, effect_params, apply_params, effect, _idx);
}

void BloodRain::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

	//float drain = GetRandEffectMinMax();

	//ApplyParams apply_params;

	//std::vector<CharacterStat> ally_effect_stats;
	//ally_effect_stats.push_back(CharacterStat{ Instigator, EStatType::HEALTH, EStatMod::CONSTANT, &Instigator->GetHealth(), drain });
	//std::vector<CharacterStat> enemy_effect_stats;
	//enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::HEALTH, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetHealth(), -drain });
	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//effect_params._struct_flags |= EStructFlags::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat(move(ally_effect_stats), move(enemy_effect_stats), EStatValueAction::UPDATE_ACTUAL);

	//std::vector<Character*> Targets = { team2[t2_idx[0]] };

	//std::shared_ptr<BloodRainEffect> effect = std::make_shared<BloodRainEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(Instigator, Targets, effect_params, apply_params, effect, _idx);
}

void ViscousAcid::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

	//std::vector<CharacterStat> enemy_apply_stats;
	//enemy_apply_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::ANY, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetArmor(), -GetRandOnApplyMinMax() });
	//ApplyParams apply_params;
	//apply_params._on_apply = ECombatEvent::ON_APPLY;
	//apply_params._apply_params_struct = EEffectParamsStruct::EFFECT_STAT;
	//apply_params._effect_stat = Effect_Stat({}, move(enemy_apply_stats), EStatValueAction::UPDATE_ACTUAL);

	//std::vector<CharacterStat> enemy_effect_stats;
	//enemy_effect_stats.push_back(CharacterStat{ team2[t2_idx[0]], EStatType::HEALTH, EStatMod::CONSTANT, &team2[t2_idx[0]]->GetHealth(), -GetRandEffectMinMax() });
	//EffectParams effect_params;
	//effect_params._on_event = ECombatEvent::ON_TURN_BEGIN;
	//effect_params._effect_params_struct = EEffectParamsStruct::EFFECT_STAT;
	//effect_params._effect_stat = Effect_Stat({}, move(enemy_effect_stats), EStatValueAction::UPDATE_ACTUAL);

	//std::vector<Character*> Targets = { team2[t2_idx[0]] };

	//std::shared_ptr<FireballEffect> effect = std::make_shared<FireballEffect>(_ID, _spell, _damage_type, _spell_type, _idx);
	//GameplayStatics::ApplyEffect(Instigator, Targets, effect_params, apply_params, effect, _idx);
}

std::stringstream& ViscousAcid::GetTooltip() {
	//if (_tooltip.str().empty()) {
	//	_tooltip << "Spill acid on the target reducing their armor by " << CV << _spell->GetOnApplyMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetOnApplyMax(_idx, _spell->GetLevel()) << CI << ".\n";
	//	_tooltip << "Additionaly, the target will suffer " << CV << _spell->GetEffectMin(_idx, _spell->GetLevel()) << CI << " to " << CV << _spell->GetEffectMax(_idx, _spell->GetLevel()) << CI << " poison damage at the start of their turn.\n";
	//	_tooltip << "Lasts for " << CV << _spell->GetDuration(_idx, _spell->GetLevel()) << CI << " turns.\n";
	//}
	return _tooltip;
}





//==============================================================================  MISC  ============================================================================================
//==================================================================================================================================================================================

void Blind::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	
	std::unique_ptr<Blind> spell = std::make_unique<Blind>();
	GameplayStatics::ApplyEffect(Instigator, Targets, move(spell), {}, {});
}



//============================================================================== SUMMON =============================================================================================
//===================================================================================================================================================================================

void SummonFireElemental::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	bool bhasSummoned = Summon(ECharacterClass::FIRE_ELEMENTAL, Instigator);
}

void SummonFireImp::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	bool bHasSummoned = Summon(ECharacterClass::FIRE_IMP, Instigator);
}


//============================================================================== MELEE =============================================================================================
//==================================================================================================================================================================================

void Melee::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

	std::vector<CharacterStat> enemy_apply_stats;
	auto stat = &Targets[0].lock()->GetHealth().GetActual();
	auto delta = [&](const std::shared_ptr<Character>& character) { return static_cast<float>(-GameplayStatics::GetRandInt(character->MinDamage, character->MaxDamage)); };
	enemy_apply_stats.push_back(CharacterStat{ Targets[0].lock().get(), EStatType::HEALTH, EStatMod::CONSTANT, stat, delta });
	ApplyParams apply_params;
	apply_params.Flags |= EStructFlags::EFFECT_STAT;
	apply_params.EffectStat = EffectStat{ {}, std::move(enemy_apply_stats) };

	std::unique_ptr<Melee> spell = std::make_unique<Melee>();
	GameplayStatics::ApplyEffect(Instigator, Targets, std::move(spell), apply_params, {});
}





//============================================================================== RANGED ============================================================================================
//==================================================================================================================================================================================

void Ranged::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

}