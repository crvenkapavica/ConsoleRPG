#include "../Combat/CombatManager.h"
#include "../Spells/SpellManager.h"
#include "../Spells/EffectStructs.h"
#include "../Spells/PassiveSpell.h"
#include "../Characters/Character.h"

CombatManager& CombatManager::GetInstance() {
	static CombatManager _instance;
	return _instance;
}

void CombatManager::SetTurns(vector<weak_ptr<Character>> characters_1, vector<weak_ptr<Character>> characters_2) {
	// in future adjust turn tables to be either randomized, or sorted based on a particular stat (level, power, etc..)

	_players = characters_1;
	_enemies = characters_2;

	for (auto& character : characters_1) {
		_turn_table.push_back(character);
		character.lock()->SetIsInCombat(true);
	}

	for (auto& character : characters_2)
		_turn_table.push_back(character); 

	OnCombatBegin();
	OnCycleBegin();
}

void CombatManager::StartCombat(weak_ptr<Character> player) {

	_player = player;

	// COMBAT LOOP
	while (_player.lock()->IsInCombat()) {
		KillFlaggedCharacters();
		if (_player.lock()->IsInCombat())
			BeginTurn(GetTurnCharacter());
		else break;
	}
}

void CombatManager::AddCombatEffect(shared_ptr<CombatEffect> effect) {
	_combat_effects.push_back(make_pair(_turn + effect->_duration, effect));
	OnApplyEffect();
	sort(_combat_effects.begin(), _combat_effects.end());
}

void CombatManager::BeginTurn(weak_ptr<Character> character) {
	if (!character.lock()->IsOnTurn()) {
		character.lock()->SetIsOnTurn(true);
		OnTurnBegin();
	}

	if (!character.expired()) {
		for (const auto& effect : character.lock()->GetEffectIds())
			if (effect == ESpellID::BLIND)
				return EndTurn(character.lock().get());

		character.lock()->TakeTurn();
	}
}

void CombatManager::EndTurn(Character* character) {
	OnTurnEnd();
	character->SetIsOnTurn(false);

	if (_turn_index == _turn_table.size() - 1)
		OnCycleEnd();

	_turn_index = ++_turn_index % _turn_table.size();

	if (_turn_index == 0) {
		++_turn;
		OnCycleBegin();
	}
}

void CombatManager::AddSummonToCombat(shared_ptr<SummonCharacter> summon) {
	weak_ptr<Character> wptr_summon = summon; 
	_summons_base.push_back(*summon);
	_summons.push_back(move(summon));
	_turn_table.insert(_turn_table.begin() + _turn_index + 1, wptr_summon);
}

void CombatManager::DisplayTurnOrder() {
	cout << ANSI_CURSOR_UP(50);
	GameplayStatics::ANSI_CURSOR_DOWN_N(static_cast<int>(_turn_table.size()));
	
	cout << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << endl;
	cout << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << endl;

	int _total = 0;
	string COLOR;
	for (int i = _turn_index; i < _turn_table.size(); i++) {
		char c = _turn_table[i].lock()->GetAlias();
		if (_turn_table[i].lock().get()) c = _turn_table[i].lock()->GetAlias();
		else continue;
		if (c >= '0' && c <= '9') COLOR = COLOR_PLAYER;
		else COLOR = COLOR_ENEMY;
		if (_total == 0) cout << COLOR << ANSI_COLOR_BLINK << ANSI_CURSOR_RIGHT(83) << "->" << c << "<-" << ANSI_COLOR_RESET << endl << endl;
		else cout << COLOR << ANSI_CURSOR_RIGHT(85) << c << endl << endl;
		if (++_total == 9) break;
	}

	while (_total < 9) {
		for (int i = 0; i < _turn_table.size(); i++) {
			char c = _turn_table[i].lock()->GetAlias();
			if (c >= '0' && c <= '9') COLOR = COLOR_PLAYER;
			else COLOR = COLOR_ENEMY;
			cout << COLOR << ANSI_CURSOR_RIGHT(85) << c << endl << endl;
			if (++_total == 9) break;
		}
	}

	cout << ANSI_CURSOR_UP(1) << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << endl;
	cout << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << endl;
	cout << ANSI_COLOR_RESET;
}

