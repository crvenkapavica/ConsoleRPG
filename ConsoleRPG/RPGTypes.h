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

typedef int attribute;
typedef int skill;

class Character;

#undef _stat
#undef max
#undef min
#undef PURE

#define START_X 8
#define START_Y 8
#define MAX_X 100
#define MAX_Y 100

#define GRID_X 21  // decrease by 4 to change grid height
#define GRID_Y 81 // decrease by 8 to change grid width
#define CHAR_GRID_X ((GRID_X - 1) / 4)
#define CHAR_GRID_Y ((GRID_Y - 1) / 8)

#define MAX_LVL 75

#define NUM_PLAYER_CLASSES 1
#define NUM_ENEMY_CLASSES 1

#define ITEM_TYPES 6
#define ITEM_RARITIES 6
#define ITEM_SLOTS 12
#define INV_SLOTS 30

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
#define COLOR_ERROR				ANSI_COLOR_RED
#define COLOR_LOOT				ANSI_COLOR_GREEN

#define COLOR_COMMON			ANSI_COLOR_FG_RGB(105,106,106)
#define COLOR_RARE				ANSI_COLOR_FG_RGB(0,112,221)
#define COLOR_EPIC				ANSI_COLOR_FG_RGB(163,53,238)
#define COLOR_LEGENDARY			ANSI_COLOR_FG_RGB(255,128,0)
#define COLOR_GODLIKE			ANSI_COLOR_FG_RGB(200,34,56)
#define COLOR_UNIQUE			ANSI_COLOR_YELLOW 


#define COLOR_BG				ANSI_COLOR_BG_RGB(142,191,208) // light-blueish version 2 [ BETTER ]
//#define COLOR_BG				ANSI_COLOR_BG_RGB(186, 206, 255) // light-blueish version 3 [ ALSO GOOD, a bit brighter than 2]
//#define COLOR_BG				ANSI_COLOR_BG_RGB(221,221,218) // very light-blueish
//#define COLOR_BG				ANSI_COLOR_BG_RGB(230,204,147) // yellowish


#define COLOR_FG				ANSI_COLOR_FG_RGB(53,135,30)
//#define COLOR_FG				ANSI_COLOR_FG_RGB(53,170,30)	// darker-green
//#define COLOR_FG				ANSI_COLOR_FG_RGB(33,119,0)		// lighter-green
//#define COLOR_FG				ANSI_COLOR_FG_RGB(130,173,136)  // grayish-green


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

#define RPG_ASSERT(expr, msg) \
    if (!(expr)) { \
		system("cls"); \
        std::cout << "Assertion failed: " << (msg) << "\n"; \
        std::cout << "In file: " << __FILE__ << ", line: " << __LINE__ << "\n"; \
        std::exit(EXIT_FAILURE); \
    }

enum class EStatType {
	ANY,
	HEALTH,
	RESISTANCE
};

enum class EStatMod {
	CONSTANT,
	ADDITIVE
};

enum class ESpellID {
	// MAGIC
	NONE,
	FIREBALL,
	BURNING,
	MOLTEN_ARMOR,
	EXPOSURE,
	STONESKIN,
	DISARM,
	BLOODBATH,
	ARCANE_INFUSION,
	BLOOD_RAIN,
	VISCOUS_ACID,

	// MISC
	BLIND,

	//SUMMON
	SUM_FIRE_ELE,
	SUM_FIRE_IMP,

	// MELEE
	MELEE,
	// RANGED
	RANGED,

	// PASSIVES
	VAMPIRIC_TOUCH,
	THORNS,
};

enum class ESpellType {
	NONE,
	BUFF,
	DEBUFF,
	AURA,
	SUMMON,
	DAMAGE,
	HEALING
};

enum class ESpellClass {
	MELEE,
	RANGED,
	MAGIC,
	SUMMON,
	MISC,
	ITEM
};

enum class ESpellActivity {
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

	// PLAYER
	BARBARIAN,
	WARLOCK,
	MAGE,

	// SUMMON
	WATER_ELEMENTAL,
	FIRE_ELEMENTAL,
	ARCANE_ELEMENTAL,
	CHAOS_GOLEM,
	MAGMA_GOLEM,
	NATURES_PROPHET,
	DEATH_PROPHET,
	FIRE_IMP,

	// ENEMY
	E_GHOUL = 50,
	E_ARCHER,
	E_MAGE,
	E_WARLOCK,
	E_SWORDSMAN,
	E_FIRE_IMP,

	// BOSSES
	B_LICH
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

enum class ECombatEvent {
	ON_APPLY,
	ON_SKIP_APPLY,
	ON_COMBAT_BEGIN,
	ON_COMBAT_END,
	ON_TURN_BEGIN,
	ON_TURN_END,
	ON_CYCLE_BEGIN,
	ON_CYCLE_END,
	ON_MAGIC_BEGIN,
	ON_MAGIC_END,
	ON_MAGIC_RECEIVED_BEGIN,
	ON_MAGIC_RECEIVED_END,
	ON_MELEE_BEGIN,
	ON_MELEE_END,
	ON_MELEE_RECEIVED_BEGIN,
	ON_MELEE_RECEIVED_END,
	ON_RANGED_BEGIN,
	ON_RANGED_END,
	ON_RANGED_RECEIVED_BEGIN,
	ON_RANGED_RECEIVED_END,
};

enum EStructFlags {
	EFFECT_STAT = 1,
};

enum class EItemId {
	NONE,
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
	RunicTrTr,
	MoonLegg,
	PhantStriders,
	TempWalkers,
	TitanMarch,
	AbStrGrvs,
	PyrWarKilt,
	CosmosGreaves,
	//Feet
	TrailBBoots,
	CobStompers,
	SerpSlip,
	EmberGreaves,
	WhispBoots,
	NightFoot,
	TitanTrek,
	FlameWalk,
	CelFoot,
	StarFoot,
	//jewl
	//ring
	
