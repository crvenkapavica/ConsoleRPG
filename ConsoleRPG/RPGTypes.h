#pragma once

#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <mutex>
#include <list>
#include <bits.h>
#include <string>
#include <optional>
#include <random>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <deque>
#include <cctype>

typedef int Attribute;
typedef int Skill;

class Character;

#undef _stat
#undef max
#undef PURE

#define START_X 8
#define START_Y 8
#define MAX_X 100
#define MAX_Y 100

#define GRID_X 21  // decrease by 4 to change grid height
#define GRID_Y 81 // decrease by 8 to change grid width
#define CHAR_GRID_X ((GRID_X - 1) / 4)
#define CHAR_GRID_Y ((GRID_Y - 1) / 8)

#define MAX_LVL 99
#define NUM_PLYR_CLASSES 1
#define NUM_ENEMY_CLASSES 1

#define ITEM_TYPES 6
#define ITEM_RARITIES 6

#define ANSI_COLOR_RESET		"\x1b[0m"
#define ANSI_COLOR_BOLD			"\033[1m"
#define ANSI_COLOR_UNDERLINE	"\033[4m"
#define ANSI_COLOR_BLACK		"\033[30m"
#define ANSI_COLOR_RED			"\x1b[31m"
#define ANSI_COLOR_GREEN		"\x1b[32m"
#define ANSI_COLOR_YELLOW		"\x1b[33m"
#define ANSI_COLOR_BLUE			"\x1b[34m"
#define ANSI_COLOR_MAGENTA		"\x1b[35m"
#define ANSI_COLOR_CYAN			"\x1b[36m"
#define ANSI_COLOR_WHITE		"\033[37m"

#define ANSI_BG_COLOR_BLACK     "\x1b[40m"
#define ANSI_BG_COLOR_RED		"\x1b[41m"
#define ANSI_BG_COLOR_GREEN     "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW    "\x1b[43m"
#define ANSI_BG_COLOR_BLUE      "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA   "\x1b[45m"
#define ANSI_BG_COLOR_CYAN      "\x1b[46m"
#define ANSI_BG_COLOR_WHITE     "\x1b[47m"

#define ANSI_COLOR_BLINK		"\x1b[5m"

#define ANSI_COLOR_FG_RGB(r, g, b) "\x1b[38;2;" #r ";" #g ";" #b "m"
#define ANSI_COLOR_BG_RGB(r, g, b) "\x1b[48;2;" #r ";" #g ";" #b "m"

#define ANSI_COLOR_CYAN_LIGHT		ANSI_COLOR_FG_RGB(9, 171, 182)
#define ANSI_COLOR_BROWN_LIGHT		ANSI_COLOR_FG_RGB(83, 45, 0)
#define ANSI_COLOR_PASTELLE_YELLOW	ANSI_COLOR_FG_RGB(56, 56, 0)
#define ANSI_COLOR_VIVID_GREEN		ANSI_COLOR_FG_RGB(24, 192, 27)
#define ANSI_COLOR_VIVID_YELLOW		ANSI_COLOR_FG_RGB(249, 192, 27)
#define ANSI_COLOR_PASTELLE_BROWN	ANSI_COLOR_FG_RGB(88, 79, 2)
#define ANSI_COLOR_SKIN				ANSI_COLOR_FG_RGB(246, 154, 129)
#define ANSI_COLOR_PASTELLE_PURPLE	ANSI_COLOR_FG_RGB(173, 172, 230)
#define ANSI_COLOR_ORANGE			ANSI_COLOR_FG_RGB(255, 121, 0)

#define COLOR_COMBAT_LOG		ANSI_COLOR_PASTELLE_PURPLE
#define COLOR_INFO				ANSI_COLOR_ORANGE
#define COLOR_PLAYER			ANSI_COLOR_VIVID_GREEN
#define COLOR_ENEMY				ANSI_COLOR_RED
#define COLOR_EFFECT			ANSI_COLOR_ORANGE
#define COLOR_VALUE				ANSI_COLOR_PASTELLE_BROWN

#define CV	COLOR_VALUE
#define CI  COLOR_INFO
#define CE	COLOR_ENEMY
#define CEF COLOR_EFFECT
#define CP	COLOR_PLAYER
#define CC	COLOR_COMBAT_LOG

