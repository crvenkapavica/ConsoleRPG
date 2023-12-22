#include "SummonCharacter.h"
#include "../GameplayStatics.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "CharacterData.h"

int SummonCharacter::_p_n = 0;
int SummonCharacter::_e_n = 0;

SummonCharacter::SummonCharacter(ECharacterClass character_class, int team)
	: Character(CharDB::_data[character_class], team, [team]() { return team == 1 ? '0' + PlayerCharacter::_n + _p_n++ : 'A' + EnemyCharacter::_n + _e_n++; })
{}

SummonCharacter::SummonCharacter(const SummonCharacter& other)
	: Character(other)
{}

SummonCharacter::SummonCharacter(SummonCharacter&& other) noexcept
	: Character(other)
{}

SummonCharacter::~SummonCharacter()
{}

void SummonCharacter::TakeTurn() {

	// The Summon is Player Controlled
	if (_team == 1) {
		GameplayStatics::RedrawGameScreen();
		int input = GameplayStatics::DisplayCombatMenu(this);
		GameplayStatics::HandleCombatInput(this, input);
	}
	// The Summon is AI controlled
	else { // The actual functionality should be programmed either inside this class or the EnemyCharacter class. Depending on how different the logic between summons and non-summons is.
		GameplayStatics::RedrawGameScreen();
		Sleep(300);

		EndTurn();
	}
}