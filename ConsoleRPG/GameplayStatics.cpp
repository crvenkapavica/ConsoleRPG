#include "GameplayStatics.h"
#include "Characters/Character.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "Spells/Spell.h"
#include "Spells/CSpellData.h"
#include "Spells/SpellManager.h"
#include "Combat/CombatManager.h"
#include "MapGenerator/MapGenerator.h"
#include "Resistances.h"
#include "Effects/EffectStructs.h"
#include "Effects/ActiveEffect.h"

weak_ptr<PlayerCharacter> GameplayStatics::_player;
vector<PlayerCharacter*> GameplayStatics::_player_characters;
vector<EnemyCharacter*> GameplayStatics::_enemy_characters;
SpellManager* GameplayStatics::_spell_manager = nullptr;
CombatManager* GameplayStatics::_combat_manager = nullptr;
MapGenerator* GameplayStatics::_map_generator = nullptr;
ConsoleMenu* GameplayStatics::_menu = nullptr;
vector<weak_ptr<EnemyCharacter>> GameplayStatics::_enemies;
vector<weak_ptr<PlayerCharacter>> GameplayStatics::_players;
stringstream GameplayStatics::_combat_log; 


void GameplayStatics::Initialize(vector<shared_ptr<PlayerCharacter>> players, SpellManager* spell_manager, CombatManager* combat_manager, MapGenerator* map_generator, ConsoleMenu* menu) {

	cout << fixed << setprecision(2);

	_player = players[0];
	_players.reserve(players.size());
	for (auto& p : players)
		_players.push_back(weak_ptr<PlayerCharacter>(p));

	for (auto& p : players)
		_player_characters.push_back(p.get());

	_spell_manager = spell_manager;
	_combat_manager = combat_manager;
	_map_generator = map_generator;
	_menu = menu;

	_map_generator->Initialize(_player_characters);
	//_map_generator->PrintDebugMap();

	DisplayMapMenu();
}

void GameplayStatics::GiveExperience(EnemyCharacter* enemy) {

	//zracunati

	int experience = 0;

	//_player->ReceiveExperience(experience);

}