void CombatManager::ApplyStat(shared_ptr<CombatEffect> effect, weak_ptr<Character> target, CharacterStat& character_stat, float& _total, bool isOnApply) {

	float value;
	float delta = character_stat.GetDelta(effect->_instigator);

	if (character_stat._stat_mod == EStatMod::ADDITIVE) {
		_total += delta;
		value = _total;
	}
	else value = delta;

	if (character_stat._stat_type == EStatType::HEALTH)
		value = GameplayStatics::ApplyDamage(GetTurnCharacter(), character_stat._character, delta, effect->_spell, isOnApply);

	auto spell_class = effect->_spell->GetClass();

	if (spell_class == ESpellClass::MAGIC)
		OnMagicReceivedBegin(target, effect->_instigator);
	else if (spell_class == ESpellClass::MELEE)
		OnMeleeReceivedBegin(target, effect->_instigator);
	else if (spell_class == ESpellClass::RANGED)
		OnRangedReceivedBegin(target, effect->_instigator);

	*character_stat._stat += value;

	if (spell_class == ESpellClass::MAGIC)
		OnMagicReceivedEnd(target, effect->_instigator);
	else if (spell_class == ESpellClass::MELEE)
		OnMeleeReceivedEnd(target, effect->_instigator);
	else if (spell_class == ESpellClass::RANGED)
		OnRangedReceivedEnd(target, effect->_instigator);

	FlagDeadCharacters();
}

void CombatManager::HandleCombatEffect(shared_ptr<CombatEffect> effect, weak_ptr<Character> target) {	
	RPG_ASSERT(!target.expired(), "HandleCombatEffect");

	if (effect->_apply_params)
		HandleApplyStat(effect, target);
	effect->_turn_applied = _turn;

	if (effect->_effect_params)
		HandleEffectStat(effect, target);
}

void CombatManager::HandleApplyStat(shared_ptr<CombatEffect> effect, weak_ptr<Character> target) {
	auto& ally_stats = effect->_apply_params->_effect_stat->_ally_stat;
	auto& enemy_stats = effect->_apply_params->_effect_stat->_enemy_stat;

	for (auto& stat : ally_stats)
		if ((effect->_turn_applied == -1) || (stat._character == target.lock().get() && stat._stat_type != EStatType::HEALTH))
			ApplyStat(effect, target, stat, stat._total, 1);

	for (auto& stat : enemy_stats)
		if ((effect->_turn_applied == -1) || (stat._character == target.lock().get() && stat._stat_type != EStatType::HEALTH))
			ApplyStat(effect, target, stat, stat._total, 1);
}

void CombatManager::HandleEffectStat(shared_ptr<CombatEffect> effect, weak_ptr<Character> target) {
	auto& ally_stats = effect->_effect_params->_effect_stat->_ally_stat;
	auto& enemy_stats = effect->_effect_params->_effect_stat->_enemy_stat;

	for (auto& stat : ally_stats)
		if (stat._character == target.lock().get() || stat._character == effect->_instigator.get())
			ApplyStat(effect, target, stat, stat._total, 0);

	for (auto& stat : enemy_stats)
		if (stat._character == target.lock().get() || stat._character == effect->_instigator.get())
			ApplyStat(effect, target, stat, stat._total, 0);
}

void CombatManager::GetCharactersBase() {

	for (auto& character : _players)
		_players_base.push_back(*static_cast<PlayerCharacter*>(character.lock().get()));

	for (auto& character : _enemies)
		_enemies_base.push_back(*static_cast<EnemyCharacter*>(character.lock().get()));
}

