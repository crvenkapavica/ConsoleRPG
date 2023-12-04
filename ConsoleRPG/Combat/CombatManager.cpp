#include "../Combat/CombatManager.h"
#include "../Spells/SpellManager.h"
#include "../Effects/EffectStructs.h"
#include "../Characters/Character.h"
#include "../Characters/EnemyCharacter.h"
#include "../Effects/PassiveEffect.h"

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

void CombatManager::AddCombatEffect(unique_ptr<CombatEffect> combat_effect) {
	_combat_effects.push_back(make_pair(_turn + combat_effect->_duration, move(combat_effect)));
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

	int total = 0;
	string COLOR;
	for (int i = _turn_index; i < _turn_table.size(); i++) {
		char c = _turn_table[i].lock()->GetAlias();
		//if (_turn_table[i].lock().get()) c = _turn_table[i].lock()->GetAlias();
		//else continue;
		if (c >= '0' && c <= '9') COLOR = COLOR_PLAYER;
		else COLOR = COLOR_ENEMY;
		if (total == 0) cout << COLOR << ANSI_COLOR_BLINK << ANSI_CURSOR_RIGHT(83) << "->" << c << "<-" << ANSI_COLOR_RESET << endl << endl;
		else cout << COLOR << ANSI_CURSOR_RIGHT(85) << c << endl << endl;
		if (++total == 9) break;
	}

	while (total < 9) {
		for (int i = 0; i < _turn_table.size(); i++) {
			char c = _turn_table[i].lock()->GetAlias();
			if (c >= '0' && c <= '9') COLOR = COLOR_PLAYER;
			else COLOR = COLOR_ENEMY;
			cout << COLOR << ANSI_CURSOR_RIGHT(85) << c << endl << endl;
			if (++total == 9) break;
		}
	}

	cout << ANSI_CURSOR_UP(1) << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << endl;
	cout << ANSI_COLOR_BROWN_LIGHT << ANSI_CURSOR_RIGHT(85) << "^" << endl;
	cout << ANSI_COLOR_RESET;
}

void CombatManager::ApplyStat(EEffectValueAction value_action, CharacterStat& character_stat, shared_ptr<ActiveEffect> effect, float& total, bool isOnApply) {

	float value;

	if (character_stat._stat_mod == EStatMod::ADDITIVE) {
		total += character_stat._value;
		value = total;
	}
	else value = character_stat._value;

	if (character_stat._stat_type == EStatType::HEALTH)
		value = GameplayStatics::ApplyDamage(GetTurnCharacter().lock().get(), character_stat._character, character_stat._value, effect, isOnApply);

	switch (value_action) {
		case EEffectValueAction::UPDATE_BASE: {
			character_stat._stat->UpdateBase(value);
			break;
		}
		case EEffectValueAction::UPDATE_ACTUAL: {
			character_stat._stat->UpdateActual(value, character_stat._character);
			break;
		}
		default:
			break;
	}
}

void CombatManager::ApplyRes(CharacterRes& character_res, float& total) {

	float value;

	if (character_res._stat_mod == EStatMod::ADDITIVE) {
		total += character_res._value;
		value = total;
	}
	else value = character_res._value;

	*character_res._res += value;
}

void CombatManager::HandleCombatEffect(CombatEffect* combat_effect, Character* target) {

	uint8_t struct_flags = combat_effect->_apply_params._struct_flags;
	if (struct_flags & 1) {} // MULTI_TARGET;
	if (struct_flags >> 1 & 1) HandleApplyStat(combat_effect, target);
	if (struct_flags >> 2 & 1) HandleApplyRes(combat_effect, target);

	if (combat_effect->_turn_applied == -1)
		combat_effect->_turn_applied = _turn;

	struct_flags = combat_effect->_effect_params._struct_flags;
	if (struct_flags & 1) {} // MULTI_TARGET
	if (struct_flags >> 1 & 1) HandleEffectStat(combat_effect, target);
	if (struct_flags >> 2 & 1) HandleEffectRes(combat_effect, target);
	
}

void CombatManager::HandleApplyStat(CombatEffect* combat_effect, Character* target) {
	auto& ally_stats = combat_effect->_apply_params._effect_stat->_ally_stat;
	auto& enemy_stats = combat_effect->_apply_params._effect_stat->_enemy_stat;
	auto value_action = combat_effect->_apply_params._effect_stat->_value_action;
	auto& effect = combat_effect->_effect;

	for (auto& stat : ally_stats)
		if ((combat_effect->_turn_applied == -1) || (stat._character == target && stat._stat != &stat._character->GetHealth()))
			ApplyStat(value_action, stat, effect, stat.total, 1);

	for (auto& stat : enemy_stats)
		if ((combat_effect->_turn_applied == -1) || (stat._character == target && stat._stat != &stat._character->GetHealth()))
			ApplyStat(value_action, stat, effect, stat.total, 1);
}

