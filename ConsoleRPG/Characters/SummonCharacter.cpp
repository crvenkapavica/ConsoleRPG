#include "SummonCharacter.h"
#include "../GameplayStatics.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

SummonCharacter::SummonCharacter(const CharacterData& data, int team)
    : Character(data.GetSummonData(), team, team == 1 ? '0' + PlayerCharacter::_n++ : 'A' + EnemyCharacter::_n++)
	, _class(data.GetSummonData()._class)
	, _summon_type(data.GetSummonData()._summon_type)
	, _dmg_type(data.GetSummonData()._dmg_type)
	, _spell(data.GetSummonData()._spell)
{}

SummonCharacter::~SummonCharacter()
{
	if (_team == 1) 
		--PlayerCharacter::_n;
	else
		--EnemyCharacter::_n;
}

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
		Sleep(3000);

		EndTurn();
	}
}