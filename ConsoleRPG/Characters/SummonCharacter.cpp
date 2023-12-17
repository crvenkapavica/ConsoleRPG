#include "SummonCharacter.h"
#include "../GameplayStatics.h"

SummonCharacter::SummonCharacter(const CharacterData& data)
    : Character(data.GetSummonData())
	, _class(data.GetSummonData()._class)
	, _summon_type(data.GetSummonData()._summon_type)
	, _dmg_type(data.GetSummonData()._dmg_type)
	, _spell(data.GetSummonData()._spell)
{}

void SummonCharacter::TakeTurn() {

	GameplayStatics::RedrawGameScreen();
	int input = GameplayStatics::DisplayCombatMenu(this);
	GameplayStatics::HandleCombatInput(this, input);
}