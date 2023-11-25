#pragma once
#include <conio.h> 
#include <windows.h> 
#include "../RPGTypes.h"

using namespace std;

class ConsoleMenu {
private:
    vector<string> _options;
    vector<string> _last_options;
    int _index = 0;
    int _up = 0;
    int _right = 0;
    bool _clear = true;
    bool _back = false;

private:
    int Prompt();
    void Display();
    void ANSI_CURSOR_UP_N(const int n, const bool clear);
    void ANSI_CURSOR_RIGHT_N(const int n);

public:
    ConsoleMenu(const vector<string>& options);
    ConsoleMenu() {}

    int Select();

    inline void SetOptions(const vector<string>& options) { _options = options; }
    inline void SetUp(const int up) { _up = up; }
    inline void SetRight(const int right) { _right = right; }
    void SetClear(const bool clear) { _clear = clear; }
    inline bool GetBack() { 
        bool b = _back;
        _back = false;
        return b;
    }
    void ANSI_CURSOR_DOWN_N(const int n);
    void Clear(int lines);
    void ClearRight(int lines);
};
