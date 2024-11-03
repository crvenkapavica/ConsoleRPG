#pragma once

#include "GameplayStatics.h"
#include "RPGTypes.h"
#include "Characters/Character.h"
#include "Characters/CharacterData.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Combat/CombatManager.h"
#include "MapGenerator/MapGenerator.h"
#include "Spells/SpellData.h"
#include "Spells/SpellManager.h"


void GameLoop(MapGenerator& mapGenerator) {
	mapGenerator.PrintDebugMap();
	//mapGenerator.DisplayMenu();s
}

#include "Display/ConsoleMenu.h"		

#include <windows.h>

void SendAltEnter() {
    // Array of INPUT structures for the key events
    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    // Key down alt
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_MENU;  // Alt key

    // Key down enter
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_RETURN;  // Enter key

    // Key up enter
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_RETURN;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    // Key up alt
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_MENU;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    // Send input
    SendInput(4, inputs, sizeof(INPUT));
}

//#include "RPGTypes.h"
//
//class writer {
//    static stringstream _combat_log;
//
//public:
//    static void Print() {
//        const int max_lines = 10;
//        deque<int> line_positions;
//        string content = _combat_log.str();
//        int pos = content.size() - 1;
//
//        // Find the positions of the line endings from the end of the stream
//        while (pos >= 0 && line_positions.size() <= max_lines) {
//            if (content[pos] == '\n') {
//                line_positions.push_front(pos);
//            }
//            --pos;
//        }
//
//        // If we have more than max_lines lines, adjust pos to the start of the 11th line from the end
//        if (line_positions.size() > max_lines) {
//            pos = line_positions[max_lines] + 1;
//        }
//        else {
//            pos = 0;  // Otherwise, print everything
//        }
//
//        // Print the last 10 lines (or all lines if fewer than 10)
//        cout << content.substr(pos) << endl;
//    }
//
//    static stringstream& GetRef() { return _combat_log; }
//};
//
//stringstream writer::_combat_log;
//
//for (int i = 1; i < 5; i++) {
//    auto& a = writer::GetRef();
//    a << "ASD " << i << endl;
//    writer::Print();
//    cout << "----------------" << endl;  // To separate the outputs
//}


#include "Characters/SummonCharacter.h"
#include "Items/ItemData.h"

int main() {


    //Sleep(100);
    //SendAltEnter();

	SpellManager::GetInstance();
	CombatManager::GetInstance();
	MapGenerator::GetInstance();

    auto CharDeleter = [](const PlayerCharacter* Ptr) { PlayerCharacter::nPlayerCharacters--; delete Ptr; };

    const std::shared_ptr<PlayerCharacter> PlayerMain(new PlayerCharacter(ECharacterClass::BARBARIAN), CharDeleter);
    const std::shared_ptr<PlayerCharacter> PlayerWarlock1(new PlayerCharacter(ECharacterClass::WARLOCK), CharDeleter);

    std::vector<std::shared_ptr<Character>> PlayerCharacters = { PlayerMain, PlayerWarlock1 };
	
    using sm = SpellManager;
    sm::CreateActiveSpell(PlayerMain.get(), ESpellID::FIREBALL);
    sm::CreateActiveSpell(PlayerMain.get(), ESpellID::BURNING);
    sm::CreateActiveSpell(PlayerMain.get(), ESpellID::EXPOSURE);
    sm::CreateActiveSpell(PlayerMain.get(), ESpellID::MOLTEN_ARMOR);
    sm::CreateActiveSpell(PlayerMain.get(), ESpellID::SUM_FIRE_ELE);
    sm::CreateActiveSpell(PlayerMain.get(), ESpellID::BLIND);

    // passive
    sm::CreatePassiveSpell(PlayerMain.get(), ESpellID::VAMPIRIC_TOUCH);
	
	ConsoleMenu Menu;
	
    // Later we don't send none of these initializers to GS. 
    // Have to implement a starting routine and a game loop. at the starting routine we get the instances.
	GameplayStatics::Initialize(std::move(PlayerCharacters), Menu);
}


