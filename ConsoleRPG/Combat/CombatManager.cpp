#include "../Combat/CombatManager.h"
#include "../Spells/SpellManager.h"
#include "../Spells/EffectStructs.h"
#include "../Characters/Character.h"
#include "../Characters/EnemyCharacter.h"
#include "../Spells/PassiveSpell.h"

CombatManager& CombatManager::GetInstance() {
	static CombatManager _instance;
	return _instance;
}

void CombatManager::SetTurns(vector<weak_ptr<PlayerCharacter>> characters_1, vector<weak_ptr<EnemyCharacter>> characters_2) {
	// in future adjust turn tables to be either randomized, or sorted based on a particular stat (level, power, etc..)

	for (auto& character : characters_1) {
		_turn_table.push_back(character);
		character.lock()->SetIsInCombat(true);
	}

	for (auto& character : characters_2) {
		_turn_table.push_back(character); 
	}

	_player_characters = characters_1;
	_enemy_characters = characters_2;

	OnCombatBegin();
	OnCycleBegin();
}

void CombatManager::StartCombat(weak_ptr<PlayerCharacter> player) {

	BeginTurn(_turn_table[0].lock().get());

	while (player.lock()->IsInCombat()) {
		DestroyDeadCharacters();
		if (!(all_of(_enemy_characters.begin(), _enemy_characters.end(), [](const weak_ptr<EnemyCharacter>& wptr) { return wptr.expired(); })))
			GetTurnCharacter().lock()->TakeTurn();
	}
	ResetCombatVariables();
}

void CombatManager::AddCombatEffect(unique_ptr<CombatEffect> effect) {
	_combat_effects.push_back(make_pair(_turn + effect->_duration, move(effect)));
	OnApplyEffect();
	sort(_combat_effects.begin(), _combat_effects.end());
}

