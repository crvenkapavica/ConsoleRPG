#include "EnemyCharacter.h"
#include "SummonCharacter.h"
#include "../GameplayStatics.h"
#include "../Characters/CharacterData.h"
#include "../Combat/CombatManager.h"
#include "../MapGenerator/MapGenerator.h"
//#include "../Spells/SpellManager.h"

int EnemyCharacter::nEnemyCharacters = 0;

EnemyCharacter::EnemyCharacter(const ECharacterClass Other)
	: Character(CharDb::Data[Other], 'A' + nEnemyCharacters++)
{}

EnemyCharacter::EnemyCharacter(const EnemyCharacter& Other)
	: Character(Other)
{}

EnemyCharacter::EnemyCharacter(EnemyCharacter&& Other) noexcept
	: Character(std::move(Other))
{}

void EnemyCharacter::TakeTurn() {
	Move();
	//CastSpell();

	GameplayStatics::RedrawGameScreen();
	Sleep(1200);
	
	CombatManager::EndTurn(*this);
}

void EnemyCharacter::Move() const {
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


		// TODO : CAST SPELL
	// if (!GetPassiveSpells().empty() && GetPassiveSpells()[0])
	// 	SpellManager::CastSpell(0, GameplayStatics::GetSharedCharacter(*this), TargetCharacters);
}