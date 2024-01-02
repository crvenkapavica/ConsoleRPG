#include "GameplayStatics.h"
#include "Characters/Character.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/SummonCharacter.h"
#include "Spells/SpellData.h"
#include "Spells/SpellManager.h"
#include "Spells/EffectStructs.h"
#include "Spells/ActiveSpell.h"
#include "Combat/CombatManager.h"
#include "MapGenerator/MapGenerator.h"
#include "Resistances.h"
#include "Items/Item.h"

weak_ptr<Character> GameplayStatics::_player;
vector<shared_ptr<Character>> GameplayStatics::_player_characters;
SpellManager* GameplayStatics::_sm = nullptr;
CombatManager* GameplayStatics::_cm = nullptr;
MapGenerator* GameplayStatics::_map_gen = nullptr;
ConsoleMenu* GameplayStatics::_menu = nullptr;
vector<weak_ptr<Character>> GameplayStatics::_enemies;
vector<weak_ptr<Character>> GameplayStatics::_players;
stringstream GameplayStatics::_combat_log; 


void GameplayStatics::Initialize(vector<shared_ptr<Character>>&& players, SpellManager& spell_manager, CombatManager& combat_manager, MapGenerator&& map_generator, ConsoleMenu& menu) {

	cout << fixed << setprecision(2);

	_player_characters = players;  // this is a vector of shared pointers of main player characters. this should never reset. if the player character dies, we employ some custom logic
								  // so it can be resurrected. if all player characters die in a combat, the player loses. Later we actually implement how to handle this.

	_player = players[0];
	for (const auto& player : _player_characters)
		_players.push_back(player);

	_sm = &spell_manager;
	_cm = &combat_manager;
	_map_gen = &map_generator;
	_menu = &menu;

	_map_gen->Initialize(_players);
	//_map_gen->PrintDebugMap();
	_map_gen->PrintDistance();

	DisplayMapMenu();
}

