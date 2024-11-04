#pragma once
#include <conio.h> 
#include <windows.h> 
#include "../RPGTypes.h"

class Item;

class ConsoleMenu {

public:
    ConsoleMenu(const std::vector<std::string>& InOptions);
    ConsoleMenu() = default;

    // Return the index of the selected item in the displayed menu(vector)
    int Select();

    inline void SetOptions(const std::vector<std::string>& InOptions) { _options = InOptions; }
    inline void SetUp(const int Up) { _up = Up; }
    inline void SetRight(const int Right) { _right = Right; }
    inline void SetItems(const std::vector<Item*>& Items) { _items = Items; _bIsItem = static_cast<bool>(!_items.empty()); }
    void SetClear(const bool Clear) { _bClear = Clear; }

    inline bool GetBack() {
        bool b = _bBack;
        _bBack = false;
        return b;
    }

    void ANSI_CURSOR_DOWN_N(const int n);

    void Clear(int lines);
    void ClearRight(int lines);
    void ClearItemInfo(int lines);

private:
    int Prompt();

    void Display();

    void ANSI_CURSOR_UP_N(int n, const bool clear);
    void ANSI_CURSOR_RIGHT_N(const int n);

    std::string GetColor(int i, std::string& s1, std::string& s2);

    void DisplayItemInfo(int i);

    int _info_lines = 0;

private:
    std::vector<std::string> _options;
    std::vector<std::string> _last_options;

    std::vector<Item*> _items;

    int _index = 0;
    int _up = 0;
    int _right = 0;

    bool _bClear = true;

    bool _bBack = false;

    bool _bIsItem = false;
};
