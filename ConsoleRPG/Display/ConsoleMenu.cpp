#include "ConsoleMenu.h"
#include "../Items/Item.h"

ConsoleMenu::ConsoleMenu(const std::vector<std::string>& InOptions)
    : _options(InOptions) 
{}

int ConsoleMenu::Prompt() {

    _index = 0;
    char input;
    do {
        Display();

        input = _getch(); 

        switch (input) {
        case 'A':
        case 'a':
        case 75: {
            if (!_bClear) {
                _up = 0;
                _bBack = true;
            }
            break;
        }
        case 'w':
        case 'W':
        case 72:
            _index = static_cast<int>((_index - 1 + _options.size()) % _options.size());
            break;
        case 's': 
        case 'S':
        case 80:
            _index = static_cast<int>((_index + 1) % _options.size());
            break;
        }
        ANSI_CURSOR_UP_N(_up, _bClear);
        if (_bBack) break;
    } while (input != '\r');  // Enter key to select

    _last_options = _options;

    return _index = _options[_index] == "<--BACK--<" ? -1 : _index;
}

void ConsoleMenu::Display() {

    for (int i = 0; i < static_cast<int>(_options.size()); i++) {
        if (_bIsItem) {
            std::string s1, s2;
            std::string C = GetColor(i, s1, s2);
            
            if ((i == 0 && _options[i] == "--> ALL ITEMS <--") || i == _options.size() - 1) C = COLOR_FG;
            if (i == _index) {
                if (!s1.empty()) std::cout << COLOR_FG << COLOR_BG << s1 << C << s2 << ANSI_COLOR_RESET;
                else std::cout << C << COLOR_BG << _options[i] << ANSI_COLOR_RESET;
            }
            else {
                if (!s1.empty()) std::cout << COLOR_FG << s1 << C << s2 << ANSI_COLOR_RESET;
                else std::cout << C << _options[i];
            }
            std::cout << '\n';
        }
        else {
            if (i == _index) {
                ANSI_CURSOR_RIGHT_N(_right);
                std::cout << COLOR_FG << COLOR_BG;
                std::cout << _options[i];;
                std::cout << ANSI_COLOR_RESET;
            }
            else {
                std::cout << COLOR_FG;
                ANSI_CURSOR_RIGHT_N(_right);
                std::cout << _options[i];
            }
            std::cout << '\n';
        }
    }
    
    if (_bIsItem)
        DisplayItemInfo(_index);
}

int ConsoleMenu::Select() {
    return Prompt();
}

void ConsoleMenu::ANSI_CURSOR_UP_N(int n, const bool clear) {
    for (int i = 0; i < n; i++) {
        if (!clear) std::cout << ANSI_CURSOR_UP(1);
        else if (_bIsItem) std::cout << ANSI_CURSOR_UP(1) << ANSI_CURSOR_RIGHT(120) << ANSI_CLEAR_TO_START << ANSI_CURSOR_LEFT(120);
        else if (clear) std::cout << ANSI_CURSOR_UP(1) << ANSI_CURSOR_RIGHT(35) << ANSI_CLEAR_TO_START << ANSI_CURSOR_LEFT(35); // valjda se left more meknuti
    }
}

void ConsoleMenu::ANSI_CURSOR_RIGHT_N(const int n) {
    for (int i = 0; i < n; i++)
        std::cout << ANSI_CURSOR_RIGHT(1);
}

void ConsoleMenu::ANSI_CURSOR_DOWN_N(const int n) {
    for (int i = 0; i < n; i++)
        std::cout << ANSI_CURSOR_DOWN(1);
}

void ConsoleMenu::Clear(int lines) {
    ANSI_CURSOR_UP_N(lines, true);
}

void ConsoleMenu::ClearRight(int lines) {
    for (int i = 0; i < lines; i++)
        std::cout << ANSI_CURSOR_UP(1) << CURSOR_LOG_RIGHT << ANSI_CLEAR_TO_END << CURSOR_LOG_LEFT;
}

void ConsoleMenu::ClearItemInfo(int lines) {
    for (int i = 0; i < lines; i++)
        std::cout << ANSI_CURSOR_RIGHT(60) << ANSI_CLEAR_TO_END << ANSI_CURSOR_UP(1);
}

std::string ConsoleMenu::GetColor(int i, std::string& s1, std::string& s2) {
    int offset;
    if ((offset = static_cast<int>(_options[i].find("---> "))) != std::string::npos) {
        s1 = _options[i].substr(0, offset + 5);
        s2 = _options[i].substr(offset + 5);
    }

    std::string C;
    if (_options[i] == "--> ALL ITEMS <--" || _options[i] == "<--BACK--<") return COLOR_FG;
    int off = _options[0] == "--> ALL ITEMS <--" ? 1 : 0;
    if (!_items[i - off]) return COLOR_FG;

    switch (_items[i - off]->ItemInfo.ItemRarity) {
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

void ConsoleMenu::DisplayItemInfo(int i) {
    int off = _options[0] == "--> ALL ITEMS <--" ? 1 : 0;
    if (i - off >= _items.size()) return;
    if (i - off < 0 || !_items[i - off]) return;

    std::string s1, s2;
    const std::string C = GetColor(i, s1, s2);
    std::string name = _items[i - off]->ItemInfo.Name;
    int offset = 60 - name.length();
    offset /= 2;

    _info_lines = 3;

    ANSI_CURSOR_UP_N(_options.size(), false); 
    std::cout << C;
    std::cout << ANSI_CURSOR_RIGHT(60);
    std::cout << "------------------------------------------------------------\n";
    std::cout << ANSI_CURSOR_RIGHT(60) << "===";
    for (int i = 3; i < 58; i++)
        if (i >= offset && name.length() >= i - offset) std::cout << name[i - offset];
        else std::cout << ' ';
    std::cout << "===" << '\n';
    std::cout << ANSI_CURSOR_RIGHT(60);
    std::cout << "------------------------------------------------------------\n" << ANSI_COLOR_RESET;
 
    if (_options.size() <= 3)
        ANSI_CURSOR_UP_N(3 - _options.size(), false);
    else ANSI_CURSOR_DOWN_N(_options.size() - 3);
}