void GameplayStatics::DisplayAllies() {

	system("cls");
	int idx = 0;
	for (auto& character : _player_characters)
		if (character->IsAlive()) {
			cout << ANSI_COLOR_VIVID_GREEN << GameplayStatics::GetEnumString(character->GetCharacterClass()) << " Level " << to_string(character->GetLevel()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_GREEN << string(1, character->GetAlias()) << ANSI_COLOR_RESET << ")";
			cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << character->GetHealth().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << character->GetArmor().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " MD: " << ANSI_COLOR_VIVID_YELLOW << character->GetDmgMelee().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " RD: " << ANSI_COLOR_VIVID_YELLOW << character->GetDmgRanged().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << character->GetCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << character->GetCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << character->GetSpellCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << character->GetSpellCrtDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << character->GetResistances().GetArcaneRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << character->GetResistances().GetFireRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << character->GetResistances().GetLightningRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << character->GetResistances().GetColdRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << character->GetResistances().GetPoisonRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << character->GetResistances().GetNecroticRes() << endl;
			//cout << ANSI_COLOR_CYAN_LIGHT << ""
			++idx;
		}
}

void GameplayStatics::DisplayEnemies() {
	
	int idx = 0;
	for (auto& enemy : _enemies) {
		if (!enemy.expired()) {
			cout << ANSI_COLOR_RED << GameplayStatics::GetEnumString(enemy.lock()->GetCharacterClass()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_RED << string(1, enemy.lock()->GetAlias()) << ANSI_COLOR_RESET << ")";
			cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetHealth().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetArmor().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " MD: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetDmgMelee().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " RD: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetDmgRanged().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetSpellCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetSpellCrtDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetArcaneRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetFireRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetLightningRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetColdRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetPoisonRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetNecroticRes() << endl;
			++idx;
		}
	}
}

void GameplayStatics::InitiateCombatMode(vector<weak_ptr<EnemyCharacter>> enemies) {

	_player.lock()->SetIsInCombat(true);

	_enemies = enemies;

	for (auto& enemy : _enemies)
		_enemy_characters.push_back(enemy.lock().get());

	_combat_manager->SetTurns(_players, _enemies);
	
	while (_player.lock()->IsInCombat()) {
		DestroyDeadCharacters();
		if (!(all_of(_enemies.begin(), _enemies.end(), [](const weak_ptr<EnemyCharacter>& wptr) { return wptr.expired(); })))
			_combat_manager->GetTurnCharacter().lock()->TakeTurn();
	}

	_combat_manager->ResetCombatVariables();
	ResetCombatVariables();

	system("cls");
}

void GameplayStatics::ExitCombatMode() {
	_player.lock()->SetIsInCombat(false);
	//_combat_manager->GetTurnCharacter().lock()->EndTurn();
}

void GameplayStatics::ResetCombatVariables() {
	_enemy_characters.clear();
	_enemies.clear();
	auto& s = GetCombatLogStream();
	s.clear();
	s.str("");
}

void GameplayStatics::DisplayMapMenu() {
	cout << ANSI_COLOR_VIVID_YELLOW;
	cout << "========     MAP MENU     ========" << endl;
	cout << "==================================" << endl;
	cout << ANSI_COLOR_RESET;

	vector<string> v = { "SHOW POSITION", "SHOW MAP", "MOVE" };
	int input = InteractiveDisplay(v);
	HandleMapInput(input);
}

int GameplayStatics::InteractiveDisplay(const vector<string>& options, const int right, const bool clear) {
	_menu->SetOptions(options);
	_menu->SetUp(static_cast<int>(options.size()));
	_menu->SetRight(right);
	_menu->SetClear(clear);
	return _menu->Select();
}

void GameplayStatics::ANSI_CURSOR_DOWN_N(int n) {
	_menu->ANSI_CURSOR_DOWN_N(n);
}

void GameplayStatics::HandleMapInput(int input) {

	switch (input) {
	case 0:
		ShowPosition();
		break;
	case 1:
		ShowMap();
		break;
	case 2:
		HandleMovement();
		break;
	default:
		break;
	}
}

void GameplayStatics::RedrawGameScreen() {
	DisplayAllies();
	DisplayEnemies();
	DisplayGrid();
	_combat_manager->DisplayTurnOrder();
	DisplayCombatLog();
}

void GameplayStatics::DisplayGrid() {
	_map_generator->DisplayGrid();
}

// Mozda meknuti te funkcije i zvati ih direktno z switcha??
//-------------------------------------------------------------
void GameplayStatics::ShowPosition() {
	_map_generator->ShowPosition();
}
void GameplayStatics::ShowMap() { 
	_map_generator->ShowMap();
}
void GameplayStatics::HandleMovement() {
	_map_generator->HandleMovement();
}
//-------------------------------------------------------------

int GameplayStatics::DisplayCombatMenu() {

	cout << ANSI_COLOR_CYAN_LIGHT;
	cout << "=> COMBAT MENU <=" << endl;
	cout << "=================" << endl;
	cout << ANSI_COLOR_RESET;
	vector<string> v = { "CAST SPELL", "MOVE", "END TURN", "INFO"};
	return InteractiveDisplay(v);
}

void GameplayStatics::HandleCombatInput(PlayerCharacter* character, int input) {

	switch (input) {
	case 0:
		DisplaySpellMenu();
		break;
	case 1:
		CombatMove();
		break;
	case 2:
		EndTurn(character);
		break;
	case 3:
		DisplayInfoMenu();
		break;
	default:
		break;
	}
}

void GameplayStatics::CombatMove() {
	map<int, EDirection> direction_map;
	vector<string> v = _map_generator->GetCombatDirections(_combat_manager->GetTurnCharacter().lock().get(), direction_map);
	int input = InteractiveDisplay(v);

	MoveEnemyCharacterOnGrid(_combat_manager->GetTurnCharacter().lock().get(), direction_map[input]);
}

void GameplayStatics::EnemyCombatMove(Character* character, map<int, EDirection>& direction_map) {
	vector<string> v = _map_generator->GetCombatDirections(character, direction_map);
}

void GameplayStatics::MoveEnemyCharacterOnGrid(Character* character, EDirection direction) {
	_map_generator->MoveCharacterOnGrid(character, direction);
}

int GameplayStatics::GetPlayerIdx(char c) {
	int ret;
	while ((ret = _map_generator->GetPlayerIdx(c)) == -1) {
		_menu->Clear(2);
		cout << ANSI_COLOR_RED << "Wrong alias. Input target alias: " << ANSI_COLOR_RESET << endl;
		cout << "-> ";
		string s; cin >> s;
		c = s[0];
	}
	return ret;
}

int GameplayStatics::GetEnemyIdx(char c) {
	int ret;
	while ((ret = _map_generator->GetEnemyIdx(c)) == -1) {
		_menu->Clear(2);
		cout << ANSI_COLOR_RED << "Wrong alias. Input target alias: " << ANSI_COLOR_RESET << endl;
		cout << "-> ";
		string s; cin >> s;
		c = s[0];
	}
	return ret;
}


int GameplayStatics::GetEnemyIdx2(char c) {
	int ret;
	while ((ret = _map_generator->GetEnemyIdx(c)) == -1) {
		_menu->Clear(2);
		cout << ANSI_COLOR_RED << "Wrong alias. Input target alias: " << ANSI_COLOR_RESET << endl;
		cout << "->>>>>>> ";
		string s; cin >> s;
		c = s[0];
	}
	return ret;
}


void GameplayStatics::DisplaySpellMenu() {

	_menu->Clear(2);
	cout << ANSI_COLOR_CYAN_LIGHT;
	cout << "=> AVAILABLE SPELLS <=" << endl;
	cout << "======================" << endl;
	cout << ANSI_COLOR_RESET;

	int spell_idx;
	auto spell_type = ESpellType::NONE;
	int effect_idx = 0;
	HandleSpellAndEffectSelection(spell_idx, spell_type, effect_idx);

	HandleSpellTargets(spell_idx, spell_type, effect_idx);
}

void GameplayStatics::HandleSpellAndEffectSelection(OUT int& spell_idx, OUT ESpellType& spell_type, OUT int& effect_idx) {

	int length = 0;
	vector<Spell*> spells;
	do {
		if (!spells.empty()) _menu->Clear(4); // Hardcoded for now, TODO change
		spell_idx = DisplayEquipedSpells(length, spells);
		effect_idx = DisplaySelectedSpellsEffects(spell_idx, length, spells);
	} while (_menu->GetBack());

	//spell_input++;
	//for (auto& spell : _combat_manager->GetTurnCharacter()->GetSpells()) {
	//	if (--spell_input) continue;
	//	spell_id = spell.second->GetID();
	//	spell_type = spell.second->GetEffects()[spell_input]->GetSpellType();
	//	break;
	//}

	_menu->ANSI_CURSOR_DOWN_N(max(static_cast<int>(spells.size()), 4));
	_menu->Clear(max(static_cast<int>(spells.size()), 4));
}

void GameplayStatics::HandleSpellTargets(int spell_idx, ESpellType spell_type, int effect_idx) {

	//spell_Type reserved for future
	//aura might not need a target
	//several other types might be self-cast only thus not needing a target

	string s_input = "";
	int target_num = 1 + _combat_manager->GetTurnCharacter().lock()->GetMultiStrike();
	for (int i = 0; i < target_num; i++) {
		cout << "Input target alias:" << endl;
		cout << "-> ";
		cin >> s_input;
	}

	vector<int> p_idx;
	vector<int> e_idx;
	for (int i = 0; i < target_num; i++) {
		char c = s_input[i];
		if (c >= '0' && c <= '9') p_idx.push_back(GetPlayerIdx(c));
		else if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z') e_idx.push_back(GetEnemyIdx(c));
	}

	// This is current workaround for not throwing when wrong team alias is the input
	// Has to be fixed when we have better targeting system
	// In essence, this is not working as intended, just prevents throwing
	/////////////////////////////////////////////////////////////////////////////
	//if (p_idx.empty()) p_idx = e_idx;
	//if (e_idx.empty()) e_idx = p_idx;
	/////////////////////////////////////////////////////////////////////////////


	_spell_manager->CastSpell(spell_idx, _combat_manager->GetTurnCharacter().lock().get(), _players, _enemies, p_idx, e_idx, effect_idx);
}

int GameplayStatics::DisplayEquipedSpells(int& length, vector<Spell*>& spells) {
	vector<string> v;
	spells.clear();
	for (auto& spell : _combat_manager->GetTurnCharacter().lock()->GetSpells()) {
		v.push_back(GetEnumString(spell->GetID()));
		spells.push_back(spell.get());
		if (static_cast<int>(v.back().size()) > length) length = static_cast<int>(v.back().size());
	}
	return InteractiveDisplay(v, 0, false);
}

int GameplayStatics::DisplaySelectedSpellsEffects(int input, int length, vector<Spell*> spells) {
	vector<string> v = { "NO EFFECT" };
	const vector<pair<int, string>> _effect_list = spells[input]->GetEffectLevelNameVector();
	for (auto& effect : _effect_list)
		v.push_back(effect.second);

	return InteractiveDisplay(v, length + 2, false);
}

void GameplayStatics::DisplayInfoMenu() {
	_menu->Clear(2);
	cout << ANSI_COLOR_CYAN_LIGHT;
	cout << "=> INFO MENU <=" << endl;
	cout << "===============" << endl;
	cout << ANSI_COLOR_RESET;

	vector<string> v = { "SPELLS", "BACK" };
	int input = InteractiveDisplay(v);
	
	switch (input) {
		case 0: {
			int spell_idx;
			auto spell_type = ESpellType::NONE;
			int effect_idx = 0;
			HandleSpellAndEffectSelection(spell_idx, spell_type, effect_idx);
			HandleEffectInfo(spell_idx, spell_type, effect_idx);
		}
	}
}

void GameplayStatics::HandleEffectInfo(int spell_idx, ESpellType spell_type, int effect_idx) {
	vector<shared_ptr<ActiveEffect>> effects;
	auto spells = _combat_manager->GetTurnCharacter().lock()->GetSpells();
	for (int i = 0; i < spells.size(); i++) {
		if (spell_idx == i) {
			effects = spells[spell_idx]->GetEffects();
			stringstream& ss = effects[effect_idx]->GetTooltip();

			const int max_lines = 19;
			vector<string> lines;
			int start_index;
			ExtractLinesFromStringtream(lines, max_lines, ss, start_index);

			_menu->ClearRight(23);
			cout << CURSOR_LOG_LEFT << ANSI_COLOR_ORANGE;
			cout << CURSOR_LOG_RIGHT << "->  INFO MENU  <-" << endl;
			cout << CURSOR_LOG_RIGHT << "=================" << endl;
			for (int i = start_index; i < lines.size(); ++i) {
				cout << CURSOR_LOG_RIGHT << lines[i];
			}

			int input;
			cout << endl << CURSOR_LOG_RIGHT << COLOR_COMBAT_LOG << "Press enter to return";
			do {
				input = _getch();
			} while (input != '\r');
		}
	}
}

void GameplayStatics::HandleInfoInput(int input) {

}

void GameplayStatics::DisplayCombatLog() {

	cout << ANSI_CURSOR_UP(50);
	int e_size = static_cast<int>(count_if(_enemies.begin(), _enemies.end(), [](const weak_ptr<EnemyCharacter>& wptr) { return !wptr.expired(); }));
	_menu->ANSI_CURSOR_DOWN_N(static_cast<int>(_players.size() + e_size));
	cout << CURSOR_LOG_RIGHT << COLOR_COMBAT_LOG;
	cout << ANSI_COLOR_BLUE << "()()()   COMBAT LOG   ()()()" << endl << CURSOR_LOG_RIGHT;
	cout << ANSI_COLOR_BLUE << "()()()()()()()()()()()()()()" << ANSI_COLOR_RESET << endl;

	const int max_lines = 19;
	vector<string> lines;
	int start_index;
	ExtractLinesFromStringtream(lines, max_lines, _combat_log, start_index);

	for (int i = start_index; i < lines.size(); i++) {
		cout << CURSOR_LOG_RIGHT << lines[i];
	}

	MoveCursorToCombatLog();
}

void GameplayStatics::MoveCursorToCombatLog() {
	cout << ANSI_CURSOR_UP(50);
	int e_size = static_cast<int>(count_if(_enemies.begin(), _enemies.end(), [](const weak_ptr<EnemyCharacter>& wptr) { return !wptr.expired(); }));
	_menu->ANSI_CURSOR_DOWN_N(21 + e_size + static_cast<int>(_players.size()));
}

void GameplayStatics::ExtractLinesFromStringtream(OUT vector<string>& lines, const int max_lines, stringstream& ss, OUT int& start_index) {
	string content = ss.str();
	if (content == "") ss << fixed << setprecision(2);
	int start = 0;

	for (int i = 0; i < content.size(); ++i) {
		if (content[i] == '\n' || i == content.size() - 1) {
			string line = content.substr(start, i - start + 1);
			lines.push_back(line);
			start = i + 1;
		}
	}
	start_index = max(0, static_cast<int>(lines.size()) - max_lines);
}

vector<weak_ptr<PlayerCharacter>> GameplayStatics::GetPlayerCharacters() {
	//vector<PlayerCharacter*> v;
	//for (auto& player : _player_characters)
	//	if (player->IsAlive())
	//		v.push_back(player);
	//return v;
	return _players;
}

vector<weak_ptr<EnemyCharacter>> GameplayStatics::GetEnemyCharacters() {
	//vector<EnemyCharacter*> v;
	//for (auto& enemy : _enemy_characters)
	//	if (enemy->IsAlive())
	//		v.push_back(enemy);
	//return v;
	return _enemies;
}

void GameplayStatics::EndTurn(Character* character) {
	_combat_manager->EndTurn(character);
}

float GameplayStatics::ApplyDamage(Character* instigator, Character* target, float damage, shared_ptr<ActiveEffect> effect, bool isOnApply) {
	
	EEffectID effect_id = effect->GetID();
	EDamageType damage_type = effect->GetDamageType();
	ESpellID spell_id = effect->GetSpell()->GetID();

	damage = float2(damage);

	float actual_damage = Resistances::CalculateDamage(damage, damage_type, target);
	float resisted = float2(damage - actual_damage);

	auto& s = GetCombatLogStream();
	const string C = GetAliasColor(instigator->GetAlias());
	const string CT = GetAliasColor(target->GetAlias());
	if (isOnApply) s << CT << target->GetAlias() << COLOR_COMBAT_LOG << " suffers from " << COLOR_EFFECT << GetEnumString(spell_id) << COLOR_COMBAT_LOG << " for " << COLOR_VALUE << actual_damage * -1 << COLOR_COMBAT_LOG << " damage [resisted:" << COLOR_VALUE << resisted * -1 << COLOR_COMBAT_LOG << "]\n";
	else s << C << target->GetAlias() << COLOR_COMBAT_LOG << " suffers from " << COLOR_EFFECT << GetEnumString(effect_id) << COLOR_COMBAT_LOG << " for " << COLOR_VALUE << actual_damage * -1 << COLOR_COMBAT_LOG << " damage [resisted:" << COLOR_VALUE << resisted * -1 << COLOR_COMBAT_LOG << "]\n";
	return actual_damage;
}

void GameplayStatics::ApplyEffect(Character* instigator, vector<weak_ptr<Character>> targets, EffectParams& effect_params, OnApplyParams& apply_params, shared_ptr<ActiveEffect> effect, int effect_idx) {
	
	const Spell* spell = effect->GetSpell();
	EEffectID effect_id = effect->GetID();

	unique_ptr<CombatEffect> combat_effect = make_unique<CombatEffect>(instigator, targets, effect_params, apply_params, effect, spell->GetDuration(effect_idx, spell->GetLevel()));

	auto& s = GetCombatLogStream();
	const string C = GetAliasColor(instigator->GetAlias());
	if (!effect_idx) s << C << instigator->GetAlias() << COLOR_COMBAT_LOG << " Casts " << COLOR_EFFECT << GameplayStatics::GetEnumString(spell->GetID()) << COLOR_COMBAT_LOG << ".\n";
	if (effect_idx) s << C << instigator->GetAlias() << CC << " applies " << CEF << GameplayStatics::GetEnumString(effect_id) << CC << ".\n";
	_combat_manager->AddCombatEffect(move(combat_effect));
}

string GameplayStatics::GetAliasColor(char alias) {
	if (UPPER(alias) >= 'A' && UPPER(alias) <= 'Z') return COLOR_ENEMY;
	else return COLOR_PLAYER;
}

void GameplayStatics::DestroyDeadCharacters() {
	int idx = _combat_manager->GetDeadIdx();
	if (idx != -1) _map_generator->KillEnemy(idx);

	_combat_manager->RemoveDeadCharacters();
}

void GameplayStatics::KillEnemy(int idx) {
	_map_generator->KillEnemy(idx);
}

string GameplayStatics::string2(float f) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << f;
	return ss.str();
}

float GameplayStatics::float2(float f) {
	return round(f * 100) / 100;
}




































std::string GameplayStatics::GetEnumString(ESpellID _enum) {
	switch (_enum) {
	case ESpellID::NONE:
		return "NONE";
	case ESpellID::FIREBALL:
		return "FIREBALL";
	case ESpellID::STONESKIN:
		return "STONESKIN";
	case ESpellID::ARCANE_INFUSION:
		return "ARCANE INFUSION";
	case ESpellID::BLOOD_RAIN:
		return "BLOOD RAIN";
	case ESpellID::VISCOUS_ACID:
		return "VISCOUS ACID";
	default:
		return "error";
	}
}

std::string GameplayStatics::GetEnumString(EEffectID _enum) {
	switch (_enum) {
	case EEffectID::FIREBALL:
		return "FIREBALL EFFECT";
	case EEffectID::BURNING:
		return "BURNING";
	case EEffectID::MOLTEN_ARMOR:
		return "MOLTEN ARMOR";
	case EEffectID::EXPOSURE:
		return "EXPOSURE";

	case EEffectID::VISCOUS_ACID:
		return "VISCOUS ACID EFFECT";
	default:
		return "DEFAULT";
	}
}


std::string GameplayStatics::GetTypeString(int _enum) {
	switch (_enum) {
	case 0:
		return "NONE";
	case 1:
		return "FIREBALL";
	case 2:
		return "STONESKIN";
	default:
		return "INVALID";
	}
}

std::string GameplayStatics::GetEnumString(ESpellType _enum) {
	switch (_enum) {
	case ESpellType::NONE:
		return "NONE";
	case ESpellType::BUFF:
		return "BUFF";
	case ESpellType::DEBUFF:
		return "DEBUFF";
	case ESpellType::AURA:
		return "AURA";
	default:
		return "No type";
	}
}

std::string GameplayStatics::GetEnumString(ESpellActivity _enum) {
	switch (_enum) {
	case ESpellActivity::NONE:
		return "NONE";
	case ESpellActivity::PASSIVE:
		return "PASIVE";
	case ESpellActivity::ACTIVE:
		return "ACTIVE";
	default:
		return "No type";
	}
}

std::string GameplayStatics::GetEnumString(EDamageType _enum) {
	switch (_enum) {
	case EDamageType::NONE:
		return "NONE";
	case EDamageType::ARCANE:
		return "ARCANE";
	case EDamageType::FIRE:
		return "FIRE";
	case EDamageType::LIGHTNING:
		return "LIGHTNING";
	case EDamageType::COLD:
		return "COLD";
	case EDamageType::POISON:
		return "POISON";
	default:
		return "No type";
	}
}

std::string GameplayStatics::GetEnumString(ECharacterClass _enum) {

	if (static_cast<int>(_enum) >= 50) {
		int int_enum = static_cast<int>(_enum);
		int_enum -= 49;
		return "Enemy Level " + to_string(int_enum);
	}
	else {
		switch (_enum) {
		case ECharacterClass::BARBARIAN:
			return "BARBARIAN";
		case ECharacterClass::WARLOCK:
			return "WARLOCK";
			break;
		default:
			return "DEFAULT";
			break;
		}
	}
}