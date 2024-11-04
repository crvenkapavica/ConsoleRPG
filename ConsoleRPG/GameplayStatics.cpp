#include "GameplayStatics.h"

#include <algorithm>
#include <utility>
#include "Resistances.h"
#include "Characters/Character.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/SummonCharacter.h"
#include "Combat/CombatManager.h"
#include "Items/Item.h"
#include "MapGenerator/MapGenerator.h"
#include "Spells/ActiveSpell.h"
#include "Spells/EffectStructs.h"
#include "Spells/SpellData.h"
#include "Spells/SpellManager.h"

std::weak_ptr<Character> GameplayStatics::PlayerAvatar;
std::vector<std::shared_ptr<Character>> GameplayStatics::PlayerCharacters;
std::stringstream GameplayStatics::CombatLog; 
ConsoleMenu* GameplayStatics::DisplayMenu = nullptr;

#define MG MapGenerator::GetInstance()   // TODO : Check valid define (and replace)

// TODO : FIXME Initialization (WHERE) ?
void GameplayStatics::Initialize(std::vector<std::shared_ptr<Character>>&& InPlayerCharacters, ConsoleMenu& InMenu) {
	std::cout << std::fixed << std::setprecision(2);
	
	// This is a std::vector of shared pointers of main player characters. this should never reset. if the player character dies, we employ some custom logic
	// So it can be resurrected. If all player characters die in a combat, the player loses. Later we actually implement how to handle this.
	PlayerCharacters = std::move(InPlayerCharacters); 	// ALL play characters
	PlayerAvatar = PlayerCharacters[0];					// Main player character - avatar
	DisplayMenu = &InMenu;
	
	// for (const auto& PlayerChar : PlayerCharacters)
	// 	Players.push_back(PlayerChar);
	
	// MapGenerator mg = MapGenerator::GetInstance();
	//Initialize(PlayerCharacters);
	//MG.PrintDebugMap();
	//MG.PrintDistance();

	DisplayMapMenu();
}

