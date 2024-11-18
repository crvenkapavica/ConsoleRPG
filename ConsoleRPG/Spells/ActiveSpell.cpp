#include "../Spells/ActiveSpell.h"
#include "../GameplayStatics.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/SummonCharacter.h"
#include "../Combat/CombatManager.h"
#include "../MapGenerator/MapGenerator.h"
#include "../Spells/EffectStructs.h"
#include "../Spells/SpellData.h"

ActiveSpell::ActiveSpell(const ESpellID SpellID, const int InLevel)
	: Spell(SpellID, ESpellActivity::ACTIVE, SpellDb::ActiveConstMap.at(SpellID).Rarity, SpellDb::ActiveConstMap.at(SpellID).Class, SpellDb::ActiveConstMap.at(SpellID).RequiredLevel, InLevel)
	, DamageType(SpellDb::ActiveConstMap.at(SpellID).DamageType)
	, SpellType(SpellDb::ActiveConstMap.at(SpellID).SpellType)
{}

std::unique_ptr<ActiveSpell> ActiveSpell::CreateActiveSpell(const ESpellID SpellID) {
	switch (SpellID) {
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
		throw std::invalid_argument("SpellID not found.");
	}
}

float ActiveSpell::GetRandEffectMinMax(const std::shared_ptr<Character>& InCharacter) {
	return AdjustDamage(GameplayStatics::GetRandFloat(SpellDb::Data[ID][Level].EffectMin, SpellDb::Data[ID][Level].EffectMax), InCharacter);
}

float ActiveSpell::GetRandOnApplyMinMax(const std::shared_ptr<Character>& InCharacter) {
	return AdjustDamage(GameplayStatics::GetRandFloat(SpellDb::Data[ID][Level].ApplyMin, SpellDb::Data[ID][Level].ApplyMax), InCharacter);
}

float ActiveSpell::AdjustDamage(float Damage, const std::shared_ptr<Character>& InCharacter) {
	switch (DamageType) {
	case EDamageType::ARCANE:
		Damage += Damage * InCharacter->ArcaneDamage;
		break;
	case EDamageType::FIRE:
		Damage += Damage * InCharacter->FireDamage;
		break;
	case EDamageType::LIGHTNING:
		Damage += Damage * InCharacter->LightningDamage;
		break;
	case EDamageType::COLD:
		Damage += Damage * InCharacter->ColdDamage;
		break;
	case EDamageType::POISON:
		Damage += Damage * InCharacter->PoisonDamage;
		break;
	case EDamageType::NECROTIC:
		Damage += Damage * InCharacter->NecroticDamage;
		break;
	case EDamageType::PHYSICAL:
		Damage += Damage * InCharacter->PhysicalDamage;
		Damage += InCharacter->GetAP().GetActual();
	case EDamageType::HEALING:
		Damage += Damage * InCharacter->Healing;
		break;
	default:
		break;
	}

	if (DamageType != EDamageType::PHYSICAL && DamageType != EDamageType::NONE)
		Damage += InCharacter->GetSP().GetActual();

	// Critical Strike
	const int Rnd = GameplayStatics::GetRandInt(0, 100000);
	if (GetClass() == ESpellClass::MAGIC) {
		if (const float Chance = InCharacter->GetSpellCritChance().GetActual() * 100000; Rnd <= Chance)
			Damage *= InCharacter->GetSpellCritDmg().GetActual() / 100;
	}
	else if (GetClass() == ESpellClass::MELEE || GetClass() == ESpellClass::RANGED) {
		if (const float Chance = InCharacter->GetCritChance().GetActual() * 100000; Rnd <= Chance)
			Damage *= InCharacter->GetCritDmg().GetActual();
	}

	return Damage;
}

