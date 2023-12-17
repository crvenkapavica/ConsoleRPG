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

void SummonCharacter::TakeTurn() {

	GameplayStatics::RedrawGameScreen();
	int input = GameplayStatics::DisplayCombatMenu(this);
	GameplayStatics::HandleCombatInput(this, input);
}