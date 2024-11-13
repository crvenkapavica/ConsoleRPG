#include "ConsoleMenu.h"
#include "../Items/Item.h"

ConsoleMenu::ConsoleMenu(const std::vector<std::string>& InOptions)
    : Options(InOptions) 
{}

int ConsoleMenu::Prompt() {
    Index = 0;
    char Input;
    do {
        Display();
        Input = static_cast<char>(_getch()); 
        switch (Input) {
        case 'A':
        case 'a':
        case 75: {
            if (!bClear) {
                Up = 0;
                bBack = true;
            }
            break;
        }
        case 'w':
        case 'W':
        case 72:
            Index = static_cast<int>((Index - 1 + Options.size()) % Options.size());
            break;
        case 's': 
        case 'S':
        case 80:
            Index = static_cast<int>((Index + 1) % Options.size());
            break;
        }
        AnsiCursorUpN(Up, bClear);
        if (bBack) break;
    } while (Input != '\r');  // Enter key to select

    PreviousOptions = Options;

    return Index = Options[Index] == "<--BACK--<" ? -1 : Index;
}

void ConsoleMenu::Display() {
    for (int i = 0; i < static_cast<int>(Options.size()); i++) {
        if (bIsItem) {
            std::string s1, s2;
            std::string C = GetColor(i, s1, s2);
            
            if ((i == 0 && Options[i] == "--> ALL ITEMS <--") || i == Options.size() - 1) C = COLOR_FG;
            if (i == Index) {
                if (!s1.empty()) std::cout << COLOR_FG << COLOR_BG << s1 << C << s2 << ANSI_COLOR_RESET;
                else std::cout << C << COLOR_BG << Options[i] << ANSI_COLOR_RESET;
            }
            else {
                if (!s1.empty()) std::cout << COLOR_FG << s1 << C << s2 << ANSI_COLOR_RESET;
                else std::cout << C << Options[i];
            }
            std::cout << '\n';
        }
        else {
            if (i == Index) {
                AnsiCursorRightN(Right);
                std::cout << COLOR_FG << COLOR_BG;
                std::cout << Options[i];;
                std::cout << ANSI_COLOR_RESET;
            }
            else {
                std::cout << COLOR_FG;
                AnsiCursorRightN(Right);
                std::cout << Options[i];
            }
            std::cout << '\n';
        }
    }
    
    if (bIsItem)
        DisplayItemInfo(Index);
}

int ConsoleMenu::Select() {
    return Prompt();
}

void ConsoleMenu::AnsiCursorUpN(const int N, const bool Clear) {
    for (int i = 0; i < N; i++) {
        if (!Clear) std::cout << ANSI_CURSOR_UP(1);
        else if (bIsItem) std::cout << ANSI_CURSOR_UP(1) << ANSI_CURSOR_RIGHT(120) << ANSI_CLEAR_TO_START << ANSI_CURSOR_LEFT(120);
        else if (Clear) std::cout << ANSI_CURSOR_UP(1) << ANSI_CURSOR_RIGHT(35) << ANSI_CLEAR_TO_START << ANSI_CURSOR_LEFT(35); // Remove left?
    }
}

void ConsoleMenu::AnsiCursorRightN(const int N) {
    for (int i = 0; i < N; i++)
        std::cout << ANSI_CURSOR_RIGHT(1);
}

void ConsoleMenu::AnsiCursorDownN(const int N) {
    for (int i = 0; i < N; i++)
        std::cout << ANSI_CURSOR_DOWN(1);
}

void ConsoleMenu::Clear(const int Lines) {
    AnsiCursorUpN(Lines, true);
}

void ConsoleMenu::ClearRight(const int Lines) {
    for (int i = 0; i < Lines; i++)
        std::cout << ANSI_CURSOR_UP(1) << CURSOR_LOG_RIGHT << ANSI_CLEAR_TO_END << CURSOR_LOG_LEFT;
}

void ConsoleMenu::ClearItemInfo(const int Lines) {
    for (int i = 0; i < Lines; i++)
        std::cout << ANSI_CURSOR_RIGHT(60) << ANSI_CLEAR_TO_END << ANSI_CURSOR_UP(1);
}

std::string ConsoleMenu::GetColor(const int OptionIndex, INOUT std::string& String1, INOUT std::string& String2) {
    if (int Offset; (Offset = static_cast<int>(Options[OptionIndex].find("---> "))) != static_cast<int>(std::string::npos)) {
        String1 = Options[OptionIndex].substr(0, Offset + 5);
        String2 = Options[OptionIndex].substr(Offset + 5);
    }

    std::string C;
    if (Options[OptionIndex] == "--> ALL ITEMS <--" || Options[OptionIndex] == "<--BACK--<") return COLOR_FG;
    const int Offset = Options[0] == "--> ALL ITEMS <--" ? 1 : 0;
    if (!Items[OptionIndex - Offset]) return COLOR_FG;

    switch (Items[OptionIndex - Offset]->ItemInfo.ItemRarity) {
    case EItemRarity::COMMON:
        return COLOR_COMMON;
    case EItemRarity::EPIC:
        return COLOR_EPIC;
    case EItemRarity::LEGENDARY:
        return COLOR_LEGENDARY;
    case EItemRarity::GODLIKE:
        return COLOR_GODLIKE;
    case EItemRarity::UNIQUE:
        return COLOR_UNIQUE;
    default:
        return COLOR_FG;
    }
}

void ConsoleMenu::DisplayItemInfo(const int InItem) {
    int Offset = Options[0] == "--> ALL ITEMS <--" ? 1 : 0;
    if (InItem - Offset >= static_cast<int>(Items.size())) return;
    if (InItem - Offset < 0 || !Items[InItem - Offset]) return;

    std::string String1, String2;
    const std::string C = GetColor(InItem, String1, String2);
    const std::string Name = Items[InItem - Offset]->ItemInfo.Name;
    Offset = 60 - static_cast<int>(Name.length());
    Offset /= 2;
    InfoLines = 3;

    AnsiCursorUpN(static_cast<int>(Options.size()), false); 
    std::cout << C;
    std::cout << ANSI_CURSOR_RIGHT(60);
    std::cout << "------------------------------------------------------------\n";
    std::cout << ANSI_CURSOR_RIGHT(60) << "===";
    for (int Col = 3; Col < 58; Col++)
        if (Col >= Offset && static_cast<int>(Name.length()) >= Col - Offset) std::cout << Name[Col - Offset];
        else std::cout << ' ';
    std::cout << "===" << '\n';
    std::cout << ANSI_CURSOR_RIGHT(60);
    std::cout << "------------------------------------------------------------\n" << ANSI_COLOR_RESET;
 
    if (Options.size() <= 3)
        AnsiCursorUpN(3 - static_cast<int>(Options.size()), false);
    else AnsiCursorDownN(static_cast<int>(Options.size()) - 3);
}