void CombatManager::BeginTurn(Character* character) {
	character->SetIsOnTurn(true);
	OnTurnBegin();
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

	BeginTurn(_turn_table[_turn_index].lock().get());
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
		//if (_turn_table[i].lock().get()) c = _turn_table[i].lock()->GetAlias();
		//else continue;
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

void CombatManager::ApplyStat(CombatEffect* effect, CharacterStat& character_stat, float& _total, bool isOnApply) {

	float value;
	float delta = character_stat.GetDelta(effect->_instigator);

	if (character_stat._stat_mod == EStatMod::ADDITIVE) {
		_total += delta;
		value = _total;
	}
	else value = delta;

	if (character_stat._stat_type == EStatType::HEALTH)
		value = GameplayStatics::ApplyDamage(GetTurnCharacter().lock().get(), character_stat._character, delta, effect->_spell, isOnApply);

	*character_stat._stat += value;
}

void CombatManager::HandleCombatEffect(CombatEffect* effect, Character* target) {

	//int struct_flags = effect->_apply_params ? effect->_apply_params->_struct_flags : 0;
	//if (struct_flags & 1) HandleApplyStat(effect, target);
	//effect->_turn_applied = _turn;

	//struct_flags = effect->_effect_params ? effect->_effect_params->_struct_flags : 0;
	//if (struct_flags & 1) HandleEffectStat(effect, target);

	if (effect->_apply_params)
		HandleApplyStat(effect, target);
	effect->_turn_applied = _turn;

	if (effect->_effect_params)
		HandleEffectStat(effect, target);
}

void CombatManager::HandleApplyStat(CombatEffect* effect, Character* target) {
	auto& ally_stats = effect->_apply_params->_effect_stat->_ally_stat;
	auto& enemy_stats = effect->_apply_params->_effect_stat->_enemy_stat;

	for (auto& stat : ally_stats)
		if ((effect->_turn_applied == -1) || (stat._character == target && stat._stat_type != EStatType::HEALTH))
			ApplyStat(effect, stat, stat._total, 1);

	for (auto& stat : enemy_stats)
		if ((effect->_turn_applied == -1) || (stat._character == target && stat._stat_type != EStatType::HEALTH))
			ApplyStat(effect, stat, stat._total, 1);
}

void CombatManager::HandleEffectStat(CombatEffect* effect, Character* target) {
	auto& ally_stats = effect->_effect_params->_effect_stat->_ally_stat;
	auto& enemy_stats = effect->_effect_params->_effect_stat->_enemy_stat;

	for (auto& stat : ally_stats)
		if (stat._character == target || stat._character == effect->_instigator)
			ApplyStat(effect, stat, stat._total, 0);

	for (auto& stat : enemy_stats)
		if (stat._character == target || stat._character == effect->_instigator)
			ApplyStat(effect, stat, stat._total, 0);
}

void CombatManager::GetCharactersBase() {
	for (auto& character : _player_characters) {
		PlayerCharacter c = *character.lock().get();
		_player_characters_base.push_back(c);
	}

	for (auto& character : _enemy_characters) {
		EnemyCharacter c = *character.lock().get();
		_enemy_characters_base.push_back(c);
	}
}

void CombatManager::ResetCharacterValues() {
	// Reset player characters for re-application of spells
	for (int i = 0; i < _player_characters.size(); i++) {
		if (GetTurnCharacter().lock().get() == _player_characters[i].lock().get())
			*_player_characters[i].lock().get() = _player_characters_base[i];
	}

	// Reset enemy characters for re-application of spells
	for (int i = 0; i < _enemy_characters.size(); i++) {
		if (GetTurnCharacter().lock().get() == _enemy_characters[i].lock().get())
			*_enemy_characters[i].lock().get() = _enemy_characters_base[i];
	}
}

void CombatManager::RemoveExpiredCombatEffects() {

	ResetCharacterValues();
	
	// Clear expired effects
	for (auto it = _combat_effects.begin(); it != _combat_effects.end();) {
		if (it->first == _turn) {
			if (it->second->_instigator->GetAlias() == GetTurnAlias()) {
				for (auto& t : it->second->_targets) {
					if (!t.expired())
						t.lock()->RemoveEffectById(it->second->_spell->GetID());
				}
				it = _combat_effects.erase(it);
			}
			else ++it;
		}
		// if the turn is not the same we stop looking as the _combat_effects vector is sorted by turns
		else break;
	}
}

void CombatManager::ApplyEffectsOnEvent(ECombatEvent on_event) {

	for (auto& effect : _combat_effects) {
		int idx = effect.second->i % static_cast<int>(effect.second->_targets.size());
		if ((effect.second->_effect_params && effect.second->_effect_params->_on_event == on_event) || effect.second->_apply_params)/*effect.second->_apply_params->_on_event == on_event*/
			if (!effect.second->_targets[idx].expired()) {
				if (effect.second->_targets[idx].lock()->GetAlias() == GetTurnAlias()) {
					effect.second->i++;
					HandleCombatEffect(effect.second.get(), effect.second->_targets[idx].lock().get());
				}
			}
			else effect.second->i++;
	}
}

void CombatManager::ApplyPassiveEffects(ECombatEvent on_event, Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<weak_ptr<Character>> targets) {
	for (const auto& passive : instigator->GetPassiveSpells()) {
		if (passive->GetOnEvent() == on_event) {
			passive->_instigator = instigator;
			passive->_targets = targets;
			passive->_team1 = team1;
			passive->_team2 = team2;
			passive->Apply();
		}
	}
}

void CombatManager::DestroyDeadCharacters() {
	int idx = GetDeadIdx();
	if (idx != -1) GameplayStatics::KillEnemy(idx);
	RemoveDeadCharacters();
}

int CombatManager::GetDeadIdx() {

	for (int i = 0; i < _enemy_characters.size(); i++)
		if (!_enemy_characters[i].expired() && !_enemy_characters[i].lock()->IsAlive()) {
			if (_enemy_characters[i].lock() == GetTurnCharacter().lock())
				bDeadOnTurn = true;
			return i;
		}

	return -1;
}

void CombatManager::RemoveDeadCharacters() {
	for (auto it = _turn_table.begin(); it != _turn_table.end();) {
		if (it->expired()) {
			it = _turn_table.erase(it);
			if (bDeadOnTurn) {
				if (_turn_table.size() == _player_characters.size()) {
					_turn_index = 0;
				}
				else {
					if (_turn_index >= _turn_table.size())
						_turn_index = 0;
					BeginTurn(_turn_table[_turn_index].lock().get());

					int idx = GetDeadIdx();
					if (idx != -1) GameplayStatics::KillEnemy(idx);
				}
			}
		}
		else ++it;
	}
	ExitCombatMode();
}

void CombatManager::ExitCombatMode() {
	if (all_of(_enemy_characters.begin(), _enemy_characters.end(), [](const weak_ptr<EnemyCharacter>& wp) { return wp.expired(); })) {

		OnCombatEnd();

		for (auto& c : _player_characters)
			c.lock()->SetIsInCombat(false);

		GameplayStatics::ExitCombatMode();
	}
}

void CombatManager::ResetCombatVariables() {
	_player_characters.clear();
	_player_characters_base.clear();
	_enemy_characters.clear();
	_enemy_characters_base.clear();
	_combat_effects.clear();
	_turn_table.clear();
	_turn_index = 0;
	_turn = 0;
}

void CombatManager::OnApplyEffect() {
	auto& effect = _combat_effects.back().second;
	if (effect->_apply_params->_struct_flags) {
		HandleCombatEffect(effect.get());
	}

	for (auto& e : effect->_targets) {
		if (!e.expired())
			e.lock()->AddEffectId(effect->_spell->GetID());
	}
}

void CombatManager::OnCombatBegin() {
	GetCharactersBase();
}
void CombatManager::OnCombatEnd() {

}

void CombatManager::OnTurnBegin() {
	//auto& s = GameplayStatics::GetCombatLogStream();
	//s << "BEGIN ===== INDEX: " << _turn_index << " SIZE: " << _turn_table.size() << " ALIAS: " << GetTurnAlias() << "\n";

	RemoveExpiredCombatEffects();
	ApplyEffectsOnEvent(ECombatEvent::ON_TURN_BEGIN);
}

void CombatManager::OnTurnEnd() {
	bDeadOnTurn = false;
	//auto& s = GameplayStatics::GetCombatLogStream();
	//s << "END ===== INDEX: " << _turn_index << " SIZE: " << _turn_table.size() << " ALIAS: " << GetTurnAlias() << "\n";

	ApplyEffectsOnEvent(ECombatEvent::ON_TURN_END);
}

void CombatManager::OnCycleBegin() {

	ApplyEffectsOnEvent(ECombatEvent::ON_CYCLE_BEGIN);
}

void CombatManager::OnCycleEnd() {

}


// public
void CombatManager::OnCastBegin(Character* instigator, vector<weak_ptr<Character>> targets) {
	//ApplyPassiveEffects(ECombatEvent::ON_CAST_BEGIN, instigator, team1, team2, targets);
}

void CombatManager::OnCastEnd(Character* instigator, vector<weak_ptr<Character>> targets) {
	//ApplyPassiveEffects(ECombatEvent::ON_CAST_END, instigator, team1, team2, targets);
}
///////