void GameplayStatics::DisplayAllies() {
	CLS;
	int CharIndex = 0;
	std::cout << MG.GetPowerLevel();
	for (const auto& PlayerChar : CombatManager::GetPlayers())
		if (!PlayerChar.expired()) {
			std::cout << ANSI_COLOR_VIVID_GREEN << GameplayStatics::GetEnumString(PlayerChar.lock()->GetClass()) << " Level " << std::to_string(PlayerChar.lock()->GetLevel()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_GREEN << std::string(1, PlayerChar.lock()->GetAlias()) << ANSI_COLOR_RESET << ")";
			std::cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetHealth().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetArmor().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " AP: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetAP().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetCritChance().GetActual() * 100 << "%";
			std::cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetCritDmg().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SP: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetSP().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetSpellCritChance().GetActual() * 100 << "%";
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetSpellCritDmg().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetResistances().GetArcaneRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetResistances().GetFireRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetResistances().GetLightningRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetResistances().GetColdRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetResistances().GetPoisonRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << PlayerChar.lock()->GetResistances().GetNecroticRes() << '\n';
			//std::cout << ANSI_COLOR_CYAN_LIGHT << ""
			++CharIndex;
		}

	CharIndex = 0;
	for (const auto& Summon : CombatManager::GetSummons())
		if (Summon && Summon->GetTeam() == 1) {
			std::cout << ANSI_COLOR_VIVID_GREEN << GameplayStatics::GetEnumString(Summon->GetClass()) << " Level " << std::to_string(Summon->GetLevel()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_GREEN << std::string(1, Summon->GetAlias()) << ANSI_COLOR_RESET << ")";
			std::cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetHealth().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetArmor().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " AP: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetAP().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetCritChance().GetActual() * 100 << "%";
			std::cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetCritDmg().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SP: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetSP().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetSpellCritChance().GetActual() * 100 << "%";
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetSpellCritDmg().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetArcaneRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetFireRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetLightningRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetColdRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetPoisonRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetNecroticRes() << '\n';
			//std::cout << ANSI_COLOR_CYAN_LIGHT << ""
			++CharIndex;
		}
}

void GameplayStatics::DisplayEnemies() {
	int CharIndex = 0;
	for (const auto& EnemyChar : CombatManager::GetEnemies()) {
		if (!EnemyChar.expired()) {
			std::cout << ANSI_COLOR_RED << GameplayStatics::GetEnumString(EnemyChar.lock()->GetClass()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_RED << std::string(1, EnemyChar.lock()->GetAlias()) << ANSI_COLOR_RESET << ")";
			std::cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetHealth().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetArmor().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " AP: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetAP().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetCritChance().GetActual() * 100 << "%";
			std::cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetCritDmg().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SP: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetSP().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetSpellCritChance().GetActual() * 100 << "%";
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetSpellCritDmg().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetResistances().GetArcaneRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetResistances().GetFireRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetResistances().GetLightningRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetResistances().GetColdRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetResistances().GetPoisonRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << EnemyChar.lock()->GetResistances().GetNecroticRes() << '\n';
			++CharIndex;
		}
	}

	CharIndex = 0;
	for (const auto& Summon : CombatManager::GetSummons())
		if (Summon && Summon->GetTeam() == 2) {
			std::cout << ANSI_COLOR_RED << GameplayStatics::GetEnumString(Summon->GetClass()) << " Level " << std::to_string(Summon->GetLevel()) << ANSI_COLOR_RESET << " (" << ANSI_COLOR_RED << std::string(1, Summon->GetAlias()) << ANSI_COLOR_RESET << ")";
			std::cout << ANSI_COLOR_CYAN_LIGHT << "\tH: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetHealth().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " A: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetArmor().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " AP: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetAP().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " C%: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetCritChance().GetActual() * 100 << "%";
			std::cout << ANSI_COLOR_CYAN_LIGHT << " CD: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetCritDmg().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SP: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetSP().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SC%: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetSpellCritChance().GetActual() * 100 << "%";
			std::cout << ANSI_COLOR_CYAN_LIGHT << " SCD: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetSpellCritDmg().GetActual();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RA: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetArcaneRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RF: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetFireRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RL: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetLightningRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RC: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetColdRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RP: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetPoisonRes();
			std::cout << ANSI_COLOR_CYAN_LIGHT << " RN: " << ANSI_COLOR_VIVID_YELLOW << Summon->GetResistances().GetNecroticRes() << '\n';
			//std::cout << ANSI_COLOR_CYAN_LIGHT << ""
			++CharIndex;
		}
}

int GameplayStatics::InteractiveDisplay(const std::vector<std::string>& Options, const int Right, const bool bClear, const std::vector<Item*>& Items) {
	DisplayMenu->SetOptions(Options);
	DisplayMenu->SetUp(static_cast<int>(Options.size()));
	DisplayMenu->SetRight(Right);
	DisplayMenu->SetClear(bClear);
	DisplayMenu->SetItems(Items);
	return DisplayMenu->Select();
}

void GameplayStatics::ANSI_CURSOR_DOWN_N(const int nDown) {
	DisplayMenu->ANSI_CURSOR_DOWN_N(nDown);
}

void GameplayStatics::DisplayMapMenuTitle() {
	std::cout << ANSI_COLOR_VIVID_YELLOW;
	std::cout << "========     MAP MENU     ========" << '\n';
	std::cout << "==================================" << '\n';
	std::cout << ANSI_COLOR_RESET;
}

void GameplayStatics::DisplayCombatMenuTitle() {
	std::cout << ANSI_COLOR_CYAN_LIGHT;
	std::cout << "=> COMBAT MENU <=" << '\n';
	std::cout << "=================" << '\n';
	std::cout << ANSI_COLOR_RESET;
}

void GameplayStatics::DisplaySpellMenuTitle() {
	DisplayMenu->Clear(2);
	std::cout << ANSI_COLOR_CYAN_LIGHT;
	std::cout << "=> AVAILABLE SPELLS <=" << '\n';
	std::cout << "======================" << '\n';
	std::cout << ANSI_COLOR_RESET;
}

void GameplayStatics::DisplayMapMenu() {
	DisplayMapMenuTitle();

	// MAP LOOP
	while (PlayerAvatar.lock()->IsAlive()) {
		std::vector<std::string> v = { "SHOW POSITION", "SHOW MAP", "MOVE", "ITEMS", "SPELLS", "STATS"};
		const int Input = InteractiveDisplay(v);
		HandleMapInput(Input);
	}
}

void GameplayStatics::HandleMapInput(const int Input) {
	switch (Input) {
	case 0:
		MG.ShowPosition();
		break;
	case 1:
		MG.ShowMap();
		break;
	case 2:
		MG.HandleMovement();
		break;
	case 3:
		DisplayItemMenu();
		break;
	//case 4:
	//	DisplaySpellMenu(); // treba drugu funkciju -> funkciju koja pokazuje v = {ACTIVE, PASSIVE, ALL(instanced)};
	case 5:
		DisplayPlayerStats();
		break;
	default:
		break;
	}
}

PlayerCharacter* GameplayStatics::GetTurnCharacter() {
	std::vector<std::string> Menu;
	for (const auto& Char : PlayerCharacters)
		Menu.push_back(GameplayStatics::GetEnumString(Char->GetClass()));
	Menu.emplace_back("<--BACK--<");
	int Input;
	if ((Input = InteractiveDisplay(Menu)) == -1) return nullptr;
	return dynamic_cast<PlayerCharacter*>(PlayerCharacters[Input].get());  //TODO : Check Ref Count ??
}

void GameplayStatics::DisplayItemMenu() {
	PlayerCharacter* CurrentCharacter;
	if ((CurrentCharacter = GetTurnCharacter()) == nullptr) return;

	bool bIsEquipped = false;
	auto& SelectedItem = CurrentCharacter->DisplayAllItems(bIsEquipped);
	if (bIsEquipped) return;
	
	std::vector<std::string> Menu;
	int Input;
	if (bIsEquipped) {
		Menu = { "UN-EQUIP", "DESTROY", "<--BACK--<" };
		if ((Input = InteractiveDisplay(Menu)) == -1) {
			CurrentCharacter->EquipItem(SelectedItem);
			return;
		}
		if (Input == 0) CurrentCharacter->UnEquipItem(SelectedItem);
		else CurrentCharacter->DestroyItem(std::move(SelectedItem));
	}
	else {
		Menu = { "EQUIP", "DESTROY", "<--BACK--<" };
		if ((Input = InteractiveDisplay(Menu)) == -1) {
			CurrentCharacter->AddItemToInventory(SelectedItem);  // TODO : if inventory is FULL, item is ["CONSUMED" - LOST]
			return;
		}
		if (Input == 0) CurrentCharacter->EquipItem(SelectedItem);
		else CurrentCharacter->DestroyItem(std::move(SelectedItem));
	}
}

void GameplayStatics::DisplayPlayerStats() {
	GetTurnCharacter()->DisplayStats();
}

void GameplayStatics::RedrawGameScreen() {
	DisplayAllies();
	DisplayEnemies();
	MG.DisplayPlayGrid();
	//MG.DisplayGrid();
	CombatManager::DisplayTurnOrder();
	DisplayCombatLog();
}


void GameplayStatics::InitiateCombatMode(std::vector<std::weak_ptr<Character>>&& Enemies) {
	PlayerAvatar.lock()->SetIsInCombat(true);

	std::vector<std::weak_ptr<Character>> WPtrPlayerCharacters(PlayerCharacters.size());
	std::ranges::transform(PlayerCharacters, WPtrPlayerCharacters.begin(), [](const std::weak_ptr<Character>& WPtrChar) { return std::weak_ptr<Character>(WPtrChar); });
	
	CombatManager::SetTurns(std::move(WPtrPlayerCharacters), std::move(Enemies));
	CombatManager::StartCombat(PlayerAvatar);


	//std::cout << "STR: " << _player.lock()->GetPlayerAttributes()._strength << "  BASE CRIT DMG: " << _player.lock()->GetCritDmg().GetBase() << "  ACTUAL CRIT DMG: " << _player.lock()->GetCritDmg().GetActual() << '\n';
	//std::cout << "AGI: " << _player.lock()->GetPlayerAttributes()._agility << "  BASE ARMOR: " << _player.lock()->GetArmor().GetBase() << "  ACTUAL ARMOR: " << _player.lock()->GetArmor().GetActual() << '\n';
	//std::cout << "INT: " << _player.lock()->GetPlayerAttributes()._intelligence << "  BASE ESSENCE: " << _player.lock()->GetEssence().GetBase() << "  ACTUAL ESSENCE: " << _player.lock()->GetEssence().GetActual() << "SpellBook Crit %: " << _player.lock()->GetSpellCritChance().GetBase() << '\n';
	//std::cout << "VIT: " << _player.lock()->GetPlayerAttributes()._vitality << "  BASE HP: " << _player.lock()->GetHealth().GetBase() << "  ACTUAL HP: " << _player.lock()->GetHealth().GetActual() << '\n';

	//int x; cin >> x;



	ResetCombatVariables();
	CLS;

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
	//	std::cout << item->_item_info._name << "\n";
	//	std::cout << GetEnumString(item->_item_info._item_slot) << "\t" << GetEnumString(item->_item_info._item_rarity) << "\t" << GetEnumString(item->_item_info._item_type) << "\t" << GetEnumString(item->_item_info._weapon_type) << "\n";
	//	cout << "Dmg min: " << item->_item_info._dmg_min << "\tDmg max: " << item->_item_info._dmg_max << "\tArmor: " << item->_item_info._armor << "\tIlvl: " << item->_item_info._lvl << "\n\n";
	//}
	//int x; cin >> x;
}

// TODO: Move to a different file, Logger.cpp ???
void GameplayStatics::ResetCombatVariables() {
	auto& StringStream = GetCombatLogStream();
	StringStream.clear();
	StringStream.str("");
}

void GameplayStatics::GiveExperience(EnemyCharacter* enemy) {
	// Calculate
	int experience = 0;
	//_player->ReceiveExperience(experience);
}

int GameplayStatics::DisplayCombatMenu(Character* character) {
	DisplayCombatMenuTitle();
	std::vector<std::string> Menu;

	if (dynamic_cast<PlayerCharacter*>(character)) {
		Menu = { "MELEE ATTACK", "RANGED ATTACK", "CAST SPELL", "MOVE", "INFO", "END TURN" };
		return InteractiveDisplay(Menu);
	}
	else if (dynamic_cast<SummonCharacter*>(character)) {
		switch (const auto Summon = dynamic_cast<SummonCharacter*>(character); Summon->GetClass()) {
		case ECharacterClass::FIRE_ELEMENTAL:
		case ECharacterClass::WATER_ELEMENTAL:
		case ECharacterClass::ARCANE_ELEMENTAL:
			Menu = { "CAST SPELL", "MOVE", "END TURN" };
			return InteractiveDisplay(Menu);
		default:
			break;
		}
	}
	return -1;
}

void GameplayStatics::HandleCombatInput(PlayerCharacter* InCharacter, const int Input) {
	switch (Input) {
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
		CombatManager::EndTurn(std::weak_ptr<Character>(InCharacter));
		break;
	default:
		break;
	}
}

void GameplayStatics::HandleCombatInput(SummonCharacter* character, const int Input) {
	switch (Input) {
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
	std::map<int, EDirection> DirectionMap;
	std::vector<std::string> Menu = MG.GetCombatDirections(CombatManager::GetTurnCharacter().lock().get(), DirectionMap);
	Menu.emplace_back("<--BACK--<");

	const int Input = InteractiveDisplay(Menu);
	if (Input == -1) return;

	MG.MoveCharacterOnGrid(CombatManager::GetTurnCharacter().lock().get(), DirectionMap[Input]);
}

void GameplayStatics::EnemyCombatMove(Character* Enemy, OUT std::map<int, EDirection>& DirectionMap) {
	std::vector<std::string> Menu = MG.GetCombatDirections(Enemy, DirectionMap);
}

void GameplayStatics::MoveCharacterOnGrid(const Character& InCharacter, const EDirection Direction) {
	MG.MoveCharacterOnGrid(&InCharacter, Direction);
}

int GameplayStatics::GetPlayerIdx(char c) {
	int ret;
	while ((ret = MG.GetPlayerIdx(c)) == -1) {
		DisplayMenu->Clear(2);
		std::cout << ANSI_COLOR_RED << "Wrong alias. Input target alias: " << ANSI_COLOR_RESET << '\n';
		std::cout << "-> ";
		std::string s;
		std::cin >> s;
		c = s[0];
	}
	return ret;
}

int GameplayStatics::GetEnemyIdx(char c) {
	int ret;
	while ((ret = MG.GetEnemyIndex(c)) == -1) {
		DisplayMenu->Clear(2);
		std::cout << ANSI_COLOR_RED << "Wrong alias. Input target alias: " << ANSI_COLOR_RESET << '\n';
		std::cout << "-> ";
		std::string s;
		std::cin >> s;
		c = s[0];
	}
	return ret;
}

int GameplayStatics::GetSpellIndex(const ActiveSpell* CastSpell, std::shared_ptr<Character>& CasterCharacter) {
	CasterCharacter = CombatManager::GetTurnCharacter().lock();
	for (int i = 0; i < static_cast<int>(CasterCharacter->GetActiveSpells().size()); i++)
		if (CastSpell == CasterCharacter->GetActiveSpells()[i].get()) return i;
	
	RPG_ASSERT(!CasterCharacter, "GetSpellIndex")
	throw std::invalid_argument("Invalid Spell");
}

void GameplayStatics::DisplayMeleeMenu() {
	std::vector<std::string> v;
	std::vector<ActiveSpell*> spells;
	for (const auto& spell : CombatManager::GetTurnCharacter().lock()->GetActiveSpells())
		if (spell->GetClass() == ESpellClass::MELEE) {
			v.push_back(GetEnumString(spell->GetId()));
			spells.push_back(spell.get());
		}
	v.emplace_back("<--BACK--<");

	int input;
	if ((input = InteractiveDisplay(v)) == -1) return;

	HandleMeleeTarget(spells[input]);
}

void GameplayStatics::DisplayRangedMenu() {
	std::vector<std::string> v;
	std::vector<ActiveSpell*> spells;
	for (const auto& spell : CombatManager::GetTurnCharacter().lock()->GetActiveSpells())
		if (spell->GetClass() == ESpellClass::RANGED) {
			v.push_back(GetEnumString(spell->GetId()));
			spells.push_back(spell.get());
		}
	v.emplace_back("<--BACK--<");

	int input; 
	if ((input = InteractiveDisplay(v)) == -1) return;

	HandleTarget(spells[input]);
}


void GameplayStatics::DisplaySpellMenu() {
	DisplaySpellMenuTitle();

	std::vector<std::string> v;
	std::vector<ActiveSpell*> spells;
	for (auto& spell : CombatManager::GetTurnCharacter().lock()->GetActiveSpells()) {
		if (spell->GetClass() == ESpellClass::MAGIC) {
			v.push_back(GetEnumString(spell->GetId()));
			spells.push_back(spell.get());
		}
	}
	v.emplace_back("<--BACK--<");

	int input;
	if ((input = InteractiveDisplay(v)) == -1) return;

	HandleTarget(spells[input]);
}

void GameplayStatics::HandleTarget(const ActiveSpell* TargetSpell) {
	std::string s_input;
	int n_targets = 0;

	if (TargetSpell->GetSpellType() != ESpellType::SUMMON) {
		n_targets = 1;
		for (int i = 0; i < n_targets; i++) {
			std::cout << "Input target alias:" << '\n';
			std::cout << "-> ";
			std::cin >> s_input;
		}
	}

	std::vector<int> p_idx;
	std::vector<int> e_idx;
	for (int i = 0; i < n_targets; i++) {
		char c = s_input[i];
		if (c >= '0' && c <= '9') p_idx.push_back(GetPlayerIdx(c));
		else if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z') e_idx.push_back(GetEnemyIdx(c));
	}
	
	std::vector<std::weak_ptr<Character>> targets;
	if (e_idx.empty())
		for (const int i : p_idx)
			targets.push_back(Players[i]);
	else
		for (const int i : e_idx)
			targets.push_back(Enemies[i]);

	std::shared_ptr<Character> character;
	int spell_idx = GetSpellIndex(TargetSpell, character);
	_sm->CastSpell(spell_idx, character, targets);
}

void GameplayStatics::HandleMeleeTarget(ActiveSpell* spell) {
	std::shared_ptr<Character> character;
	int spell_idx = GetSpellIndex(spell, character);

	std::vector<Character*> characters = MG.GetCharactersInRange(CombatManager::GetTurnCharacter().lock().get());

	std::vector<std::string> alias;
	for (const auto& c : characters)
		if (c) alias.emplace_back(1, (c->GetAlias()));
	alias.emplace_back("<--BACK--<");

	int input;
	if ((input = InteractiveDisplay(alias)) == -1) return;

	int enemy_idx = GetEnemyIdx(alias[input][0]);
	std::vector<std::weak_ptr<Character>> targets = { Enemies[enemy_idx] };
	_sm->CastSpell(spell_idx, character, targets);
}

void GameplayStatics::DisplayInfoMenu() {
	DisplayMenu->Clear(2);
	std::cout << ANSI_COLOR_CYAN_LIGHT;
	std::cout << "=> INFO MENU <=" << '\n';
	std::cout << "===============" << '\n';
	std::cout << ANSI_COLOR_RESET;

	const std::vector<std::string> v = { "SPELLS", "BACK" };
	int input = InteractiveDisplay(v);
}

void GameplayStatics::HandleInfoInput(int input) {

}

//void GameplayStatics::HandleEffectInfo(int spell_idx, ESpellType spell_type, int effect_idx) {
//	std::vector<std::shared_ptr<ActiveSpell>> effects;
//	auto spellbooks = CombatManager::GetTurnCharacter().lock()->GetActiveSpells();
//	for (int i = 0; i < spellbooks.size(); i++) {
//		if (spell_idx == i) {
//			effects = spellbooks[spell_idx]->GetEffects();
//			stringstream& ss = effects[effect_idx]->GetTooltip();
//
//			const int max_lines = 19;
//			std::vector<string> lines;
//			int start_index;
//			ExtractLinesFromStringstream(lines, max_lines, ss, start_index);
//
//			_menu->ClearRight(23);
//			cout << CURSOR_LOG_LEFT << ANSI_COLOR_ORANGE;
//			cout << CURSOR_LOG_RIGHT << "->  INFO MENU  <-" << '\n';
//			cout << CURSOR_LOG_RIGHT << "=================" << '\n';
//			for (int i = start_index; i < lines.size(); ++i) {
//				cout << CURSOR_LOG_RIGHT << lines[i];
//			}
//
//			int input;
//			cout << '\n' << CURSOR_LOG_RIGHT << COLOR_COMBAT_LOG << "Press enter to return";
//			do {
//				input = _getch();
//			} while (input != '\r');
//		}
//	}
//}

void GameplayStatics::DisplayCombatLog() {

	std::cout << ANSI_CURSOR_UP(50);
	int e_size = static_cast<int>(std::count_if(Enemies.begin(), Enemies.end(), [](const std::weak_ptr<Character>& wptr) { return !wptr.expired(); }));
	int s_size = static_cast<int>(CombatManager::GetSummons().size());
	DisplayMenu->ANSI_CURSOR_DOWN_N(static_cast<int>(Players.size() + e_size + s_size));
	std::cout << CURSOR_LOG_RIGHT << COLOR_COMBAT_LOG;
	std::cout << ANSI_COLOR_BLUE << "()()()   COMBAT LOG   ()()()" << "\n" << CURSOR_LOG_RIGHT;
	std::cout << ANSI_COLOR_BLUE << "()()()()()()()()()()()()()()" << ANSI_COLOR_RESET << "\n";

	const int max_lines = 49;
	std::vector<std::string> lines;
	int start_index;
	ExtractLinesFromStringStream(lines, max_lines, CombatLog, start_index);

	for (int i = start_index; i < lines.size(); i++) {
		std::cout << CURSOR_LOG_RIGHT << lines[i];
	}

	// Move Menu below grid
	std::cout << ANSI_CURSOR_UP(50);
	DisplayMenu->ANSI_CURSOR_DOWN_N(21 + e_size + s_size + static_cast<int>(Players.size()));
}

void GameplayStatics::ExtractLinesFromStringStream(OUT std::vector<std::string>& Lines, const int MaxLines, std::stringstream& Buffer, OUT int& StartIndex) {
	const std::string content = Buffer.str();
	if (content.empty()) Buffer << std::fixed << std::setprecision(2);
	int start = 0;

	for (int i = 0; i < content.size(); ++i) {
		if (content[i] == '\n' || i == content.size() - 1) {
			std::string line = content.substr(start, i - start + 1);
			Lines.push_back(line);
			start = i + 1;
		}
	}
	StartIndex = std::max(0, static_cast<int>(Lines.size()) - MaxLines);
}

std::weak_ptr<Character> GameplayStatics::GetWeakCharacter(const Character& InCharacter) {
	if (InCharacter.GetTeam() == 1) {
		for (const auto& player : Players)
			if (player.lock()->GetAlias() == InCharacter.GetAlias())
				return player;
	}
	else {
		for (const auto& enemy : Enemies)
			if (enemy.lock()->GetAlias() == InCharacter.GetAlias())
				return enemy;
	}
	return {};
}

std::shared_ptr<Character> GameplayStatics::GetSharedCharacter(const Character& InCharacter) {
	
	if (InCharacter.GetTeam() == 1) {
		for (const auto& player : Players)
			if (auto sharedPlayer = player.lock();
				sharedPlayer && sharedPlayer->GetAlias() == InCharacter.GetAlias()) return sharedPlayer;
	}
	else
		for (const auto& enemy : Enemies)
			if (auto sharedEnemy = enemy.lock();
				sharedEnemy && sharedEnemy->GetAlias() == InCharacter.GetAlias()) return sharedEnemy;
	
	return nullptr;
}

std::vector<std::weak_ptr<Character>> GameplayStatics::GetPlayerCharacters() {
	return Players;
}

std::vector<std::weak_ptr<Character>> GameplayStatics::GetEnemyCharacters() {
	return Enemies;
}

float GameplayStatics::ApplyDamage(const std::weak_ptr<Character>& Instigator, Character* Target, float Damage, const std::unique_ptr<ActiveSpell>& Spell, const bool bIsOnApply) {
	RPG_ASSERT(!Instigator.expired(), "ApplyDamage")

	Damage = Float2(Damage);
	float ActualDamage;
	if (Spell->GetDamageType() != EDamageType::PURE)
		ActualDamage = Resistances::CalculateDamage(Damage, Spell->GetDamageType(), Target);
	else ActualDamage = Damage;
	const float Resisted = Float2(Damage - ActualDamage);   // resisted / armor blocked - znaci change i u combat logu...ovisno o damage type..

	// POGLEDATI ZAKAJ TU IMA IKAKSE VEZE BOOLEAN
	//==============================================
	auto& s = GetCombatLogStream();
	const std::string C = GetAliasColor(Instigator.lock()->GetAlias());
	const std::string CT = GetAliasColor(Target->GetAlias());
	if (bIsOnApply) s << CT << Target->GetAlias() << COLOR_COMBAT_LOG << " suffers from " << COLOR_EFFECT << GetEnumString(Spell->GetId()) << COLOR_COMBAT_LOG << " for " << COLOR_VALUE << ActualDamage * -1 << COLOR_COMBAT_LOG << " damage [resisted:" << COLOR_VALUE << Resisted * -1 << COLOR_COMBAT_LOG << "]\n";
	else s << C << Target->GetAlias() << COLOR_COMBAT_LOG << " suffers from " << COLOR_EFFECT << GetEnumString(Spell->GetId()) << COLOR_COMBAT_LOG << " for " << COLOR_VALUE << ActualDamage * -1 << COLOR_COMBAT_LOG << " damage [resisted:" << COLOR_VALUE << Resisted * -1 << COLOR_COMBAT_LOG << "]\n";
	//==============================================

	return ActualDamage;
}

void GameplayStatics::ApplyEffect(std::shared_ptr<Character>& Instigator, std::vector<std::weak_ptr<Character>>& Targets, std::unique_ptr<ActiveSpell> Spell,
								  const std::optional<ApplyParams>& ApplyParams, const std::optional<EffectParams>& EffectParams) {
	const std::string C = GetAliasColor(Instigator->GetAlias());
	auto& s = GetCombatLogStream();
	s << C << Instigator->GetAlias() << COLOR_COMBAT_LOG << " Casts " << COLOR_EFFECT << GameplayStatics::GetEnumString(Spell->GetId()) << COLOR_COMBAT_LOG << ".\n";

	auto effect = std::make_shared<CombatEffect>(std::move(Instigator), Targets, Spell, ApplyParams, EffectParams, SpellDb::Data[Spell->GetId()][Spell->GetLvl()].Duration);
	CombatManager->AddCombatEffect(std::move(effect));
}

void GameplayStatics::KillEnemy(int Idx) {
	MG.KillEnemy(Idx);
}


//// REMOVE AFTER MAKING MAP_GEN A SINGLETON
bool GameplayStatics::AddCharacterToCharGrid(const std::shared_ptr<Character>& Instigator, const std::weak_ptr<Character>& Summon) {
	return MG.AddCharacterToCharGrid(Instigator, Summon);
}

////////////////////////////////////////////////

void GameplayStatics::RollLoot() {
	for (const auto& player : PlayerCharacters) {
		auto loot = std::move(Item::GenerateLoot(static_pointer_cast<PlayerCharacter>(player), /*MG.GetPowerLvl()*/ 360));
		DisplayLoot(static_pointer_cast<PlayerCharacter>(player), std::move(loot));
	}
}

void GameplayStatics::DisplayLoot(const std::weak_ptr<PlayerCharacter>& Character, std::vector<std::unique_ptr<Item>> Loot) {
	CLS;
	if (!Loot.empty()) {
		std::cout << COLOR_LOOT << GetEnumString(Character.lock()->GetClass()) << "'s loot!. (" << Loot.size() << ")" << "\nChoose which items you want to keep.\n";

		std::vector<Item*> items;
		std::vector<std::string> Menu = { "--> ALL ITEMS <--" };
		
		while (!Loot.empty()) {
			items.clear();
			for (const auto& item : Loot) {
				Menu.push_back(item->ItemInfo.Name);
				items.push_back(item.get());
			}
			Menu.emplace_back("<--BACK--<");

			const int input = InteractiveDisplay(Menu, 0, true, items);
			if (input == -1) break;
			if (input == 0) { // add all items
				if (Character.lock()->GetInventorySpace() >= Loot.size()) {
					for (auto& item : Loot)
						if (item) Character.lock()->AddItemToInventory(std::move(item));
					break;
				}
				else {
					std::cout << COLOR_ERROR << "Not enough inventory space! " << COLOR_LOOT << "(" << Character.lock()->GetInventorySpace() << ")\n";
				}
			}
			else { // add selected item
				if (Character.lock()->GetInventorySpace() > 0) {
					Character.lock()->AddItemToInventory(std::move(Loot[input - 1]));
					Loot.erase(std::ranges::find_if(Loot, [](const auto& Ptr) { return Ptr == nullptr; }));  // TODO : CHECK RANGES WORK

					Menu = { "--> ALL ITEMS <--" };
					for (const auto& item : Loot)
						Menu.push_back(item->ItemInfo.Name);
					Menu.emplace_back("<--BACK--<");
				}
				else {
					std::cout << COLOR_ERROR << "Not enough inventory space! " << COLOR_LOOT << "(" << Character.lock()->GetInventorySpace() << ")\n";
				}
			}
		}
	}
	else {
		std::cout << COLOR_LOOT << GetEnumString(Character.lock()->GetClass()) << " received no loot this time.\n";
		CLS;
	}
}


std::string GameplayStatics::GetAliasColor(const char Alias) {
	if (UPPER(Alias) >= 'A' && UPPER(Alias) <= 'Z') return COLOR_ENEMY;
	else return COLOR_PLAYER;
}

std::string GameplayStatics::String2(const float F) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << F;
	return ss.str();
}

float GameplayStatics::Float2(const float F) {
	return round(F * 100) / 100;
}

int GameplayStatics::GetRandInt(const int A, const int B) {
	static std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<int> distribution(A, B);
	return distribution(generator);
}

float GameplayStatics::GetRandFloat(const float A, const float B) {
	static std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<float> distribution(A, B);
	return Float2(distribution(generator));
}

std::string GameplayStatics::GetEnumString(const ESpellID Enum) {
	switch (Enum) {
	case ESpellID::FIREBALL:
		return "FIREBALL";
	case ESpellID::BURNING:
		return "BURNING";
	case ESpellID::MOLTEN_ARMOR:
		return "MOLTEN ARMOR";
	case ESpellID::EXPOSURE:
		return "EXPOSURE";
	case ESpellID::STONESKIN:
		return "STONESKIN";
	case ESpellID::DISARM:
		return "DISARM";
	case ESpellID::BLOODBATH:
		return "BLOODBATH";
	case ESpellID::ARCANE_INFUSION:
		return "ARCANE INFUSION";
	case ESpellID::BLOOD_RAIN:
		return "BLOOD RAIN";
	case ESpellID::VISCOUS_ACID:
		return "VISCOUS ACID";
	case ESpellID::SUM_FIRE_ELE:
		return "SUMMON FIRE ELEMENTAL";
	case ESpellID::SUM_FIRE_IMP:
		return "SUMMON FIRE IMP";
	case ESpellID::BLIND:
		return "BLIND";
	case ESpellID::MELEE:
		return "MELEE ATTACK";
	case ESpellID::RANGED:
		return "RANGED ATTACK";
	default:
		return "DEFAULT";
	}
}


std::string GameplayStatics::GetTypeString(int Enum) {
	switch (Enum) {
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

std::string GameplayStatics::GetEnumString(ESpellType Enum) {
	switch (Enum) {
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

std::string GameplayStatics::GetEnumString(ESpellActivity Enum) {
	switch (Enum) {
	case ESpellActivity::PASSIVE:
		return "PASIVE";
	case ESpellActivity::ACTIVE:
		return "ACTIVE";
	default:
		return "No type";
	}
}

std::string GameplayStatics::GetEnumString(EDamageType Enum) {
	switch (Enum) {
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

std::string GameplayStatics::GetEnumString(ECharacterClass Enum) {
	switch (Enum) {
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

std::string GameplayStatics::GetEnumString(EItemSlot Enum) {
	switch (Enum) {
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

std::string GameplayStatics::GetEnumString(EWeaponType Enum) {
	switch (Enum) {
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

std::string GameplayStatics::GetEnumString(EItemRarity Enum) {
	switch (Enum) {
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

std::string GameplayStatics::GetEnumString(EItemType Enum) {
	switch (Enum) {
	case EItemType::CONSUMABLE:
		return "Consumable";
	case EItemType::SCROLL:
		return "Scroll";
	case EItemType::ARMOR:
		return "Armor";
	case EItemType::JEWELLERY:
		return "Jewlery";
	case EItemType::WEAPON:
		return "Weapon";
	case EItemType::RELIC:
		return "Relic";
	default:
		return "";
	}
}