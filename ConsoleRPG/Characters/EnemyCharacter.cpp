#include "EnemyCharacter.h"
#include "CharacterData.h"
#include "../GameplayStatics.h"
#include "../Spells/SpellManager.h"
#include "../Combat/CombatManager.h"

EnemyCharacter::EnemyCharacter(const CharacterData::EnemyStats& data) 
	: Character(data)
	, _level(0)
	, _count(0)
{}

void EnemyCharacter::TakeTurn() {

	Move();

	/*if (_alias == 'A' && cm.GetTurn() == 0) {
		CastSpell();
	}*/

	GameplayStatics::RedrawGameScreen();
	Sleep(300);

	EndTurn();
}

void EnemyCharacter::Move() {
	map<int, EDirection> direction_map;
	GameplayStatics::EnemyCombatMove(this, direction_map);
	if (direction_map.size()) {
		int rnd = rand() % static_cast<int>(direction_map.size());
		GameplayStatics::MoveCharacterOnGrid(this, direction_map[rnd]);
	}
}

void EnemyCharacter::CastSpell() {
	_players = GameplayStatics::GetPlayerCharacters();
	_enemies = GameplayStatics::GetEnemyCharacters();
	vector<int> p_idx = { 0 };
	vector<int> e_idx;

	vector<weak_ptr<Character>> targets = { _players[p_idx[0]] };

	SpellManager& sm = SpellManager::GetInstance();
	sm.CastSpell(0, this, targets);
}