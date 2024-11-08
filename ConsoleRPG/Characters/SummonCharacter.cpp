#include "SummonCharacter.h"
#include "CharacterData.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "../GameplayStatics.h"
#include "../Combat/CombatManager.h"

int SummonCharacter::nPlayerSummons = 0;
int SummonCharacter::nEnemySummons = 0;

SummonCharacter::SummonCharacter(const ECharacterClass InCharacterClass, int InTeam)
	: Character(CharDb::Data[InCharacterClass]
		, InTeam
		, [InTeam]() { return InTeam == 1 ? '0' + PlayerCharacter::nPlayerCharacters + nPlayerSummons++ : 'A' + EnemyCharacter::nEnemyCharacters + nEnemySummons++; })
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

		CombatManager::EndTurn(*this);
	}
}