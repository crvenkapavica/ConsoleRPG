#include "SummonCharacter.h"
#include "CharacterData.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "../GameplayStatics.h"

int SummonCharacter::nPlayerSummons = 0;
int SummonCharacter::nEnemySummons = 0;

SummonCharacter::SummonCharacter(const ECharacterClass InCharacterClass, int InTeam)
	: Character(CharDb::Data[InCharacterClass]
	, InTeam
	, [InTeam]() { return InTeam == 1 ? '0' + PlayerCharacter::nPlayerCharacters + nPlayerSummons++ : 'A' + EnemyCharacter::nEnemyCharacters + nEnemySummons++; })
{}

SummonCharacter::SummonCharacter(const SummonCharacter& Other)
	: Character(Other)
{}

SummonCharacter::SummonCharacter(SummonCharacter&& Other) noexcept
	: Character(std::move(Other))
{}

void SummonCharacter::TakeTurn() {
	// The Summon is Player Controlled
	if (Team == 1) {
		GameplayStatics::RedrawGameScreen();
		const int Input = GameplayStatics::DisplayCombatMenu(this);
		GameplayStatics::HandleCombatInput(this, Input);
	}
	// The Summon is AI controlled
	// The actual functionality should be programmed either inside this class or the EnemyCharacter class. Depending on how different the logic between summons and non-summons is.
	else { 
		GameplayStatics::RedrawGameScreen();
		Sleep(300);

		EndTurn();
	}
}