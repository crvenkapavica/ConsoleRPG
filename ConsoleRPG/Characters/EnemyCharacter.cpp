#include "EnemyCharacter.h"
#include "SummonCharacter.h"
#include "../GameplayStatics.h"
#include "../Spells/SpellManager.h"
#include "../Combat/CombatManager.h"
#include "../Characters/CharacterData.h"

int EnemyCharacter::_n = 0;

EnemyCharacter::EnemyCharacter(ECharacterClass character_class)
	: Character(CharDB::_data[character_class], 'A' + _n++)
	, _level(0)
	, _count(0)
{}

EnemyCharacter::EnemyCharacter(const EnemyCharacter& other)
	: Character(other)
	, _level(0)
	, _count(0)
{}

EnemyCharacter::EnemyCharacter(EnemyCharacter&& enemy) noexcept
	: Character(enemy)
	, _level(enemy._level)
	, _count(enemy._count)
{}

EnemyCharacter::~EnemyCharacter()
{}

void EnemyCharacter::TakeTurn() {

	Move();

	//CastSpell();

	GameplayStatics::RedrawGameScreen();
	Sleep(1200);

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

	if (GetPassiveSpells().size() && GetPassiveSpells()[0]) {

		SpellManager& sm = SpellManager::GetInstance();
		sm.CastSpell(0, GameplayStatics::GetSharedCharacter(this), targets);
	}
}