void CombatManager::ResetCharacterValues() {

	// Reset player characters for re-application of spells
	for (int i = 0; i < _players.size(); i++)
		if (GetTurnCharacter().lock().get() == _players[i].lock().get())
			*_players[i].lock().get() = _players_base[i];

	// Reset enemy characters for re-application of spells
	for (int i = 0; i < _enemies.size(); i++)
		if (GetTurnCharacter().lock().get() == _enemies[i].lock().get())
			*_enemies[i].lock().get() = _enemies_base[i];
	
	for (int i = 0; i < _summons.size(); i++)
		if (GetTurnCharacter().lock().get() == _summons[i].get())
			*_summons[i].get() = _summons_base[i];
}

void CombatManager::RemoveExpiredCombatEffects() {
	
	// Clear expired effects
	for (auto it = _combat_effects.begin(); it != _combat_effects.end();) {
		if (it->first == _turn) {
			if (it->second->_instigator->GetAlias() == GetTurnAlias()) {
				for (auto& t : it->second->_targets) {
					if (!t.expired())
						t.lock()->RemoveEffectById(it->second->_spell->GetId());
				}
				it = _combat_effects.erase(it);
			}
			else ++it;
		}
		// if the turn is not the same we stop looking as the _combat_effects vector is sorted by turns
		//else break;
		else ++it;
	}
}

void CombatManager::ApplyEffectsOnEvent(ECombatEvent on_event) {
	for (auto& effect : _combat_effects) {
		if (!_player.lock()->IsInCombat()) return;
		int idx = effect.second->i % static_cast<int>(effect.second->_targets.size());
		if ((effect.second->_effect_params && effect.second->_effect_params->_on_event == on_event) || effect.second->_apply_params)
			if (!effect.second->_targets[idx].expired()) {
				if (effect.second->_targets[idx].lock()->GetAlias() == GetTurnCharacter().lock()->GetAlias()) {
					effect.second->i++;
					HandleCombatEffect(effect.second, effect.second->_targets[idx]);
				}
			}
			else effect.second->i++;
	}
}

void CombatManager::InstigatePassiveEffects(const weak_ptr<Character>& instigator, vector<weak_ptr<Character>> targets, ECombatEvent on_event) {
	RPG_ASSERT(!instigator.expired(), "InstigatePassiveEffects - instigator");

	for (const auto& passive : instigator.lock()->GetPassiveSpells()) {
		if (passive->GetOnEvent() == on_event) {
			passive->_instigator = instigator;
			passive->_targets = targets;
			passive->Apply();
		}
	}
}

void CombatManager::TriggerPassiveEffects(const weak_ptr<Character>& character, const weak_ptr<Character>& instigator, ECombatEvent on_event) {
	RPG_ASSERT(!character.expired(), "TriggerPassiveEffects - character");
	RPG_ASSERT(!instigator.expired(), "TriggerPassiveEffects - instigator");

	for (const auto& passive : character.lock()->GetPassiveSpells()) {
		if (passive->GetOnEvent() == on_event) {
			passive->_instigator = instigator;
			passive->Apply();
		}
	}
}

void CombatManager::FlagDeadCharacters() {
	for (auto& character : _turn_table)
		if (!character.expired()) character.lock()->CheckDie();
	KillFlaggedCharacters();
}

void CombatManager::KillFlaggedCharacters() {

	for (int i = 0; i < _enemies.size(); i++) 
		if (!_enemies[i].expired() && !_enemies[i].lock()->IsAlive())
			GameplayStatics::KillEnemy(i);
	
	RemoveDeadCharacters();

	if (_player.lock()->IsInCombat())
		if (_turn_index == static_cast<int>(_turn_table.size()))
			_turn_index = 0;
		else while (_turn_table[_turn_index].expired())
			_turn_index = ++_turn_index % _turn_table.size();
}