#define ANSI_CURSOR_UP(n)		"\x1b[" #n "A"
#define ANSI_CURSOR_DOWN(n)		"\x1b[" #n "B"
#define ANSI_CURSOR_RIGHT(n)	"\x1b[" #n "C"
#define ANSI_CURSOR_LEFT(n)		"\x1b[" #n "D"

#define ANSI_CLEAR_LINE			"\x1b[2K"
#define ANSI_CLEAR_TO_END		"\x1b[K"
#define ANSI_CLEAR_TO_START		"\x1b[1K"

#define CURSOR_LOG_RIGHT		ANSI_CURSOR_RIGHT(90)
#define CURSOR_LOG_LEFT			ANSI_CURSOR_LEFT(90)

#define OBSTACLE 'O'
#define PATH '.'
#define RECTANGLE ','
#define PLAYER '@'
#define ENEMY 'X'

#define OUT
#define INOUT

#define UPPER(c) ((c) >= 'a' && (c) <= 'z' ? static_cast<char>((c) - 32) : (c))

enum class EStatType {
	ANY,
	HEALTH
};

enum class EStatMod {
	CONSTANT,
	ADDITIVE
};

enum class EActionType {
	MELEE,
	RANGED,
	SPELL,
	EFFECT,
	MOVEMENT
};

enum class ESpellID {
	NONE,
	FIREBALL,
	STONESKIN,
	ARCANE_INFUSION,
	BLOOD_RAIN,
	VISCOUS_ACID
};

enum class ESpellType {
	NONE,
	BUFF,
	DEBUFF,
	AURA,
	PROJECTILE
};

enum class ESpellActivity {
	NONE,
	PASSIVE,
	ACTIVE
};

enum class EDamageType {
	NONE,
	ARCANE,
	FIRE,
	LIGHTNING,
	COLD,
	POISON,
	NECROTIC,
	PHYSICAL,
	HEALING,
	PURE,
};

enum class EResistanceType {
	ARCANE,
	FIRE,
	LIGHTNING,
	COLD,
	POISON,
	NECROTIC
};

enum class ESpellRarity {
	BASIC,
	ENHANCED,
	RARE,
	EPIC,
	LEGENDARY,
	GODLIKE
};


enum class ECharacterClass {
	NONE,
	BARBARIAN,
	WARLOCK,
	MAGE,
	ENEMY_LVL1 = 50,
	ENEMY_LVL2,
	ENEMY_LVL3,
	ENEMY_LVL4,
	ENEMY_LVL5,
	ENEMY_LVL6,
	ENEMY_LVL7,
	ENEMY_LVL8,
	ENEMY_LVL9,
	ENEMY_LVL10,
	ENEMY_LVL11,
	ENEMY_LVL12,
	ENEMY_LVL13,
	ENEMY_LVL14,
	ENEMY_LVL15,
	ENEMY_LVL16,
	ENEMY_LVL17,
	ENEMY_LVL18,
	ENEMY_LVL19,
	ENEMY_LVL20,
	ENEMY_SWORDSMAN_LVL1,
	ENEMY_SWORDSMAN_LVL2,
	ENEMY_ARCHER_LVL1,
	ENEMY_ARCHER_LVL2,
	ENEMY_MAGE_LVL1,
	ENEMY_MAGE_LVL2,
	ENEMY_WARLOCK_LVL1,
	ENEMY_WARLOCK_LVL2
};

enum class EDirection {
	NORTH = 0,
	NORTHEAST, 
	EAST, 
	SOUTHEAST, 
	SOUTH, 
	SOUTHWEST, 
	WEST, 
	NORTHWEST
};

enum class EEffectID {
	NONE,
	FIREBALL,
	BURNING,
	MOLTEN_ARMOR,
	EXPOSURE,
	STONESKIN,
	DISARM,
	THRONS,
	BLOODBATH,
	ARCANE_INFUSION,
	AI_TEMP1,
	AI_TEMP2,
	AI_TEMP3,
	BLOOD_RAIN,
	BR_TEMP1,
	BR_TEMP2,
	BR_TEMP3,
	VISCOUS_ACID,
	VA_TEMP1,
	VA_TEMP2,
	VA_TEMP3,


	// PASSIVES
	VAMPIRIC_TOUCH
};

