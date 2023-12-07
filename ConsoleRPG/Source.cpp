#pragma once

#include "RPGTypes.h"
#include "Characters/Character.h"
#include "Characters/CharacterData.h"
#include "MapGenerator/MapGenerator.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "Spells/SpellBook.h"
#include "Spells/SpellDB.h"
#include "GameplayStatics.h"
#include "Spells/SpellManager.h"
#include "Combat/CombatManager.h"

using namespace std;

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


#include "Inventory/ItemData.h"
int main() {


    //Sleep(100);
    //SendAltEnter();

    vector<shared_ptr<PlayerCharacter>> player_characters;

    CharacterData barbarian_data(ECharacterClass::BARBARIAN);
    auto player_main = make_shared<PlayerCharacter>(barbarian_data.GetPlayerAttributeData());

    CharacterData warlock_data(ECharacterClass::WARLOCK);
    auto player_warlock = make_shared<PlayerCharacter>(warlock_data.GetPlayerAttributeData());
    player_warlock->SetAlias('1');

    player_characters = { player_main, player_warlock };

    SpellManager& spell_manager = SpellManager::GetInstance();

	spell_manager.CreateSpellBook(player_main.get(), ESpellBookID::FIREBALL, 0);
	spell_manager.CreateSpellBook(player_main.get(), ESpellBookID::STONESKIN, 0);
    spell_manager.CreateSpellBook(player_main.get(), ESpellBookID::ARCANE_INFUSION, 0);
    spell_manager.CreateSpellBook(player_main.get(), ESpellBookID::BLOOD_RAIN, 0);
    spell_manager.CreateSpellBook(player_main.get(), ESpellBookID::VISCOUS_ACID, 0);
    spell_manager.CreateSpellBook(player_warlock.get(), ESpellBookID::STONESKIN, 0);

    //pasive
    //spell_manager.CreatePassiveSpell(player_main.get(), ESpellID::VAMPIRIC_TOUCH);

    CombatManager& combat_manager = CombatManager::GetInstance();
	ConsoleMenu menu;
	MapGenerator map_generator;

	GameplayStatics::Initialize(player_characters, &spell_manager, &combat_manager, &map_generator, &menu);
}