void CombatManager::RemoveDeadCharacters() {
	for (auto it = _turn_table.begin(); it != _turn_table.end();) {
		if (it->expired())
			it = _turn_table.erase(it);
		else ++it;
	}

	if (all_of(_enemies.begin(), _enemies.end(), [](const weak_ptr<Character>& wp) { return wp.expired(); })) {
		ExitCombatMode();
	}
}

void CombatManager::ExitCombatMode() {
	OnCombatEnd();

	for (auto& c : _players)
		c.lock()->SetIsInCombat(false);

	_player.lock()->SetIsInCombat(false);
}

void CombatManager::ResetCombatVariables() {
	_players.clear();
	_players_base.clear();
	_enemies.clear();
	_enemies_base.clear();
	_combat_effects.clear();
	_turn_table.clear();
	_summons.clear();
	_summons_base.clear();
	_turn_index = 0;
	_turn = 0;
}

void CombatManager::OnApplyEffect() {
	auto& effect = _combat_effects.back().second;
	if (effect->_apply_params) {
		HandleCombatEffect(effect, effect->_targets[0]);
	}

	for (auto& e : effect->_targets) {
		if (!e.expired())
			e.lock()->AddEffectId(effect->_spell->GetId());
	}
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
void CombatManager::OnMagicBegin(weak_ptr<Character> instigator, vector<weak_ptr<Character>> targets) {
	InstigatePassiveEffects(instigator, targets, ECombatEvent::ON_MAGIC_BEGIN);
}

void CombatManager::OnMagicEnd(weak_ptr<Character> instigator, vector<weak_ptr<Character>> targets) {
	InstigatePassiveEffects(instigator, targets, ECombatEvent::ON_MAGIC_END);
}

void CombatManager::OnMagicReceivedBegin(weak_ptr<Character> character, weak_ptr<Character> instigator) {
	TriggerPassiveEffects(character, instigator, ECombatEvent::ON_MAGIC_RECEIVED_BEGIN);
}

void CombatManager::OnMagicReceivedEnd(weak_ptr<Character> character, weak_ptr<Character> instigator) {
	TriggerPassiveEffects(character, instigator, ECombatEvent::ON_MAGIC_RECEIVED_END);
}

void CombatManager::OnMeleeBegin(weak_ptr<Character> instigator, vector<weak_ptr<Character>> targets) {
	InstigatePassiveEffects(instigator, targets, ECombatEvent::ON_MELEE_BEGIN);
}

void CombatManager::OnMeleeEnd(weak_ptr<Character> instigator, vector<weak_ptr<Character>> targets) {
	InstigatePassiveEffects(instigator, targets, ECombatEvent::ON_MELEE_END);
}

void CombatManager::OnMeleeReceivedBegin(weak_ptr<Character> character, weak_ptr<Character> instigator) {
	TriggerPassiveEffects(character, instigator, ECombatEvent::ON_MELEE_RECEIVED_BEGIN);
}

void CombatManager::OnMeleeReceivedEnd(weak_ptr<Character> character, weak_ptr<Character> instigator) {
	TriggerPassiveEffects(character, instigator, ECombatEvent::ON_MELEE_RECEIVED_END);
}

void CombatManager::OnRangedBegin(weak_ptr<Character> instigator, vector<weak_ptr<Character>> targets) {
	InstigatePassiveEffects(instigator, targets, ECombatEvent::ON_RANGED_BEGIN);
}

void CombatManager::OnRangedEnd(weak_ptr<Character> instigator, vector<weak_ptr<Character>> targets) {
	InstigatePassiveEffects(instigator, targets, ECombatEvent::ON_RANGED_END);
}

void CombatManager::OnRangedReceivedBegin(weak_ptr<Character> character, weak_ptr<Character> instigator) {
	TriggerPassiveEffects(character, instigator, ECombatEvent::ON_RANGED_RECEIVED_BEGIN);
}

void CombatManager::OnRangedReceivedEnd(weak_ptr<Character> character, weak_ptr<Character> instigator) {
	TriggerPassiveEffects(character, instigator, ECombatEvent::ON_RANGED_RECEIVED_END);
}
///////