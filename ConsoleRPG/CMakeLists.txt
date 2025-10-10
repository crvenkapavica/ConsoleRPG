set(PROJECT_NAME ConsoleRPG)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Characters/CharacterData.h"
    "Characters/SummonCharacter.h"
    "Items/Item.h"
    "Items/ItemData.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Characters/SummonCharacter.cpp"
    "GameplayStatics.cpp"
    "GameplayStatics.h"
    "Items/Item.cpp"
    "Items/ItemData.cpp"
    "Resistances.cpp"
    "Resistances.h"
    "RPGTypes.h"
    "Source.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__Characters
    "Characters/Character.cpp"
    "Characters/Character.h"
    "Characters/CharacterData.cpp"
    "Characters/EnemyCharacter.cpp"
    "Characters/EnemyCharacter.h"
    "Characters/PlayerCharacter.cpp"
    "Characters/PlayerCharacter.h"
)
source_group("Source Files\\Characters" FILES ${Source_Files__Characters})

set(Source_Files__Combat
    "Combat/CombatManager.cpp"
    "Combat/CombatManager.h"
)
source_group("Source Files\\Combat" FILES ${Source_Files__Combat})

set(Source_Files__Display
    "Display/ConsoleMenu.cpp"
    "Display/ConsoleMenu.h"
)
source_group("Source Files\\Display" FILES ${Source_Files__Display})

set(Source_Files__Effects
    "Spells/ActiveSpell.cpp"
    "Spells/ActiveSpell.h"
    "Spells/EffectStructs.h"
    "Spells/PassiveSpell.cpp"
    "Spells/PassiveSpell.h"
    "Spells/Spell.cpp"
    "Spells/Spell.h"
)
source_group("Source Files\\Effects" FILES ${Source_Files__Effects})

set(Source_Files__MapGenerator
    "MapGenerator/MapGenerator.cpp"
    "MapGenerator/MapGenerator.h"
)
source_group("Source Files\\MapGenerator" FILES ${Source_Files__MapGenerator})

set(Source_Files__Spells
    "Spells/SpellData.cpp"
    "Spells/SpellData.h"
    "Spells/SpellManager.cpp"
    "Spells/SpellManager.h"
)
source_group("Source Files\\Spells" FILES ${Source_Files__Spells})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    ${Source_Files__Characters}
    ${Source_Files__Combat}
    ${Source_Files__Display}
    ${Source_Files__Effects}
    ${Source_Files__MapGenerator}
    ${Source_Files__Spells}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE ConsoleRPG)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