void GameplayStatics::DisplayAllies() {

	system("cls");
	int idx = 0;
	for (const auto& character : _cm->GetPlayers())
		if (!character.expired()) {
			cout << ANSI_COLOR_VIVID_GREEN << GameplayStatics::GetEnumString(character.lock()->GetCharacterClass()) << " Level " << to_string(character.lock()->GetLevel()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_GREEN << string(1, character.lock()->GetAlias()) << ANSI_COLOR_RESET << ")";
			cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetHealth().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetArmor().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " AP: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetAP().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SP: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetSP().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetSpellCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetSpellCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetResistances().GetArcaneRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetResistances().GetFireRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetResistances().GetLightningRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetResistances().GetColdRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetResistances().GetPoisonRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << character.lock()->GetResistances().GetNecroticRes() << endl;
			//cout << ANSI_COLOR_CYAN_LIGHT << ""
			++idx;
		}

	idx = 0;
	for (const auto& summon : _cm->GetSummons())
		if (summon && summon->GetTeam() == 1) {
			cout << ANSI_COLOR_VIVID_GREEN << GameplayStatics::GetEnumString(summon->GetCharacterClass()) << " Level " << to_string(summon->GetLevel()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_GREEN << string(1, summon->GetAlias()) << ANSI_COLOR_RESET << ")";
			cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << summon->GetHealth().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << summon->GetArmor().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " AP: " << ANSI_COLOR_VIVID_YELLOW << summon->GetAP().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << summon->GetCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << summon->GetCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SP: " << ANSI_COLOR_VIVID_YELLOW << summon->GetSP().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << summon->GetSpellCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << summon->GetSpellCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetArcaneRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetFireRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetLightningRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetColdRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetPoisonRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetNecroticRes() << endl;
			//cout << ANSI_COLOR_CYAN_LIGHT << ""
			++idx;
		}
}

void GameplayStatics::DisplayEnemies() {
	
	int idx = 0;
	for (const auto& enemy : _cm->GetEnemies()) {
		if (!enemy.expired()) {
			cout << ANSI_COLOR_RED << GameplayStatics::GetEnumString(enemy.lock()->GetCharacterClass()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_RED << string(1, enemy.lock()->GetAlias()) << ANSI_COLOR_RESET << ")";
			cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetHealth().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetArmor().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " AP: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetAP().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SP: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetSP().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetSpellCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetSpellCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetArcaneRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetFireRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetLightningRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetColdRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetPoisonRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << enemy.lock()->GetResistances().GetNecroticRes() << endl;
			++idx;
		}
	}

	idx = 0;
	for (const auto& summon : _cm->GetSummons())
		if (summon && summon->GetTeam() == 2) {
			cout << ANSI_COLOR_RED << GameplayStatics::GetEnumString(summon->GetCharacterClass()) << " Level " << to_string(summon->GetLevel()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_RED << string(1, summon->GetAlias()) << ANSI_COLOR_RESET << ")";
			cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << summon->GetHealth().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << summon->GetArmor().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " AP: " << ANSI_COLOR_VIVID_YELLOW << summon->GetAP().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << summon->GetCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << summon->GetCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SP: " << ANSI_COLOR_VIVID_YELLOW << summon->GetSP().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << summon->GetSpellCritChance().GetActual() * 100 << "%";
			cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << summon->GetSpellCritDmg().GetActual();
			cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetArcaneRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetFireRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetLightningRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetColdRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetPoisonRes();
			cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << summon->GetResistances().GetNecroticRes() << endl;
			//cout << ANSI_COLOR_CYAN_LIGHT << ""
			++idx;
		}
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

void GameplayStatics::DisplayMapMenuTitle() {
	cout << ANSI_COLOR_VIVID_YELLOW;
	cout << "========     MAP MENU     ========" << endl;
	cout << "==================================" << endl;
	cout << ANSI_COLOR_RESET;
}

void GameplayStatics::DisplayCombatMenuTitle() {
	cout << ANSI_COLOR_CYAN_LIGHT;
	cout << "=> COMBAT MENU <=" << endl;
	cout << "=================" << endl;
	cout << ANSI_COLOR_RESET;
}

void GameplayStatics::DisplaySpellMenuTitle() {
	_menu->Clear(2);
	cout << ANSI_COLOR_CYAN_LIGHT;
	cout << "=> AVAILABLE SPELLS <=" << endl;
	cout << "======================" << endl;
	cout << ANSI_COLOR_RESET;
}

void GameplayStatics::DisplayMapMenu() {
	DisplayMapMenuTitle();

	// MAP LOOP
	while (_player.lock()->IsAlive()) {
		vector<string> v = { "SHOW POSITION", "SHOW MAP", "MOVE", "ITEMS", "SPELLS", "STATS"};
		int input = InteractiveDisplay(v);
		HandleMapInput(input);
	}
}

void GameplayStatics::HandleMapInput(int input) {

	switch (input) {
	case 0:
		_map_gen->ShowPosition();
		break;
	case 1:
		_map_gen->ShowMap();
		break;
	case 2:
		_map_gen->HandleMovement();
		break;
	case 3:
		DisplayItemMenu();
		break;
	//case 4:
	//	DisplaySpellMenu(); // treba drugu funkciju -> funkciju koja pokazuje v = {ACTIVE, PASSIVE, ALL(instanced)};
	case 5:
		DisplayPlayerStats();
	default:
		break;
	}
}

PlayerCharacter* GameplayStatics::GetPlayer() {
	vector<string> v;
	for (const auto& p : _players)
		v.push_back(GameplayStatics::GetEnumString(p.lock()->GetCharacterClass()));
	v.push_back("<--BACK--<");
	int input;
	if ((input = InteractiveDisplay(v)) == -1) return nullptr;
	return dynamic_cast<PlayerCharacter*>(_players[input].lock().get());
}

void GameplayStatics::DisplayItemMenu() {
	PlayerCharacter* player;
	if ((player = GetPlayer()) == nullptr) return;

	bool bIsEquiped = false;
	unique_ptr<Item> item;
	if (!(item = player->DisplayAllItems(bIsEquiped))) return;
	
	vector<string> v;
	int input;
	if (bIsEquiped) {
		v = { "UN-EQUIP", "DESTROY", "<--BACK--<" };
		if ((input = InteractiveDisplay(v)) == -1) {
			player->EquipItem(move(item));
			return;
		}
		if (input == 0) player->UnEquipItem(move(item));
		else player->DestroyItem(move(item));
	}
	else {
		v = { "EQUIP", "DESTROY", "<--BACK--<" };
		if ((input = InteractiveDisplay(v)) == -1) {
			player->AddItemToInventory(move(item));
			return;
		}
		if (input == 0) player->EquipItem(move(item));
		else player->DestroyItem(move(item));
	}
}

//void GameplayStatics::DisplaySomePlayerSpellMenu() {}

void GameplayStatics::DisplayPlayerStats() {
	PlayerCharacter* player = GetPlayer();
	player->DisplayStats();
}

void GameplayStatics::RedrawGameScreen() {
	DisplayAllies();
	DisplayEnemies();
	_map_gen->DisplayGrid();
	_cm->DisplayTurnOrder();
	DisplayCombatLog();
}


void GameplayStatics::InitiateCombatMode(vector<weak_ptr<Character>>&& enemies) {

	_player.lock()->SetIsInCombat(true);

	_enemies = enemies;

	_cm->SetTurns(_players, _enemies);
	_cm->StartCombat(_player);


	//cout << "STR: " << _player.lock()->GetPlayerAttributes()._strength << "  BASE CRIT DMG: " << _player.lock()->GetCritDmg().GetBase() << "  ACTUAL CRIT DMG: " << _player.lock()->GetCritDmg().GetActual() << endl;
	//cout << "AGI: " << _player.lock()->GetPlayerAttributes()._agility << "  BASE ARMOR: " << _player.lock()->GetArmor().GetBase() << "  ACTUAL ARMOR: " << _player.lock()->GetArmor().GetActual() << endl;
	//cout << "INT: " << _player.lock()->GetPlayerAttributes()._intelligence << "  BASE ESSENCE: " << _player.lock()->GetEssence().GetBase() << "  ACTUAL ESSENCE: " << _player.lock()->GetEssence().GetActual() << "SpellBook Crit %: " << _player.lock()->GetSpellCritChance().GetBase() << endl;
	//cout << "VIT: " << _player.lock()->GetPlayerAttributes()._vitality << "  BASE HP: " << _player.lock()->GetHealth().GetBase() << "  ACTUAL HP: " << _player.lock()->GetHealth().GetActual() << endl;

	//int x; cin >> x;



	ResetCombatVariables();
	system("cls");

	//Give / Handle Combat(player) Experience
	//GenerateLoot
	//Add loot to inventory
	//DIsplay message how much xp has been gained and which items received
	//Offer an option to inspect and equip/uneqip items
	//Continue to map


			// OPTIONALLY
				// -> try to move Map Loop from MapGenerator to GameplayStatics, if that makes sense.

	//std::vector<unique_ptr<Item>> items = Item::GenerateLoot(_player, 195);
	//for (auto& item : items) {
	//	cout << item->_item_info._name << "\n";
	//	cout << GetEnumString(item->_item_info._item_slot) << "\t" << GetEnumString(item->_item_info._item_rarity) << "\t" << GetEnumString(item->_item_info._item_type) << "\t" << GetEnumString(item->_item_info._weapon_type) << "\n";
	//	cout << "Dmg min: " << item->_item_info._dmg_min << "\tDmg max: " << item->_item_info._dmg_max << "\tArmor: " << item->_item_info._armor << "\tIlvl: " << item->_item_info._lvl << "\n\n";
	//}
	//int x; cin >> x;
}

void GameplayStatics::ResetCombatVariables() {
	_enemies.clear();
	auto& s = GetCombatLogStream();
	s.clear();
	s.str("");
}

void GameplayStatics::GiveExperience(EnemyCharacter* enemy) {
	//zracunati
	int experience = 0;
	//_player->ReceiveExperience(experience);
}

int GameplayStatics::DisplayCombatMenu(Character* character) {
	DisplayCombatMenuTitle();
	vector<string> v;

	if (dynamic_cast<PlayerCharacter*>(character)) {
		v = { "MELEE ATTACK", "RANGED ATTACK", "CAST SPELL", "MOVE", "INFO", "END TURN" };
		return InteractiveDisplay(v);
	}
	else if (dynamic_cast<SummonCharacter*>(character)) {
		SummonCharacter* summon = static_cast<SummonCharacter*>(character);
		switch (summon->GetClass()) {
		case ECharacterClass::FIRE_ELEMENTAL:
		case ECharacterClass::WATER_ELEMENTAL:
		case ECharacterClass::ARCANE_ELEMENTAL:
			v = { "CAST SPELL", "MOVE", "END TURN" };
			return InteractiveDisplay(v);
		default:
			break;
		}
	}
	return -1;
}

void GameplayStatics::HandleCombatInput(PlayerCharacter* character, int input) {

	switch (input) {
	case 0:
		DisplayMeleeMenu();
		break;
	case 1:
		DisplayRangedMenu();
		break;
	case 2:
		DisplaySpellMenu();
		break;
	case 3:
		CombatMove();
		break;
	case 4:
		DisplayInfoMenu();
		break;
	case 5:
		EndTurn(character);
		break;
	default:
		break;
	}
}

void GameplayStatics::HandleCombatInput(SummonCharacter* character, int input) {
	switch (input) {
	case 0:
		DisplaySpellMenu();
		break;
	case 1:
		//Move();
		break;
	case 2:
		EndTurn(character);
		break;
	default:
		break;
	}
}

void GameplayStatics::CombatMove() {
	map<int, EDirection> direction_map;
	vector<string> v = _map_gen->GetCombatDirections(_cm->GetTurnCharacter().lock().get(), direction_map);
	v.push_back("<--BACK--<");

	int input = InteractiveDisplay(v);
	if (input == -1) return;

	_map_gen->MoveCharacterOnGrid(_cm->GetTurnCharacter().lock().get(), direction_map[input]);
}

void GameplayStatics::EnemyCombatMove(Character* character, OUT map<int, EDirection>& direction_map) {
	vector<string> v = _map_gen->GetCombatDirections(character, direction_map);
}

void GameplayStatics::MoveCharacterOnGrid(Character* character, EDirection direction) {
	_map_gen->MoveCharacterOnGrid(character, direction);
}

int GameplayStatics::GetPlayerIdx(char c) {
	int ret;
	while ((ret = _map_gen->GetPlayerIdx(c)) == -1) {
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
	while ((ret = _map_gen->GetEnemyIdx(c)) == -1) {
		_menu->Clear(2);
		cout << ANSI_COLOR_RED << "Wrong alias. Input target alias: " << ANSI_COLOR_RESET << endl;
		cout << "-> ";
		string s; cin >> s;
		c = s[0];
	}
	return ret;
}

int GameplayStatics::GetSpellIdx(ActiveSpell* spell, shared_ptr<Character>& character) {
	character = _cm->GetTurnCharacter().lock();
	for (int i = 0; i < character->GetActiveSpells().size(); i++) {
		if (spell == character->GetActiveSpells()[i].get()) {
			return i;
		}
	}
	RPG_ASSERT(!character, "GetSpellIdx");
	throw std::invalid_argument("Invalid Spell");
}

void GameplayStatics::DisplayMeleeMenu() {
	vector<string> v;
	vector<ActiveSpell*> spells;
	for (const auto& spell : _cm->GetTurnCharacter().lock()->GetActiveSpells())
		if (spell->GetClass() == ESpellClass::MELEE) {
			v.push_back(GetEnumString(spell->GetID()));
			spells.push_back(spell.get());
		}
	v.push_back("<--BACK--<");

	int input;
	if ((input = InteractiveDisplay(v)) == -1) return;

	HandleMeleeTarget(spells[input]);
}

void GameplayStatics::DisplayRangedMenu() {
	vector<string> v;
	vector<ActiveSpell*> spells;
	for (const auto& spell : _cm->GetTurnCharacter().lock()->GetActiveSpells())
		if (spell->GetClass() == ESpellClass::RANGED) {
			v.push_back(GetEnumString(spell->GetID()));
			spells.push_back(spell.get());
		}
	v.push_back("<--BACK--<");

	int input; 
	if ((input = InteractiveDisplay(v)) == -1) return;

	HandleTarget(spells[input]);
}


void GameplayStatics::DisplaySpellMenu() {
	DisplaySpellMenuTitle();

	vector<string> v;
	vector<ActiveSpell*> spells;
	for (auto& spell : _cm->GetTurnCharacter().lock()->GetActiveSpells()) {
		if (spell->GetClass() == ESpellClass::MAGIC) {
			v.push_back(GetEnumString(spell->GetID()));
			spells.push_back(spell.get());
		}
	}
	v.push_back("<--BACK--<");

	int input;
	if ((input = InteractiveDisplay(v)) == -1) return;

	HandleTarget(spells[input]);
}

void GameplayStatics::HandleTarget(ActiveSpell* spell) {

	string s_input;
	int n_targets = 0;

	if (spell->GetSpellType() != ESpellType::SUMMON) {
		n_targets = 1;
		for (int i = 0; i < n_targets; i++) {
			cout << "Input target alias:" << endl;
			cout << "-> ";
			cin >> s_input;
		}
	}

	vector<int> p_idx;
	vector<int> e_idx;
	for (int i = 0; i < n_targets; i++) {
		char c = s_input[i];
		if (c >= '0' && c <= '9') p_idx.push_back(GetPlayerIdx(c));
		else if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z') e_idx.push_back(GetEnemyIdx(c));
	}
	
	vector<weak_ptr<Character>> targets;
	if (e_idx.size() == 0)
		for (int i = 0; i < p_idx.size(); i++)
			targets.push_back(_players[p_idx[i]]);
	else
		for (int i = 0; i < e_idx.size(); i++)
			targets.push_back(_enemies[e_idx[i]]);

	shared_ptr<Character> character;
	int spell_idx = GetSpellIdx(spell, character);
	_sm->CastSpell(spell_idx, character, targets);
}

void GameplayStatics::HandleMeleeTarget(ActiveSpell* spell) {
	shared_ptr<Character> character;
	int spell_idx = GetSpellIdx(spell, character);

	vector<Character*> characters = _map_gen->GetCharactersInRange(_cm->GetTurnCharacter().lock().get());

	vector<string> alias;
	for (const auto& c : characters)
		if (c) alias.push_back(string(1, (c->GetAlias())));
	alias.push_back("<--BACK--<");

	int input;
	if ((input = InteractiveDisplay(alias)) == -1) return;

	int enemy_idx = GetEnemyIdx(alias[input][0]);
	vector<weak_ptr<Character>> targets = { _enemies[enemy_idx] };
	_sm->CastSpell(spell_idx, character, targets);
}

void GameplayStatics::DisplayInfoMenu() {
	_menu->Clear(2);
	cout << ANSI_COLOR_CYAN_LIGHT;
	cout << "=> INFO MENU <=" << endl;
	cout << "===============" << endl;
	cout << ANSI_COLOR_RESET;

	vector<string> v = { "SPELLS", "BACK" };
	int input = InteractiveDisplay(v);
}

void GameplayStatics::HandleInfoInput(int input) {

}

//void GameplayStatics::HandleEffectInfo(int spell_idx, ESpellType spell_type, int effect_idx) {
//	vector<shared_ptr<ActiveSpell>> effects;
//	auto spellbooks = _cm->GetTurnCharacter().lock()->GetActiveSpells();
//	for (int i = 0; i < spellbooks.size(); i++) {
//		if (spell_idx == i) {
//			effects = spellbooks[spell_idx]->GetEffects();
//			stringstream& ss = effects[effect_idx]->GetTooltip();
//
//			const int max_lines = 19;
//			vector<string> lines;
//			int start_index;
//			ExtractLinesFromStringstream(lines, max_lines, ss, start_index);
//
//			_menu->ClearRight(23);
//			cout << CURSOR_LOG_LEFT << ANSI_COLOR_ORANGE;
//			cout << CURSOR_LOG_RIGHT << "->  INFO MENU  <-" << endl;
//			cout << CURSOR_LOG_RIGHT << "=================" << endl;
//			for (int i = start_index; i < lines.size(); ++i) {
//				cout << CURSOR_LOG_RIGHT << lines[i];
//			}
//
//			int input;
//			cout << endl << CURSOR_LOG_RIGHT << COLOR_COMBAT_LOG << "Press enter to return";
//			do {
//				input = _getch();
//			} while (input != '\r');
//		}
//	}
//}

void GameplayStatics::DisplayCombatLog() {

	cout << ANSI_CURSOR_UP(50);
	int e_size = static_cast<int>(std::count_if(_enemies.begin(), _enemies.end(), [](const std::weak_ptr<Character>& wptr) { return !wptr.expired(); }));
	int s_size = static_cast<int>(_cm->GetSummons().size());
	_menu->ANSI_CURSOR_DOWN_N(static_cast<int>(_players.size() + e_size + s_size));
	cout << CURSOR_LOG_RIGHT << COLOR_COMBAT_LOG;
	cout << ANSI_COLOR_BLUE << "()()()   COMBAT LOG   ()()()" << "\n" << CURSOR_LOG_RIGHT;
	cout << ANSI_COLOR_BLUE << "()()()()()()()()()()()()()()" << ANSI_COLOR_RESET << "\n";

	const int max_lines = 49;
	std::vector<string> lines;
	int start_index;
	ExtractLinesFromStringstream(lines, max_lines, _combat_log, start_index);

	for (int i = start_index; i < lines.size(); i++) {
		std::cout << CURSOR_LOG_RIGHT << lines[i];
	}

	// Move Menu below grid
	std::cout << ANSI_CURSOR_UP(50);
	_menu->ANSI_CURSOR_DOWN_N(21 + e_size + s_size + static_cast<int>(_players.size()));
}

void GameplayStatics::ExtractLinesFromStringstream(OUT std::vector<string>& lines, const int max_lines, stringstream& ss, OUT int& start_index) {
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

std::weak_ptr<Character> GameplayStatics::GetWeakCharacter(Character* character) {
	if (character->GetTeam() == 1) {
		for (const auto& player : _players)
			if (player.lock()->GetAlias() == character->GetAlias())
				return player;
	}
	else {
		for (const auto& enemy : _enemies)
			if (enemy.lock()->GetAlias() == character->GetAlias())
				return enemy;
	}
	return weak_ptr<Character>();
}

std::shared_ptr<Character> GameplayStatics::GetSharedCharacter(Character* character) {
	if (character->GetTeam() == 1) {
		for (const auto& player : _players) {
			auto shared = player.lock();
			if (shared && shared->GetAlias() == character->GetAlias())
				return shared;
		}
	}
	else {
		for (const auto& enemy : _enemies) {
			auto shared = enemy.lock();
			if (shared && shared->GetAlias() == character->GetAlias())
				return shared;
		}
	}
	return nullptr;
}

std::vector<weak_ptr<Character>> GameplayStatics::GetPlayerCharacters() {
	return _players;
}

std::vector<weak_ptr<Character>> GameplayStatics::GetEnemyCharacters() {
	return _enemies;
}

float GameplayStatics::ApplyDamage(weak_ptr<Character> instigator, Character* target, float damage, unique_ptr<ActiveSpell>& spell, bool isOnApply) {
	RPG_ASSERT(!instigator.expired(), "ApplyDamage");

	damage = float2(damage);
	float actual_damage;
	if (spell->GetDamageType() != EDamageType::PURE)
		actual_damage = Resistances::CalculateDamage(damage, spell->GetDamageType(), target);
	else actual_damage = damage;
	float resisted = float2(damage - actual_damage);   // resisted / armor blocked - znaci change i u combat logu...ovisno o damage type..

	// POGLEDATI ZAKAJ TU IMA IKAKSE VEZE BOOLEAN
	//==============================================
	auto& s = GetCombatLogStream();
	const string C = GetAliasColor(instigator.lock().get()->GetAlias());
	const string CT = GetAliasColor(target->GetAlias());
	if (isOnApply) s << CT << target->GetAlias() << COLOR_COMBAT_LOG << " suffers from " << COLOR_EFFECT << GetEnumString(spell->GetID()) << COLOR_COMBAT_LOG << " for " << COLOR_VALUE << actual_damage * -1 << COLOR_COMBAT_LOG << " damage [resisted:" << COLOR_VALUE << resisted * -1 << COLOR_COMBAT_LOG << "]\n";
	else s << C << target->GetAlias() << COLOR_COMBAT_LOG << " suffers from " << COLOR_EFFECT << GetEnumString(spell->GetID()) << COLOR_COMBAT_LOG << " for " << COLOR_VALUE << actual_damage * -1 << COLOR_COMBAT_LOG << " damage [resisted:" << COLOR_VALUE << resisted * -1 << COLOR_COMBAT_LOG << "]\n";
	//==============================================

	return actual_damage;
}

void GameplayStatics::ApplyEffect(std::shared_ptr<Character>& instigator, std::vector<weak_ptr<Character>>& targets, std::unique_ptr<ActiveSpell> spell, std::optional<ApplyParams> apply_params, std::optional<EffectParams> effect_params) {
	const string C = GetAliasColor(instigator->GetAlias());
	auto& s = GetCombatLogStream();
	s << C << instigator->GetAlias() << COLOR_COMBAT_LOG << " Casts " << COLOR_EFFECT << GameplayStatics::GetEnumString(spell->GetID()) << COLOR_COMBAT_LOG << ".\n";

	std::shared_ptr<CombatEffect> effect = std::make_shared<CombatEffect>(move(instigator), targets, spell, apply_params, effect_params, SpellDB::_data[spell->GetID()][spell->GetLvl()]._duration);
	_cm->AddCombatEffect(std::move(effect));
}

void GameplayStatics::KillEnemy(int idx) {
	_map_gen->KillEnemy(idx);
}

void GameplayStatics::EndTurn(Character* character) {
	_cm->EndTurn(character);
}


//// REMOVE AFTER MAKING MAP_GEN A SINGLETON


bool GameplayStatics::AddCharacterToCharGrid(const shared_ptr<Character>& instigator, std::weak_ptr<Character> summon) {
	return _map_gen->AddCharacterToCharGrid(instigator, summon);
}



////////////////////////////////////////////////



string GameplayStatics::GetAliasColor(char alias) {
	if (UPPER(alias) >= 'A' && UPPER(alias) <= 'Z') return COLOR_ENEMY;
	else return COLOR_PLAYER;
}

string GameplayStatics::string2(float f) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << f;
	return ss.str();
}

float GameplayStatics::float2(float f) {
	return round(f * 100) / 100;
}

int GameplayStatics::GetRandInt(int a, int b) {
	static std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<int> distribution(a, b);
	return distribution(generator);
}

float GameplayStatics::GetRandFloat(float a, float b) {
	static std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<float> distribution(a, b);
	return float2(distribution(generator));
}

std::string GameplayStatics::GetEnumString(ESpellID _enum) {
	switch (_enum) {
	case ESpellID::FIREBALL:
		return "FIREBALL";
	case ESpellID::BURNING:
		return "BURNING";
	case ESpellID::MOLTEN_ARMOR:
		return "MOLTEN ARMOR";
	case ESpellID::EXPOSURE:
		return "EXPOSURE";
	case ESpellID::VISCOUS_ACID:
		return "VISCOUS ACID";
	case ESpellID::BLIND:
		return "BLIND";
	case ESpellID::SUM_FIRE_ELE:
		return "SUMMON FIRE ELEMENTAL";
	case ESpellID::MELEE:
		return "MELEE ATTACK";
	case ESpellID::RANGED:
		return "RANGED ATTACK";
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


	switch (_enum) {
	case ECharacterClass::BARBARIAN:
		return "BARBARIAN";
	case ECharacterClass::WARLOCK:
		return "WARLOCK";
		break;

	case ECharacterClass::E_GHOUL:
		return "GHOUL";
		break;
	case ECharacterClass::E_ARCHER:
		return "ARCHER";
		break;
	case ECharacterClass::E_MAGE:
		return "MAGE";
		break;
	case ECharacterClass::E_WARLOCK:
		return "WARLOCK";
		break;
	case ECharacterClass::E_SWORDSMAN:
		return "SWORDSMAN";
		break;
	case ECharacterClass::E_FIRE_IMP:
		return "FIRE IMP";
		break;

	default:
		return "DEFAULT CLASS";
		break;
	}
}

std::string GameplayStatics::GetEnumString(EItemSlot _enum) {
	switch (_enum) {
	case EItemSlot::NONE:
		return "None";
	case EItemSlot::HEAD:
		return "HEAD";
	case EItemSlot::CHEST:
		return "CHEST";
	case EItemSlot::HANDS:
		return "HANDS";
	case EItemSlot::BELT:
		return "BELT";
	case EItemSlot::LEGS:
		return "LEGS";
	case EItemSlot::FEET:
		return "FEET";
	case EItemSlot::NECK:
		return "NECK";
	case EItemSlot::FINGER1:
	case EItemSlot::FINGER2:
		return "FINGER";
	case EItemSlot::WPN_MAIN:
		return "MAIN-HAND";
	case EItemSlot::WPN_OFF:
		return "OFF-HAND";
	case EItemSlot::RELIC:
		return "RELIC";
	default:
		return "";
	}
}

std::string GameplayStatics::GetEnumString(EWeaponType _enum) {
	switch (_enum) {
	case EWeaponType::NONE:
		return "None";
	case EWeaponType::STAFF:
		return "Staff";
	case EWeaponType::BOW:
		return "Bow";
	case EWeaponType::AXE_2H:
		return "2H Axe";
	case EWeaponType::MACE_2H:
		return "2H Mace";
	case EWeaponType::SWORD_2H:
		return "2H Sword";
	case EWeaponType::AXE_1H:
		return "1H Axe";
	case EWeaponType::MACE_1H:
		return "1H Maxe";
	case EWeaponType::SWORD_1H:
		return "1H Sword";
	case EWeaponType::DAGGER:
		return "Dagger";
	case EWeaponType::SHIELD:
		return "Shield";
	default:
		return "";
	}
}

std::string GameplayStatics::GetEnumString(EItemRarity _enum) {
	switch (_enum) {
	case EItemRarity::COMMON:
		return "Common";
	case EItemRarity::RARE:
		return "Rare";
	case EItemRarity::EPIC:
		return "Epic";
	case EItemRarity::LEGENDARY:
		return "Legendary";
	case EItemRarity::GODLIKE:
		return "GODLIKE";
	case EItemRarity::UNIQUE:
		return "UNIQUE";
	default:
		return "";
	}
}

std::string GameplayStatics::GetEnumString(EItemType _enum) {
	switch (_enum) {
	case EItemType::CONSUMABLE:
		return "Consumable";
	case EItemType::SCROLL:
		return "Scroll";
	case EItemType::ARMOR:
		return "Armor";
	case EItemType::JEWLERY:
		return "Jewlery";
	case EItemType::WEAPON:
		return "Weapon";
	case EItemType::RELIC:
		return "Relic";
	default:
		return "";
	}
}