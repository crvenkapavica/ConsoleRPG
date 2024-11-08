#include "../Combat/CombatManager.h"
//#include "../Characters/Character.h"
#include "../Spells/EffectStructs.h"
#include "../Spells/PassiveSpell.h"

int CombatManager::nCycle = 0;
int CombatManager::nTurn = 0;
bool CombatManager::bNext = false;

// TODO : Rename Team1, Team2 - for randomized combat?
void CombatManager::SetTurns(std::vector<std::weak_ptr<Character>>&& Team1, std::vector<std::weak_ptr<Character>>&& Team2) {
	// In future adjust turn tables to be either randomized, or sorted based on a particular stat (level, power, etc..)
	PlayerCharacters = std::move(Team1);
	EnemyCharacters = std::move(Team2);

	for (const auto& Char : PlayerCharacters) {
		TurnTable.push_back(Char);
		Char.lock()->SetIsInCombat(true); // TODO: FIXME
	}

	for (const auto& Char : EnemyCharacters)
		TurnTable.push_back(Char); 

	OnCombatBegin();
	OnCycleBegin();
}

void CombatManager::StartCombat(const std::weak_ptr<Character>& Avatar) {
	PlayerAvatar = Avatar;
	
	// COMBAT LOOP
	while (PlayerAvatar.lock()->IsInCombat()) {
		KillFlaggedCharacters();
		if (PlayerAvatar.lock()->IsInCombat())
			BeginTurn(GetTurnCharacter());
		else break;
	}
}

void CombatManager::AddCombatEffect(const std::shared_ptr<CombatEffect>& Effect) {
	CombatEffects.emplace_back(nCycle + Effect->Duration, Effect);
	OnApplyEffect();
	std::ranges::sort(CombatEffects);
}

void CombatManager::BeginTurn(const std::weak_ptr<Character>& InCharacter) {
	if (!InCharacter.lock()->IsOnTurn()) {
		InCharacter.lock()->SetIsOnTurn(true);
		OnTurnBegin();
	}

	if (!InCharacter.expired()) {
		for (auto& effect : InCharacter.lock()->GetEffectIds())
			if (effect == ESpellID::BLIND)
				return EndTurn(*InCharacter.lock());

		InCharacter.lock()->TakeTurn();
	}
}

void CombatManager::EndTurn(Character& InCharacter) {
	OnTurnEnd();
	InCharacter.SetIsOnTurn(false);

	if (nTurn == static_cast<int>(TurnTable.size() - 1)) OnCycleEnd();
	nTurn = ++nTurn % static_cast<int>(TurnTable.size());

	if (nTurn == 0) {
		++nCycle;
		OnCycleBegin();
	}
}

void CombatManager::AddSummonToCombat(const std::shared_ptr<SummonCharacter>& Summon) {
	SummonCharactersBase.push_back(*Summon);
	SummonCharacters.push_back(Summon);
	TurnTable.insert(TurnTable.begin() + nTurn + 1, std::weak_ptr<SummonCharacter>(Summon));  // TODO: Check ref count (bcs of wptr); 
}

void CombatManager::DisplayTurnOrder() {
	std::cout << ANSI_CURSOR_UP(50);
	GameplayStatics::ANSI_CURSOR_DOWN_N(static_cast<int>(TurnTable.size()));
	
	std::cout << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << '\n';
	std::cout << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << '\n';

	int Total = 0;
	std::string COLOR;
	for (int i = nTurn; i < static_cast<int>(TurnTable.size()); i++) {
		char Alias = TurnTable[i].lock()->GetAlias();
		if (TurnTable[i].lock()) Alias = TurnTable[i].lock()->GetAlias();
		else continue;
		if (Alias >= '0' && Alias <= '9') COLOR = COLOR_PLAYER;
		else COLOR = COLOR_ENEMY;
		if (Total == 0) std::cout << COLOR << ANSI_COLOR_BLINK << ANSI_CURSOR_RIGHT(83) << "->" << Alias << "<-" << ANSI_COLOR_RESET << '\n' << '\n';
		else std::cout << COLOR << ANSI_CURSOR_RIGHT(85) << Alias << '\n' << '\n';
		if (++Total == 9) break;
	}

	while (Total < 9) {
		for (const auto& WPtrCharacter : TurnTable) {
			const char Alias = WPtrCharacter.lock()->GetAlias();
			if (Alias >= '0' && Alias <= '9') COLOR = COLOR_PLAYER;
			else COLOR = COLOR_ENEMY;
			std::cout << COLOR << ANSI_CURSOR_RIGHT(85) << Alias << '\n' << '\n';
			if (++Total == 9) break;
		}
	}

	std::cout << ANSI_CURSOR_UP(1) << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << '\n';
	std::cout << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << '\n';
	std::cout << ANSI_COLOR_RESET;
}

