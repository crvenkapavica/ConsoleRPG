#include "EnemyCharacter.h"
#include "CharacterData.h"
#include "../GameplayStatics.h"
#include "../Spells/SpellBook.h"
#include "../Spells/SpellManager.h"

EnemyCharacter::EnemyCharacter(const CharacterData::EnemyStats& data) 
	: Character(data)
	, _level(0)
	, _count(0)
{}

void EnemyCharacter::TakeTurn() {
	map<int, EDirection> direction_map;
	GameplayStatics::EnemyCombatMove(this, direction_map);
	if (direction_map.size()) {
		int rnd = rand() % static_cast<int>(direction_map.size());
		GameplayStatics::MoveEnemyCharacterOnGrid(this, direction_map[rnd]);
	}

	//CastSpell();

	GameplayStatics::RedrawGameScreen();
	Sleep(300);
	auto& s = GameplayStatics::GetCombatLogStream();
	s << "Character (" << COLOR_ENEMY << _alias << COLOR_COMBAT_LOG << ") ends their turn.\n";
	EndTurn();
}

void EnemyCharacter::CastSpell() {
	_player_characters = GameplayStatics::GetPlayerCharacters();
	_enemy_characters = GameplayStatics::GetEnemyCharacters();
	vector<int> p_idx;
	vector<int> e_idx = { 0 };
	int effect_index = 0;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CastSpell(0, this, _player_characters, _enemy_characters, p_idx, e_idx);
}