int ActiveSpell::AddRandomTargets(int nTargets, std::vector<std::weak_ptr<Character>>& Targets, const std::shared_ptr<Character>& Instigator, const std::string& Name) {
	std::vector<std::weak_ptr<Character>> Enemies;
	if (Instigator->GetTeam() == 1) Enemies = GameplayStatics::GetEnemyCharacters();
	else Enemies = GameplayStatics::GetPlayerCharacters();

	const int expired = static_cast<int>(std::ranges::count_if(
		Enemies, [](const auto& WPtrChar) { return WPtrChar.expired(); }
	));
	const int size = static_cast<int>(Enemies.size()) - expired;
	if (size == 1) return 0;
	nTargets = size == nTargets ? nTargets - 1 : nTargets;
	 
	for (int i = 0; i < nTargets; i++) {
		int Random;
		do Random = GameplayStatics::GetRandInt(0, static_cast<int>(Enemies.size() - 1));
		while (std::ranges::any_of(
			Targets, [&](const auto& WPtrChar) {
				return Enemies[Random].expired() || Enemies[Random].lock().get() == WPtrChar.lock().get();
			}
		));
		Targets.push_back(Enemies[Random]);
	}
	
	std::ranges::sort(
		Targets, [&](const auto& CharacterA, const auto& CharacterB) {
			return (!CharacterA.lock() || !CharacterB.lock())
			? false
			: CharacterA.lock()->GetAlias() < CharacterB.lock()->GetAlias();
		}
    );

	// if (CharacterA.lock() && CharacterB.lock();
	// CharacterA.lock()->GetAlias() < CharacterB.lock()->GetAlias()
	// ) return true;
	// return false;

	auto& Stream = GameplayStatics::GetCombatLogStream();
	static std::string C = GameplayStatics::GetAliasColor(Targets[0].lock()->GetAlias());
	Stream << "Characters: " << C << Targets[0].lock()->GetAlias() << CL << ", " << C;
	for (int i = 0; i < nTargets; i++) {
		Stream << Targets[i + 1].lock()->GetAlias();  //warning ?
		if (i != nTargets - 1) Stream << CL << ", " << C;
	}
	Stream << CL << " got hit by " << CEF << Name << CL << ".\n";
	return nTargets;
}

bool ActiveSpell::Summon(const ECharacterClass CharacterClass, const std::shared_ptr<Character>& Instigator) {
	auto Deleter = [](const SummonCharacter* Ptr) { Ptr->GetTeam() == 1 ? SummonCharacter::nPlayerSummons-- : SummonCharacter::nEnemySummons--; delete Ptr; };

	// replace with direct map_gen call after making map_GEN singleton
	if (const std::shared_ptr<SummonCharacter> Summon(new SummonCharacter(CharacterClass, Instigator->GetTeam()), Deleter);
		MapGenerator::GetInstance().AddCharacterToCharGrid(Instigator, Summon)
		) { CombatManager::AddSummonToCombat(Summon); return true; }
	
	auto& s = GameplayStatics::GetCombatLogStream();
	s << COLOR_ERROR << "Summon failed. No space." << ANSI_COLOR_RESET << "\n";
	return false;
}

void Fireball::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	std::vector<CharacterStat> EnemyApplyStats;
	const int RandomTargets = AddRandomTargets(2, Targets, Instigator, "FIREBALL SPREAD");
	
	for (int i = 0; i <= RandomTargets; ++i) {
		const auto Stat = &Targets[i].lock()->GetHealth().GetActual();
		auto Delta = [&](const std::shared_ptr<Character>& SPtrChar) { return -GetRandOnApplyMinMax(SPtrChar); };
		EnemyApplyStats.emplace_back(CharacterStat{
			.PtrCharacter = Targets[i].lock().get(),
			.StatType = EStatType::HEALTH,
			.StatMod = EStatMod::CONSTANT,
			.Stat = Stat,
			.GetDelta = Delta
		});
	}
	
	ApplyParams AppParams;
	AppParams.Flags |= EStructFlags::EFFECT_STAT;
	AppParams.EffectStat = EffectStat({}, std::move(EnemyApplyStats));

	auto Spell = std::make_unique<Fireball>();
	GameplayStatics::ApplyEffect(Instigator, Targets, std::move(Spell), AppParams, {});
}