void CombatManager::ApplyStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target, const CharacterStat& CharStat, INOUT float& Total, const bool bIsOnApply) {
	float Value;
	const float Delta = CharStat.GetDelta(Effect->Instigator);

	if (CharStat.StatMod == EStatMod::ADDITIVE) {
		Total += Delta;
		Value = Total;
	}
	else Value = Delta;

	if (CharStat.StatType == EStatType::HEALTH)
		Value = GameplayStatics::ApplyDamage(GetTurnCharacter(), CharStat.PtrCharacter, Delta, Effect->ActiveSpell, bIsOnApply);

	const auto SpellClass = Effect->ActiveSpell->GetClass();

	if (SpellClass == ESpellClass::MAGIC)
		OnMagicReceivedBegin(Target, Effect->Instigator);
	else if (SpellClass == ESpellClass::MELEE)
		OnMeleeReceivedBegin(Target, Effect->Instigator);
	else if (SpellClass == ESpellClass::RANGED)
		OnRangedReceivedBegin(Target, Effect->Instigator);

	*CharStat.Stat += Value;

	if (SpellClass == ESpellClass::MAGIC)
		OnMagicReceivedEnd(Target, Effect->Instigator);
	else if (SpellClass == ESpellClass::MELEE)
		OnMeleeReceivedEnd(Target, Effect->Instigator);
	else if (SpellClass == ESpellClass::RANGED)
		OnRangedReceivedEnd(Target, Effect->Instigator);

	FlagDeadCharacters();
}

void CombatManager::HandleCombatEffect(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target) {	
	RPG_ASSERT(!Target.expired(), "HandleCombatEffect")

	if (Effect->ApplyParams)
		HandleApplyStat(Effect, Target);
	Effect->TurnApplied = nCycle;

	if (Effect->EffectParams)
		HandleEffectStat(Effect, Target);
}

void CombatManager::HandleApplyStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target) {
	auto& AllyStats = Effect->ApplyParams->EffectStat->AllyStats;
	auto& EnemyStats = Effect->ApplyParams->EffectStat->EnemyStats;

	for (auto& stat : AllyStats)
		if ((Effect->TurnApplied == -1) || (stat.PtrCharacter == Target.lock().get() && stat.StatType != EStatType::HEALTH))
			ApplyStat(Effect, Target, stat, stat.Total, true);

	for (auto& stat : EnemyStats)
		if ((Effect->TurnApplied == -1) || (stat.PtrCharacter == Target.lock().get() && stat.StatType != EStatType::HEALTH))
			ApplyStat(Effect, Target, stat, stat.Total, true);
}

void CombatManager::HandleEffectStat(const std::shared_ptr<CombatEffect>& Effect, const std::weak_ptr<Character>& Target) {
	auto& AllyStats = Effect->EffectParams->EffectStat->AllyStats;
	auto& EnemyStats = Effect->EffectParams->EffectStat->EnemyStats;

	for (auto& stat : AllyStats)
		if (stat.PtrCharacter == Target.lock().get() || stat.PtrCharacter == Effect->Instigator.get())
			ApplyStat(Effect, Target, stat, stat.Total, false);

	for (auto& stat : EnemyStats)
		if (stat.PtrCharacter == Target.lock().get() || stat.PtrCharacter == Effect->Instigator.get())
			ApplyStat(Effect, Target, stat, stat.Total, false);
}

void CombatManager::GetCharactersBase() {
	for (auto& character : PlayerCharacters)
		PlayerCharactersBase.push_back(*dynamic_cast<PlayerCharacter*>(character.lock().get()));

	for (auto& character : EnemyCharacters)
		EnemyCharactersBase.push_back(*dynamic_cast<EnemyCharacter*>(character.lock().get()));
}