void CombatManager::HandleEffectStat(CombatEffect* combat_effect, Character* target) {
	auto& ally_stats = combat_effect->_effect_params._effect_stat->_ally_stat;
	auto& enemy_stats = combat_effect->_effect_params._effect_stat->_enemy_stat;
	auto value_action = combat_effect->_effect_params._effect_stat->_value_action;
	auto& effect = combat_effect->_effect;

	for (auto& stat : ally_stats)
		if (stat._character == target || stat._character == combat_effect->_instigator)
			ApplyStat(value_action, stat, effect, stat.total, 0);

	for (auto& stat : enemy_stats)
		if (stat._character == target || stat._character == combat_effect->_instigator)
			ApplyStat(value_action, stat, effect, stat.total, 0);
}

void CombatManager::HandleApplyRes(CombatEffect* combat_effect, Character* target) {
	auto& ally_res = combat_effect->_apply_params._effect_res->_ally_res;
	auto& enemy_res = combat_effect->_apply_params._effect_res->_enemy_res;

	for (auto& r : ally_res)
		if (combat_effect->_turn_applied == -1 || r._character == target)
			ApplyRes(r, r.total);
	for (auto& r : enemy_res)
		if (combat_effect->_turn_applied == -1 || r._character == target)
			ApplyRes(r, r.total);
}

void CombatManager::HandleEffectRes(CombatEffect* combat_effect, Character* target) {
	auto& ally_res = combat_effect->_effect_params._effect_res->_ally_res;
	auto& enemy_res = combat_effect->_effect_params._effect_res->_enemy_res;;

	for (auto& r : ally_res)
		if (r._character == target || r._character == combat_effect->_instigator) // treba provjeriti ako ide instigator tu...
			ApplyRes(r, r.total);

	for (auto& r : enemy_res)
		if (r._character == target || r._character == combat_effect->_instigator)
			ApplyRes(r, r.total);
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
						t.lock()->RemoveEffectById(it->second->_effect->GetID());
				}
				it = _combat_effects.erase(it);
			}
			else ++it;
		}
		// if the turn is not the same we stop looking as the _combat_effects vector is sorted by turns
		else break;
	}
}

void CombatManager::ApplyEffectsOnEvent(EEffectEvent on_event) {

	for (auto& effect : _combat_effects) {
		int idx = effect.second->i % static_cast<int>(effect.second->_targets.size());
		if ((effect.second->_effect_params._on_event == on_event || effect.second->_apply_params._on_event == on_event))
			if (!effect.second->_targets[idx].expired()) {
				if (effect.second->_targets[idx].lock()->GetAlias() == GetTurnAlias()) {
					effect.second->i++;
					HandleCombatEffect(effect.second.get(), effect.second->_targets[idx].lock().get());
				}
			}
			else effect.second->i++;
	}
}

void CombatManager::ApplyPassiveEffects(EEffectEvent on_event, Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<weak_ptr<Character>> targets) {
	for (const auto& passive : instigator->GetPassives()) {
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
	if (effect->_apply_params._struct_flags) {
		HandleCombatEffect(effect.get());
	}

	for (auto& e : effect->_targets) {
		if (!e.expired())
			e.lock()->AddEffectId(effect->_effect->GetID());
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
	ApplyEffectsOnEvent(EEffectEvent::ON_TURN_BEGIN);
}

void CombatManager::OnTurnEnd() {
	bDeadOnTurn = false;
	//auto& s = GameplayStatics::GetCombatLogStream();
	//s << "END ===== INDEX: " << _turn_index << " SIZE: " << _turn_table.size() << " ALIAS: " << GetTurnAlias() << "\n";

	ApplyEffectsOnEvent(EEffectEvent::ON_TURN_END);
}

void CombatManager::OnCycleBegin() {

	ApplyEffectsOnEvent(EEffectEvent::ON_CYCLE_BEGIN);
}

void CombatManager::OnCycleEnd() {

}


// public
void CombatManager::OnCastBegin(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<weak_ptr<Character>> targets) {
	ApplyPassiveEffects(EEffectEvent::ON_CAST_BEGIN, instigator, team1, team2, targets);
}

void CombatManager::OnCastEnd(Character* instigator, vector<weak_ptr<Character>> team1, vector<weak_ptr<Character>> team2, vector<weak_ptr<Character>> targets) {
	ApplyPassiveEffects(EEffectEvent::ON_CAST_END, instigator, team1, team2, targets);
}
///////