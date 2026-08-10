#pragma once
#include <conio.h> 
#include <windows.h> 
#include "../RPGTypes.h"

class Item;

// TODO: Static?
class ConsoleMenu {
public:
    ConsoleMenu() = default;
    explicit ConsoleMenu(const std::vector<std::string>& InOptions);

    // Return the index of the selected item in the displayed menu(vector)
    const int& Select();

    inline void SetOptions(const std::vector<std::string>& InOptions) { Options = InOptions; }
    inline void SetUp(const int InUp) { Up = InUp; }
    inline void SetRight(const int InRight) { Right = InRight; }
    inline void SetItems(const std::vector<Item*>& InItems) { Items = InItems; bIsItem = static_cast<bool>(!Items.empty()); }
    inline void SetClear(const bool InbClear) { bClear = InbClear; }

private:
    inline bool GetBack() { return std::exchange(bBack, false); }

public:
    void AnsiCursorDownN(const int N);

    void Clear(int Lines);
    void ClearRight(int Lines);
    void ClearItemInfo(int Lines);

private:
    const int& Prompt();
    void Display();
    void AnsiCursorUpN(int N, const bool Clear);
    void AnsiCursorRightN(const int N);

    std::string GetColor(int OptionIndex, std::string& String1, std::string& String2);
    void DisplayItemInfo(int InItem);

    int InfoLines = 0;

private:
    std::vector<std::string> Options;
    std::vector<std::string> PreviousOptions;
    std::vector<Item*> Items;

    int Index = 0;
    int Up = 0;
    int Right = 0;

    bool bClear = true;
    bool bBack = false;
    bool bIsItem = false;
};