void CombatManager::ResetCharacterValues() {
	// Reset player characters for re-application of spells
	for (int i = 0; i < static_cast<int>(PlayerCharacters.size()); i++)
		if (GetTurnCharacter().lock().get() == PlayerCharacters[i].lock().get())
			// ReSharper disable once CppPossiblyUnintendedObjectSlicing
			*PlayerCharacters[i].lock() = PlayerCharactersBase[i];

	// Reset enemy characters for re-application of spells
	for (int i = 0; i < static_cast<int>(EnemyCharacters.size()); i++)
		if (GetTurnCharacter().lock().get() == EnemyCharacters[i].lock().get())
			// ReSharper disable once CppPossiblyUnintendedObjectSlicing
			*EnemyCharacters[i].lock() = EnemyCharactersBase[i];
	
	for (int i = 0; i < static_cast<int>(SummonCharacters.size()); i++)
		if (GetTurnCharacter().lock().get() == SummonCharacters[i].get())
			// ReSharper disable once CppPossiblyUnintendedObjectSlicing
			*SummonCharacters[i] = SummonCharactersBase[i];
}

void CombatManager::RemoveExpiredCombatEffects() {
	// Clear expired effects
	for (auto it = CombatEffects.begin(); it != CombatEffects.end();) {
		if (it->first == nCycle) {
			if (it->second->Instigator->GetAlias() == GetTurnAlias()) {
				for (auto& t : it->second->Targets) {
					if (!t.expired())
						t.lock()->RemoveEffectById(it->second->ActiveSpell->GetID());
				}
				it = CombatEffects.erase(it);
			}
			else ++it;
		}
		// if the turn is not the same we stop looking as the _combat_effects std::vector is sorted by turns
		//else break;
		else ++it;
	}
}

void CombatManager::ApplyEffectsOnEvent(const ECombatEvent OnEvent) {
	for (auto& Effect : CombatEffects | std::views::values) {
		if (!PlayerAvatar.lock()->IsInCombat()) return;

		const int EffectIndex = Effect->Index % static_cast<int>(Effect->Targets.size());
		if ((Effect->EffectParams && Effect->EffectParams->OnEvent == OnEvent) || Effect->ApplyParams) {
			if (!Effect->Targets[EffectIndex].expired()) {
				if (Effect->Targets[EffectIndex].lock()->GetAlias() == GetTurnCharacter().lock()->GetAlias()) {
					Effect->Index++;
					HandleCombatEffect(Effect, Effect->Targets[EffectIndex]);
				}
			}
			else Effect->Index++;
		}	
	}
}

void CombatManager::InstigatePassiveEffects(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets, const ECombatEvent OnEvent) {
	RPG_ASSERT(!Instigator.expired(), "InstigatePassiveEffects - Instigator")
	
	for (const auto& passive : Instigator.lock()->GetPassiveSpells()) {
		if (passive->GetOnEvent() == OnEvent) {
			passive->Instigator = Instigator;
			passive->Targets = Targets;
			passive->Apply();
		}
	}
}

void CombatManager::TriggerPassiveEffects(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator, const ECombatEvent OnEvent) {
	RPG_ASSERT(!Target.expired(), "TriggerPassiveEffects - character")
	RPG_ASSERT(!Instigator.expired(), "TriggerPassiveEffects - Instigator")
	
	for (const auto& passive : Target.lock()->GetPassiveSpells()) {
		if (passive->GetOnEvent() == OnEvent) {
			passive->Instigator = Instigator;
			passive->Apply();
		}
	}
}

void CombatManager::FlagDeadCharacters() {
	for (auto& character : TurnTable)
		if (!character.expired()) character.lock()->CheckDie();
	KillFlaggedCharacters();
}

void CombatManager::KillFlaggedCharacters() {
	for (int i = 0; i < static_cast<int>(EnemyCharacters.size()); i++) 
		if (!EnemyCharacters[i].expired() && !EnemyCharacters[i].lock()->IsAlive())
			GameplayStatics::KillEnemy(i);
	
	RemoveDeadCharacters();

	if (PlayerAvatar.lock()->IsInCombat()) {
		if (nTurn == static_cast<int>(TurnTable.size())) nTurn = 0;
		else while (TurnTable[nTurn].expired())
			nTurn = ++nTurn % static_cast<int>(TurnTable.size());
	}
}

void CombatManager::RemoveDeadCharacters() {
	for (auto it = TurnTable.begin(); it != TurnTable.end();) {
		if (it->expired())
			it = TurnTable.erase(it);
		else ++it;
	}

	if (std::ranges::all_of(EnemyCharacters, [](const std::weak_ptr<Character>& WPtr) { return WPtr.expired(); }))
		ExitCombatMode();
}

