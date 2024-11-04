#include "EnemyCharacter.h"
#include "SummonCharacter.h"
#include "../GameplayStatics.h"
#include "../Spells/SpellManager.h"
#include "../Combat/CombatManager.h"
#include "../Characters/CharacterData.h"
#include "../MapGenerator/MapGenerator.h"

int EnemyCharacter::nEnemyCharacters = 0;

EnemyCharacter::EnemyCharacter(const ECharacterClass InCharacterClass)
	: Character(CharDb::Data[InCharacterClass], 'A' + nEnemyCharacters++)
	// , Level(0)
	// , Count(0)
{}

EnemyCharacter::EnemyCharacter(EnemyCharacter&& Other) 
	: Character(std::move(Other))
	// , Level(0)
	// , Count(0)
{}

void EnemyCharacter::TakeTurn() {
	Move();
	//CastSpell();

	GameplayStatics::RedrawGameScreen();
	Sleep(1200);
	
	CombatManager::EndTurn(*this);
}

void EnemyCharacter::Move() {
	std::map<int, EDirection> DirectionMap;
	GameplayStatics::EnemyCombatMove(this, DirectionMap);
	if (!DirectionMap.empty()) {
		const int Rnd = GameplayStatics::GetRandInt(1, static_cast<int>(DirectionMap.size()));
		MapGenerator::GetInstance().MoveCharacterOnGrid(*this, DirectionMap[Rnd]);
	}
}

void EnemyCharacter::CastSpell() {
	Players = GameplayStatics::GetPlayerCharacters();
	Enemies = GameplayStatics::GetEnemyCharacters();
	const std::vector<int> PlayerTargetIndex = { 0 };
	const std::vector<int> EnemyTargetIndex;

	std::vector<std::weak_ptr<Character>> TargetCharacters = { Players[PlayerTargetIndex[0]] };

	if (!GetPassiveSpells().empty() && GetPassiveSpells()[0])
		SpellManager::CastSpell(0, GameplayStatics::GetSharedCharacter(*this), TargetCharacters);
}