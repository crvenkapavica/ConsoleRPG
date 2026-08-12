cmake_minimum_required(VERSION 4.3)

set(CONSOLERPG_MSVC_2026_TOOLCHAIN
    "${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/msvc-2026-x64.cmake"
)

if (WIN32 AND EXISTS "${CONSOLERPG_MSVC_2026_TOOLCHAIN}")
    include("${CONSOLERPG_MSVC_2026_TOOLCHAIN}")
endif()

project(ConsoleRPG LANGUAGES CXX)

add_executable(ConsoleRPG
    Source.cpp

    Characters/Character.cpp
    Characters/Character.h
    Characters/CharacterData.cpp
    Characters/CharacterData.h
    Characters/EnemyCharacter.cpp
    Characters/EnemyCharacter.h
    Characters/PlayerCharacter.cpp
    Characters/PlayerCharacter.h
    Characters/SummonCharacter.cpp
    Characters/SummonCharacter.h

    Combat/CombatManager.cpp
    Combat/CombatManager.h

    Display/ConsoleMenu.cpp
    Display/ConsoleMenu.h

    Items/Item.cpp
    Items/Item.h
    Items/ItemData.cpp
    Items/ItemData.h

    MapGenerator/MapGenerator.cpp
    MapGenerator/MapGenerator.h

    Spells/ActiveSpell.cpp
    Spells/ActiveSpell.h
    Spells/EffectStructs.h
    Spells/PassiveSpell.cpp
    Spells/PassiveSpell.h
    Spells/Spell.cpp
    Spells/Spell.h
    Spells/SpellData.cpp
    Spells/SpellData.h
    Spells/SpellManager.cpp
    Spells/SpellManager.h

    GameplayStatics.cpp
    GameplayStatics.h
    Resistances.cpp
    Resistances.h
    RPGTypes.h
)

set_target_properties(ConsoleRPG PROPERTIES
    CXX_EXTENSIONS NO
)

target_include_directories(ConsoleRPG PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

if (MSVC)
    target_include_directories(ConsoleRPG SYSTEM PRIVATE
        ${CONSOLERPG_MSVC_2026_INCLUDE_DIRS}
    )
    set_property(TARGET ConsoleRPG PROPERTY MSVC_DEBUG_INFORMATION_FORMAT ProgramDatabase)

    # CMake 4.3.1 does not advertise cxx_std_26 for MSVC 19.50 yet.
    # MSVC's forward-looking C++26 mode is selected with /std:c++latest.
    target_compile_options(ConsoleRPG PRIVATE
        /std:c++latest
        /permissive-
        /W4
        /utf-8
        /Zc:__cplusplus
        /EHsc
    )
    target_compile_definitions(ConsoleRPG PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
    )
else()
    target_compile_features(ConsoleRPG PRIVATE cxx_std_26)
    set_target_properties(ConsoleRPG PROPERTIES
        CXX_STANDARD 26
        CXX_STANDARD_REQUIRED YES
    )
    target_compile_options(ConsoleRPG PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

install(TARGETS ConsoleRPG
    RUNTIME DESTINATION bin
)