enum class EEffectEvent {
	ON_APPLY,
	ON_SKIP_APPLY,
	ON_COMBAT_BEGIN,
	ON_COMBAT_END,
	ON_TURN_BEGIN,
	ON_TURN_END,
	ON_CYCLE_BEGIN,
	ON_CYCLE_END,
	ON_MELEE_BEGIN,
	ON_MELEE_END,
	ON_RANGE_BEGIN,
	ON_RANGE_END,
	ON_MELEE_RECEIVED_BEGIN,
	ON_MELEE_RECEIVED_END,
	ON_RANGE_RECEIVED_BEGIN,
	ON_RANGE_RECEIVED_END,
	ON_CAST_BEGIN,
	ON_CAST_END
};

enum class EEffectValueAction {
	DEFAULT,
	UPDATE_BASE,
	UPDATE_ACTUAL,
	UPDATE_BONUS
};

enum class EEffectDamageType {
	BURNING,
	PHYSICAL,
	PURE,
};

enum class EEffectType {
	PASSIVE,
	ITEM
};

enum EStructFlags {
	EFFECT_MULTI_TARGET = 1,
	EFFECT_STAT = 2,
	EFFECT_RES = 4
};

enum class EItemID {
	//consumables
	HPotion,
	EPotion,
	FortElixir,
	StrElixir,
	AgiElixir,

	//scrolls
	Scroll,

	//armor
	//head
	ClothCap,
	LeatherHood,
	RusticHelm,
	IronCasque,
	HardColf,
	GoldDiadem,
	WarCasque,
	PhoCrown,
	CelDiadem,
	CrownCosmos,
	//Chest
	ClothTunic,
	PadVest,
	StudLeather,
	HardCuirass,
	ScaleMail,
	KnightPlate,
	DraghideArm,
	PhoMail,
	CelAegis,
	ArmAncient,
	//Hands
	ClothWraps,
	StudGauntlets,
	IronBracers,
	ChainGloves,
	SilvGauntlets,
	KnightVambrace,
	TitanGrasp,
	PhoTalons,
	CelHandwraps,
	InfHandGrds,
	//Belt
	WovenSash,
	LeatherBelt,
	IronBuckBelt,
	ReinfGirdle,
	ChainLBelt,
	PlatedWaist,
	DragBelt,
	PhoWaistwrap,
	AncGirdle,
	InfWaistguard,
	//Legs
	ThistTrous,
	IronGreaves,
	RunicTrBoots,
	MoonLegg,
	PhantStriders,
	TempWalkers,
	TitanMarch,
	AbStrBoots,
	PyrWarKilt,
	CosmosGreaves

	//jewl


	//weapon

	//relic
};

enum class EItemSlot {
	NONE = -1,
	HEAD = 0,
	CHEST,
	HANDS,
	BELT,
	LEGS,
	FEET,
	NECK = 10,
	FINGER1,
	FINGER2,
	WEAPON_MAIN = 20,
	WEAPON_OFF,
	RELIC = 30
};

enum class EWeaponType {
	NONE = -1,
	BOW = 0,
	FIRST_2H = BOW,
	STAFF,		
	AXE_2H,
	MACE_2H,
	SWORD_2H,
	LAST_2H = SWORD_2H,
	AXE_1H,
	FIRST_1H = AXE_1H,
	MACE_1H,
	SWORD_1H,
	DAGGER,
	SHIELD,
	LAST_1H = SHIELD,
	LAST = SHIELD
};

enum class EItemRarity {
	COMMON,
	RARE,
	EPIC,
	LEGENDARY,
	GODLIKE,
	UNIQUE
};

enum class EItemType {
	CONSUMABLE = 2,
	SCROLL = 6,
	ARMOR = 15,
	JEWLERY = 22,
	WEAPON = 28,
	RELIC = 45
};

struct grid_node {

	Character* _here;
	Character* _neighbors[8];

	grid_node()
		: _here(nullptr)
	{
		for (int i = 0; i < 8; ++i) _neighbors[i] = nullptr;
	}

	//void Swap(int x1, int y1, int x2, int y2) // SWAP for possible future TACTICAL MODE
};

struct path_node {

	//corresponds to dX and dY directions
	bool right;
	bool left;
	bool down;
	bool up;

	path_node() : right(0), left(0), up(0), down(0)
	{}

	bool& operator[](const int& idx) {
		switch (idx) {
		case 0: return right;
		case 1: return left;
		case 2: return down;
		case 3: return up;
		default: return right;
		}
	}

	bool operator[](const int& idx) const {
		switch (idx) {
		case 0: return right == true;
		case 1: return left == true;
		case 2: return down == true;
		case 3: return up == true;
		default: return false;
		}
	}
};

