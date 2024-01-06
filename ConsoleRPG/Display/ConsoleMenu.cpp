#include "ConsoleMenu.h"

ConsoleMenu::ConsoleMenu(const std::vector<string>& options)
    : _options(options) 
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

    for (int i = 0; i < _options.size(); i++) {
        if (_bIsItem) {
            string rarity = "";
            vector<string> v = { "empty", "Rare", "Epic", "Legendary", "GODLIKE", "UNIQUE"};
            for (int j = 0; j < v.size(); j++)
                if (_options[i].find(v[j]) != std::string::npos)
                    rarity = v[j];

            string s1 = "", s2 = "";
            int offset;
            if ((offset = _options[i].find(" --> ")) != std::string::npos) {
                s1 = _options[i].substr(0, offset + 5);
                s2 = _options[i].substr(offset + 5);
            }

            string C;
                 if (rarity == "empty")     C = COLOR_FG;
            else if (rarity == "Rare")      C = COLOR_RARE;
            else if (rarity == "Epic")      C = COLOR_EPIC;
            else if (rarity == "Legendary") C = COLOR_LEGENDARY;
            else if (rarity == "GODLIKE")   C = COLOR_GODLIKE;
            else if (rarity == "UNIQUE")    C = COLOR_UNIQUE;
            else                            C = COLOR_COMMON;
            
            if ((i == 0 && _options[i] == "--> ALL ITEMS <--") || i == _options.size() - 1) C = COLOR_FG;
            if (i == _index) {
                if (s1.size()) std::cout << COLOR_FG << COLOR_BG << s1 << C << s2 << ANSI_COLOR_RESET;
                else std::cout << C << COLOR_BG << _options[i] << ANSI_COLOR_RESET;
            }
            else {
                if (s1.size()) std::cout << COLOR_FG << s1 << C << s2 << ANSI_COLOR_RESET;
                else std::cout << C << _options[i];
            }
            cout << endl;
        }
        else {
            if (i == _index) {
                ANSI_CURSOR_RIGHT_N(_right);
                cout << COLOR_FG << COLOR_BG;
                cout << _options[i];;
                cout << ANSI_COLOR_RESET;
            }
            else {
                cout << COLOR_FG;
                ANSI_CURSOR_RIGHT_N(_right);
                cout << _options[i];
            }
            cout << endl;
        }
    }
}

int ConsoleMenu::Select() {
    return Prompt();
}

void ConsoleMenu::ANSI_CURSOR_UP_N(const int n, const bool clear) {
    for (int i = 0; i < n; i++) {
        if (_bIsItem) cout << ANSI_CURSOR_UP(1) << ANSI_CURSOR_RIGHT(85) << ANSI_CLEAR_TO_START << ANSI_CURSOR_LEFT(85);
        else if (clear) cout << ANSI_CURSOR_UP(1) << ANSI_CURSOR_RIGHT(35) << ANSI_CLEAR_TO_START << ANSI_CURSOR_LEFT(35); // valjda se left more meknuti
        if (!clear) cout << ANSI_CURSOR_UP(1);
    }
}

void ConsoleMenu::ANSI_CURSOR_RIGHT_N(const int n) {
    for (int i = 0; i < n; i++)
       cout << ANSI_CURSOR_RIGHT(1);
}

void ConsoleMenu::ANSI_CURSOR_DOWN_N(const int n) {
    for (int i = 0; i < n; i++)
        cout << ANSI_CURSOR_DOWN(1);
}

void ConsoleMenu::Clear(int lines) {
    ANSI_CURSOR_UP_N(lines, true);
}

void ConsoleMenu::ClearRight(int lines) {
    for (int i = 0; i < lines; i++)
        cout << ANSI_CURSOR_UP(1) << CURSOR_LOG_RIGHT << ANSI_CLEAR_TO_END << CURSOR_LOG_LEFT;
}