	//neck

	//weapon
	//bows
	ShortBow,
	LongBow,
	RecBow,
	CompBow,
	RangBow,
	ElvenLBow,
	DragBow,
	PhoBow,
	InfFurBow,
	SoulBow,
	//staff
	WoodStf,
	AppStf,
	SageStick,
	SorcCane,
	SunStf,
	GlacRod,
	EarthStf,
	CelScep,
	EclStf,
	VoidStf,
	//Axe 2H
	TimbAxe,
	IronSplt,
	BerHatch,
	StReav,
	RavCleav,
	WarChop,
	NightEx,
	InfEdge,
	AbDestr,
	SoulHarv,
	//Mace 2h
	WoodClub,
	IronMaul,
	StoneSled,
	SpikBlud,
	BoneMace,
	SkyHammer,
	TideBreak,
	SoulMaul,
	PhantFlail,
	DeathDcree,
	//Sword 2h
	GraveBld,
	IronReq,
	GhoulSlyr,
	SoulClay,
	PlagZwei,
	SpecEdge,
	EbonReap,
	DreadDecap,
	EclDeathBld,
	ShadMonGuil,
	//Axe 1H
	HatchAxe,
	TimbCut,
	IronTom,
	BronzeHat,
	MercAxe,
	SteelChop,
	SilvHat,
	BloodAxe,
	RavenAxe,
	PhantBane,
	//Mace 1H
	NovMac,
	IronClub,
	StoneMallet,
	BrassKnob,
	ThornMace,
	SilverSc,
	CrystSc,
	NightMaul,
	VoidCudg,
	EthCrush,
	//Sword 1H
	TrainBlade,
	IronShortS,
	MercSabre,
	BronzeCut,
	TGuardRap,
	BersScim,
	DragSabre,
	SunShort,
	ShadBlade,
	PhantRap,
	//Dagger
	RogShiv,
	IronStil,
	HuntKnife,
	ThiefBlad,
	SerpFang,
	AssNeedle,
	VipersKiss,
	WindShank,
	ShadPierc,
	EthRipp,
	//Orb
	NovSph,
	MystMarb,
	DruidStone,
	AethOrb,
	WizEye,
	DivGlobe,
	NethStone,
	PhoTear,
	RealShard,
	CosSphere,
	//Shield




	//relic
};

enum class EItemSlot : int8_t {
	NONE = -1,
	HEAD = 0,
	CHEST,
	HANDS,
	BELT,
	LEGS,
	FEET,
	NECK,
	FINGER1,
	FINGER2,
	WPN_MAIN,
	WPN_OFF,
	RELIC,

	WPN_BOTH = 100,
	FINGER
};

enum class EWeaponType : int8_t {
	NONE = -1,
	BOW = 0,		// % extra random magic damage  f
	FIRST_2H = BOW,
	STAFF = 1,			// spell_crit damage %   f
	AXE_2H = 2,			// armor penetration amount  f
	MACE_2H = 3,		// crit damage   f
	SWORD_2H = 4,		// double_strike chance  f
	LAST_2H = SWORD_2H,
	AXE_1H = 5,			// armor penetration amount  f
	FIRST_1H = AXE_1H,
	MACE_1H = 6,		// crit chance  f
	SWORD_1H = 7,		// double_strike chance  f
	DAGGER = 8,			// bleed chance (a bleed damage je isti?)  f
	ORB = 9,			// spell_crit_chance %
	SHIELD = 10,			// % amount to block  f
	LAST_1H = SHIELD,
	LAST = SHIELD
};

enum class EItemRarity : int8_t {
	MISC,
	COMMON,
	RARE,
	EPIC,
	LEGENDARY,
	GODLIKE,
	UNIQUE
};

enum class EItemType : int8_t {
	CONSUMABLE,
	SCROLL,
	ARMOR,
	JEWELLERY,
	WEAPON,
	RELIC,
	MISC
};

enum class EItemAffix : int8_t {
	NONE,

	Active,
	Passive,
	
	STR,
	AGI,
	INT,
	VIT,
	CON,
	END,
	ALL_ATTR,

	//To_Arcane,
	//To_Fire,
	//To_Ligthning,
	//To_Cold,
	//To_Poison,
	//To_Necrotic,
	//To_Physical,
	//To_Pure,
	//To_Elemental, // Fire, Light, Cold
	//To_Black, // Necrotic, Poison
	////To_Divine, // Arcane, Holy

	Arcane_dmg,
	Fire_dmg,
	Ligthning_dmg,
	Cold_dmg,
	Poison_dmg,
	Necrotic_dmg,
	Physical_dmg,
	Pure_dmg,
	Elemental_dmg, // Fire, Light, Cold
	Black_dmg, // Necrotic, Poison
	//Divine_dmg, // Arcane, Holy

	Arcane_res,
	Fire_res,
	Ligthning_res,
	Cold_res,
	Poison_res,
	Necrotic_res,
	Physical_res,
	Elemental_res, // Fire, Light, Cold
	Black_res, // Necrotic, Poison
	//Divine_res, // Arcane, Holy

};

// MAIN CATEGORY

// ACTIVE
// PASIVE
// ATTRIBUTE
// STAT
// DMG
// RES
// TO_DMG
// MISC (buff/debuff duration)