#include "SummonCharacter.h"
#include "../GameplayStatics.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

SummonCharacter::SummonCharacter(const CharacterData& data, int team, vector<weak_ptr<Character>>& v)
	: Character(data.GetSummonData(), team, [team, &v]() { 
		char c = team == 1 ? '0' : 'A';
		for (int i = 0; i < v.size(); i++)
			if (team == 1) { if (v[i].lock()->GetAlias() != c + i) return static_cast<char>(c + i); }
			else if (v[i].lock()->GetAlias() != c + i) { /*++EnemyCharacter::_n;*/ return static_cast<char>(c + i); };
		return team == 1 ? static_cast<char>('0' + v.size()) : static_cast<char>('A' + v.size());
		})
	, _class(data.GetSummonData()._class)
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
		Sleep(300);

		EndTurn();
	}
}