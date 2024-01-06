#pragma once
#include <conio.h> 
#include <windows.h> 
#include "../RPGTypes.h"

using namespace std; // TODO - remove

class ConsoleMenu {

public:
    ConsoleMenu(const std::vector<string>& options);
    ConsoleMenu() {}

    // Return the index of the selected item in the displayed menu(vector)
    int Select();

    inline void SetOptions(const std::vector<string>& options) { _options = options; }
    inline void SetUp(const int up) { _up = up; }
    inline void SetRight(const int right) { _right = right; }
    inline void SetIsItem(const bool bIsItem) { _bIsItem = bIsItem; }
    void SetClear(const bool clear) { _bClear = clear; }

    inline bool GetBack() {
        bool b = _bBack;
        _bBack = false;
        return b;
    }

    void ANSI_CURSOR_DOWN_N(const int n);

    void Clear(int lines);
    void ClearRight(int lines);

private:
    int Prompt();

    void Display();

    void ANSI_CURSOR_UP_N(const int n, const bool clear);
    void ANSI_CURSOR_RIGHT_N(const int n);

private:
    std::vector<string> _options;
    std::vector<string> _last_options;

    int _index = 0;
    int _up = 0;
    int _right = 0;

    bool _bClear = true;

    bool _bBack = false;

    bool _bIsItem = false;
};