std::stringstream& Fireball::GetTooltip() {
	if (Tooltip.str().empty()) {
		Tooltip << COLOR_INFO << "Hits the target for " << COLOR_VALUE << SpellDb::Data[ID][Level].ApplyMin << COLOR_INFO << " to " << COLOR_VALUE << SpellDb::Data[ID][Level].ApplyMax << COLOR_INFO << " damage.\n";
	}
	return Tooltip;
}

void Burning::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	std::vector<CharacterStat> EnemyEffectStats;
	const int RandomTargets = AddRandomTargets(2, Targets, Instigator, "BURNING SPREAD");
	for (int i = 0; i <= RandomTargets; i++) {
		EnemyEffectStats.emplace_back(CharacterStat{
			.PtrCharacter = Targets[i].lock().get(),
			.StatType = EStatType::HEALTH,
			.StatMod = EStatMod::CONSTANT,
			.Stat = &Targets[i].lock()->GetHealth().GetActual(),
			.GetDelta = [this](const std::shared_ptr<Character>& SPtrChar) { return -GetRandEffectMinMax(SPtrChar); }
		});
	}

	EffectParams EnemyEffectParams;
	EnemyEffectParams.OnEvent = ECombatEvent::ON_TURN_BEGIN;
	EnemyEffectParams.Flags |= EStructFlags::EFFECT_STAT;
	EnemyEffectParams.EffectStat = EffectStat({}, std::move(EnemyEffectStats));

	auto Spell = std::make_unique<Burning>();
	GameplayStatics::ApplyEffect(Instigator, Targets, std::move(Spell), {}, EnemyEffectParams);
}

std::stringstream& Burning::GetTooltip() {
	if (Tooltip.str().empty()) {
		Tooltip << "Applies burn to the target and 2 random Targets that deals " << CV << SpellDb::Data[ID][Level].EffectMin << COLOR_INFO << " to " << CV << SpellDb::Data[ID][Level].EffectMax << COLOR_INFO << " damage.\n";
		Tooltip << "The burn effect lasts for " << CV << SpellDb::Data[ID][Level].Duration;
	}
	return Tooltip;
}

//============================================================================== MAGIC =============================================================================================
//==================================================================================================================================================================================

void MoltenArmor::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	std::vector<CharacterStat> EnemyApplyStats;
	const int RandomTargets = AddRandomTargets(2, Targets, Instigator, "MOLTEN ARMOR SPREAD");
	
	for (int i = 0; i <= RandomTargets; i++) {
		const auto ConstDelta = -GetRandOnApplyMinMax(Instigator);
		auto Delta = [=](const auto& SPtrChar) { return ConstDelta; };
		EnemyApplyStats.emplace_back(CharacterStat{
			.PtrCharacter = Targets[i].lock().get(),
			.StatType = EStatType::ANY,
			.StatMod = EStatMod::CONSTANT,
			.Stat = &Targets[i].lock()->GetArmor().GetActual(),
			.GetDelta =[&](const auto& SPtrChar) { return -GetRandOnApplyMinMax(Instigator); }
		});
	}
	
	ApplyParams AppParams;
	AppParams.Flags |= EStructFlags::EFFECT_STAT;
	AppParams.EffectStat = EffectStat({}, std::move(EnemyApplyStats));

	std::vector<CharacterStat> EnemyEffectStats;
	for (int i = 0; i <= RandomTargets; i++) {
		const auto Stat = &Targets[i].lock()->GetArmor().GetActual();
		auto Delta = [&](const std::shared_ptr<Character>& SPtrChar) { return -GetRandEffectMinMax(SPtrChar); };
		EnemyEffectStats.emplace_back(CharacterStat{
			.PtrCharacter = Targets[i].lock().get(),
			.StatType = EStatType::ANY,
			.StatMod = EStatMod::ADDITIVE,
			.Stat = Stat,
			.GetDelta = Delta
		});
	}
	
	EffectParams EffParams;
	EffParams.OnEvent = ECombatEvent::ON_TURN_BEGIN;
	EffParams.Flags |= EStructFlags::EFFECT_STAT;
	EffParams.EffectStat = EffectStat({}, std::move(EnemyEffectStats));

	auto Spell = std::make_unique<MoltenArmor>();
	GameplayStatics::ApplyEffect(Instigator, Targets, std::move(Spell), AppParams, EffParams);
}

