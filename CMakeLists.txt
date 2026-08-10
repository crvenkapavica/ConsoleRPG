#set(CMAKE_C_COMPILER "A:/Visual Studio 2026/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe")
#set(CMAKE_CXX_COMPILER "A:/Visual Studio 2026/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe")
project(ConsoleRPG LANGUAGES CXX)
cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
#        AND MSVC)                                # clang-cl on Windows
#    set(CMAKE_LINKER "link.exe" CACHE FILEPATH "")
#    # or per-target:
#    # set_target_properties(ConsoleRPG PROPERTIES LINKER_LANGUAGE CXX)
#    # target_link_options(ConsoleRPG PRIVATE -fuse-ld=link)
#endif()

add_executable(ConsoleRPG
        Source.cpp
        # ─ Characters ─
        Characters/Character.cpp            Characters/Character.h
        Characters/CharacterData.cpp        Characters/CharacterData.h
        Characters/EnemyCharacter.cpp       Characters/EnemyCharacter.h
        Characters/PlayerCharacter.cpp      Characters/PlayerCharacter.h
        Characters/SummonCharacter.cpp      Characters/SummonCharacter.h
        # ─ Combat ─
        Combat/CombatManager.cpp            Combat/CombatManager.h
        # ─ Display ─
        Display/ConsoleMenu.cpp             Display/ConsoleMenu.h
        # ─ Items ─
        Items/Item.cpp                      Items/Item.h
        Items/ItemData.cpp                  Items/ItemData.h
        # ─ Map generation ─
        MapGenerator/MapGenerator.cpp       MapGenerator/MapGenerator.h
        # ─ Spells ─
        Spells/ActiveSpell.cpp              Spells/ActiveSpell.h
        Spells/PassiveSpell.cpp             Spells/PassiveSpell.h
        Spells/Spell.cpp                    Spells/Spell.h
        Spells/SpellData.cpp                Spells/SpellData.h
        Spells/SpellManager.cpp             Spells/SpellManager.h
        Spells/EffectStructs.h
        # ─ Misc ─
        GameplayStatics.cpp                 GameplayStatics.h
        Resistances.cpp                     Resistances.h
        RPGTypes.h
)

# ── Baseline language level ──────────────────────────────────
# Ask CMake for C++23 so its feature-tests work everywhere
# set_target_properties(ConsoleRPG PROPERTIES
#     CXX_STANDARD 23
#     CXX_STANDARD_REQUIRED ON
#     CMAKE_CXX_EXTENSIONS  OFF
#     CMAKE_CXX_FLAGS "-std=c++26 -Wall -Wextra -Wpedantic")

# ── Extra flags to unlock “C++26” features on compilers that already have them ─
#  • MSVC  : /std:c++latest
#  • GCC/Clang : -std=c++2c (or -std=c++2b on older toolchains)
if (MSVC)
    target_compile_options(ConsoleRPG PRIVATE /std:c++latest)
else()
    # Change -std=c++2c → -std=c++2b if your compiler hasn’t renamed the flag yet
    target_compile_options(ConsoleRPG PRIVATE -std=c++2b)
endif()

# ── Warnings (portable) ──────────────────────────────────────
if (MSVC)
    target_compile_options(ConsoleRPG PRIVATE /W4 /permissive-)
else()
    target_compile_options(ConsoleRPG PRIVATE -Wall -Wextra -pedantic)
endif()


include(GNUInstallDirs)
# ── Private include directories (keeps your project self-contained) ─────────
target_include_directories(ConsoleRPG PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})
#Characters Combat Display Items MapGenerator Spells)
install(TARGETS ConsoleRPG
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