void CombatManager::ExitCombatMode() {
	OnCombatEnd();

	for (auto& c : PlayerCharacters)
		c.lock()->SetIsInCombat(false);

	PlayerAvatar.lock()->SetIsInCombat(false);
}

void CombatManager::ResetCombatVariables() {
	PlayerCharacters.clear();
	PlayerCharactersBase.clear();
	EnemyCharacters.clear();
	EnemyCharactersBase.clear();
	CombatEffects.clear();
	TurnTable.clear();
	SummonCharacters.clear();
	SummonCharactersBase.clear();
	nTurn = 0;
	nCycle = 0;
}

void CombatManager::OnApplyEffect() {
	const auto& Effect = CombatEffects.back().second;
	if (Effect->ApplyParams)
		HandleCombatEffect(Effect, Effect->Targets[0]);
	
	for (auto& e : Effect->Targets)
		if (!e.expired()) e.lock()->AddEffectId(Effect->ActiveSpell->GetID());
}

void CombatManager::OnCombatBegin() {
	GetCharactersBase();
}

void CombatManager::OnCombatEnd() {
	//Turn this on to not make characters heal at end of combat. But the function has to be changed so only player characters are reset. 
	//Needs to be implemented after resurrection functionality
	//ResetCharacterValues();  

	ResetCombatVariables();

	GameplayStatics::RollLoot();
}

void CombatManager::OnTurnBegin() {
	ResetCharacterValues();
	RemoveExpiredCombatEffects();
	ApplyEffectsOnEvent(ECombatEvent::ON_TURN_BEGIN);
}

void CombatManager::OnTurnEnd() {
	ApplyEffectsOnEvent(ECombatEvent::ON_TURN_END);
}

void CombatManager::OnCycleBegin() {
	ApplyEffectsOnEvent(ECombatEvent::ON_CYCLE_BEGIN);
}

void CombatManager::OnCycleEnd() {
}


// public
void CombatManager::OnMagicBegin(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) {
	InstigatePassiveEffects(Instigator, Targets, ECombatEvent::ON_MAGIC_BEGIN);
}

void CombatManager::OnMagicEnd(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) {
	InstigatePassiveEffects(Instigator, Targets, ECombatEvent::ON_MAGIC_END);
}

void CombatManager::OnMagicReceivedBegin(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator) {
	TriggerPassiveEffects(Target, Instigator, ECombatEvent::ON_MAGIC_RECEIVED_BEGIN);
}

void CombatManager::OnMagicReceivedEnd(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator) {
	TriggerPassiveEffects(Target, Instigator, ECombatEvent::ON_MAGIC_RECEIVED_END);
}

void CombatManager::OnMeleeBegin(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) {
	InstigatePassiveEffects(Instigator, Targets, ECombatEvent::ON_MELEE_BEGIN);
}

void CombatManager::OnMeleeEnd(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) {
	InstigatePassiveEffects(Instigator, Targets, ECombatEvent::ON_MELEE_END);
}

void CombatManager::OnMeleeReceivedBegin(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator) {
	TriggerPassiveEffects(Target, Instigator, ECombatEvent::ON_MELEE_RECEIVED_BEGIN);
}

void CombatManager::OnMeleeReceivedEnd(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator) {
	TriggerPassiveEffects(Target, Instigator, ECombatEvent::ON_MELEE_RECEIVED_END);
}

void CombatManager::OnRangedBegin(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) {
	InstigatePassiveEffects(Instigator, Targets, ECombatEvent::ON_RANGED_BEGIN);
}

void CombatManager::OnRangedEnd(const std::weak_ptr<Character>& Instigator, const std::vector<std::weak_ptr<Character>>& Targets) {
	InstigatePassiveEffects(Instigator, Targets, ECombatEvent::ON_RANGED_END);
}

void CombatManager::OnRangedReceivedBegin(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator) {
	TriggerPassiveEffects(Target, Instigator, ECombatEvent::ON_RANGED_RECEIVED_BEGIN);
}

void CombatManager::OnRangedReceivedEnd(const std::weak_ptr<Character>& Target, const std::weak_ptr<Character>& Instigator) {
	TriggerPassiveEffects(Target, Instigator, ECombatEvent::ON_RANGED_RECEIVED_END);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////