std::stringstream& MoltenArmor::GetTooltip() {
	if (Tooltip.str().empty()) {
		Tooltip << "Blast the enemy with extreme temperature which melts \n";
		Tooltip << "the Targets armor, decreasing it by " << CV <<  SpellDb::Data[ID][Level].ApplyMin << CI << " to " << CV <<  SpellDb::Data[ID][Level].ApplyMax << CI << ".\n";
		Tooltip << "Additionally, decreases the armor by " CV <<  SpellDb::Data[ID][Level].EffectMin << CI << " to " << CV <<  SpellDb::Data[ID][Level].EffectMax << CI << " at the end of their turn. \n";
		Tooltip << "Additionally, hits 2 random enemies and applies the effect to them! \n";
		Tooltip << "The effect lasts for " << CV << SpellDb::Data[ID][Level].Duration << CI << " turns.\n";
	}
	return Tooltip;
}

void Exposure::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {
	std::vector<CharacterStat> EnemyApplyStats;
	const auto Stat = &Targets[0].lock()->GetResistances().GetFireRes();
	auto Delta = [&](const std::shared_ptr<Character>& SPtrChar) { return -GetRandOnApplyMinMax(SPtrChar); };
	EnemyApplyStats.emplace_back(CharacterStat{
		.PtrCharacter = Targets[0].lock().get(),
		.StatType = EStatType::RESISTANCE,
		.StatMod = EStatMod::CONSTANT,
		.Stat = Stat,
		.GetDelta = Delta
	});
	
	ApplyParams AppParams;
	AppParams.Flags |= EStructFlags::EFFECT_STAT;
	AppParams.EffectStat = EffectStat({}, std::move(EnemyApplyStats));

	auto Spell = std::make_unique<Exposure>();
	GameplayStatics::ApplyEffect(Instigator, Targets, std::move(Spell), AppParams, {});
}

std::stringstream& Exposure::GetTooltip() {
	if (Tooltip.str().empty()) {
		Tooltip << "Cast a potent blaze of fire at the target, exposing them, \n";
		Tooltip << "decreasing their resistance to fire and burn spells by " << CV << SpellDb::Data[ID][Level].EffectMin << CI << " to " << CV << SpellDb::Data[ID][Level].EffectMax << CI ".\n";
		Tooltip << "The effect lasts for " << CV << SpellDb::Data[ID][Level].Duration << CI << " turns.\n";
	}
	return Tooltip;
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
	return Tooltip;
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
	return Tooltip;
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

	// TODO: Check for weapon equip, make disarm mechanics
	
	std::vector<CharacterStat> EnemyApplyStats;
	EnemyApplyStats.push_back(CharacterStat{
		.PtrCharacter = Targets[0].lock().get(),
		.StatType = EStatType::HEALTH,
		.StatMod = EStatMod::CONSTANT,
		.Stat = &Targets[0].lock()->GetHealth().GetActual(),
		.GetDelta = [&](const auto& SPtrChar) { return static_cast<float>(-GameplayStatics::GetRandInt(SPtrChar->MinDamage, SPtrChar->MaxDamage)); }
	});

	ApplyParams AppParams;
	AppParams.Flags |= EStructFlags::EFFECT_STAT;
	AppParams.EffectStat = EffectStat{ {}, std::move(EnemyApplyStats) };

	auto Spell = std::make_unique<Melee>();
	GameplayStatics::ApplyEffect(Instigator, Targets, std::move(Spell), AppParams, {});
}





//============================================================================== RANGED ============================================================================================
//==================================================================================================================================================================================

void Ranged::Apply(const std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets) {

}