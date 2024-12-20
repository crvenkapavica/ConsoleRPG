#include "ItemData.h"

std::vector<ItemData> ItemDb::Data;

ItemDb::ItemDataConstructor::ItemDataConstructor() {
	//																																					  				   <lvl    >lvl    <dmg    >dmg     arm     amount slots  arm_mod  wpn_mod  chnc
	Data.push_back({ EItemID::HPotion,		"Health potion",				EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		ESpellID::NONE,		0,		0,		0,		0,		0,		0.25f,	0,		0.f,		0.f,	200 });
	Data.push_back({ EItemID::EPotion,		"Essence potion",				EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		ESpellID::NONE,		0,		0,		0,		0,		0,		0.15f,	0,		0.f,		0.f,	400 });
	Data.push_back({ EItemID::FortElixir,		"Elixir of Fortitude",		EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		ESpellID::NONE,		0,		0,		0,		0,		0,		0.20f,	0,		0.f,		0.f,	600 });
	Data.push_back({ EItemID::StrElixir,		"Elixir of Strength",			EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		ESpellID::NONE,		0,		0,		0,		0,		0,		5.f,	0,		0.f,		0.f,	800 });
	Data.push_back({ EItemID::AgiElixir,		"Elixir of Agility",			EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		ESpellID::NONE,		0,		0,		0,		0,		0,		5.f,	0,		0.f,		0.f,	1000 });
	//scrolls
	Data.push_back({ EItemID::Scroll,			"Scroll",						EItemType::SCROLL,		EWeaponType::NONE,		EItemSlot::NONE,		ESpellID::NONE,		0,		0,		0,		0,		0,		0.f,	0,		0.f,		0.f,	1000 });
	//armor Head
	Data.push_back({ EItemID::ClothCap,		"Cloth Cap",					EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		3,		35,		0,		0,		40,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::LeatherHood,	"Leather Hood",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		36,		78,		0,		0,		95,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::RusticHelm,		"Rustic Helm",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		79,		153,	0,		0,		240,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::IronCasque,		"Iron Casque",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		154,	241,	0,		0,		460,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::HardColf,		"Hardened Colf",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		242,	349,	0,		0,		690,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::GoldDiadem,		"Golden Diadem",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		350,	475,	0,		0,		1080,	0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::WarCasque,		"Warlord's Casque",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		476,	581,	0,		0,		1510,	0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::PhoCrown,		"Phoenix Crown",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		582,	696,	0,		0,		2280,	0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::CelDiadem,		"Celestial Diadem",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		697,	837,	0,		0,		3350,	0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::CrownCosmos,	"Crown of the Cosmos",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		ESpellID::NONE,		838,	969,	0,		0,		4625,	0.f,	0,		0.f,		0.f,	0 });
	//Chest																																																												
	Data.push_back({ EItemID::ClothTunic,		"Cloth Tunic",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		3,	35,		0,		0,		50,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::PadVest,		"Padded Vest",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		36,	78,		0,		0,		118,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::StudLeather,	"Studded Leather",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		79,	153,	0,		0,		300,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::HardCuirass,	"Hardened Cuirass",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		154,	241,	0,		0,		575,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::ScaleMail,		"Scale Mail",					EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		242,	349,	0,		0,		860,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::KnightPlate,	"Knight's Plate",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		350,	475,	0,		0,		1350,	0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::DraghideArm,	"Dragonhide Armor",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		476,	581,	0,		0,		1880,	0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::PhoMail,		"Phoenix Mail",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		582,	696,	0,		0,		2850,	0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::CelAegis,		"Celestial Aegis",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		697,	837,	0,		0,		4180,	0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::ArmAncient,		"Armor of the Ancients",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		ESpellID::NONE,		838,	969,	0,		0,		5780,	0.f,	0,		0.f,		0.f,	0 });
	//Hands																																																											  
	Data.push_back({ EItemID::ClothWraps,		"Cloth Wraps",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		3,	35,		0,		0,		10,		0.f,	1,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::StudGauntlets,	"Studded Gauntlets",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		36,	78,		0,		0,		23,		0.f,	1,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::IronBracers,	"Iron Bracers",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		79,	153,	0,		0,		32,		0.f,	2,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::ChainGloves,	"Chain Gloves",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		154,	241,	0,		0,		115,		0.f,	2,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::SilvGauntlets,	"Silvered Gauntlets",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		242,	349,	0,		0,		172,		0.f,	3,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::KnightVambrace,	"Knight's Vambrace",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		350,	475,	0,		0,		270,		0.f,	3,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::TitanGrasp,		"Titan's Grasp",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		476,	581,	0,		0,		377,		0.f,	4,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::PhoTalons,		"Phoenix Talons",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		582,	696,	0,		0,		570,		0.f,	4,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::CelHandwraps,	"Celestial Handwrapds",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		697,	837,	0,		0,		837,		0.f,	5,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::InfHandGrds,	"Infernal Handguards",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		ESpellID::NONE,		838,	969,	0,		0,		1156,	0.f,	5,		0.f,		0.f,	0 });
	//Legs																																																											  
	Data.push_back({ EItemID::ThistTrous,		"Thistleweave Trousers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		3,	35,		0,		0,		15,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::IronGreaves,	"Ironscale Greaves",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		36,	78,		0,		0,		35,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::RunicTrTr,		"Runic Trail-Trousers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		79,	153,	0,		0,		90,		0.f,	0,		0.10f,	0.f,	0 });
	Data.push_back({ EItemID::MoonLegg,		"Moonbark Legguards",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		154,	241,	0,		0,		172,		0.f,	0,		0.17f,	0.f,	0 });
	Data.push_back({ EItemID::PhantStriders,	"Phantom Striders",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		242,	349,	0,		0,		258,		0.f,	0,		0.24f,	0.f,	0 });
	Data.push_back({ EItemID::TempWalkers,	"Tempest-Walkers",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		350,	475,	0,		0,		405,		0.f,	0,		0.33f,	0.f,	0 });
	Data.push_back({ EItemID::TitanMarch,		"Titan's Marchers",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		476,	581,	0,		0,		566,		0.f,	0,		0.46f,	0.f,	0 });
	Data.push_back({ EItemID::AbStrGrvs,		"Abyssal Stride Greaves",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		582,	696,	0,		0,		855,		0.f,	0,		0.59f,	0.f,	0 });
	Data.push_back({ EItemID::PyrWarKilt,		"Pyreborn War-Kilt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		697,	837,	0,		0,		1256,	0.f,	0,		0.72f,	0.f,	0 });
	Data.push_back({ EItemID::CosmosGreaves,	"Cosmosforged Greaves",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		ESpellID::NONE,		838,	969,	0,		0,		1734,	0.f,	0,		0.95f,	0.f,	0 });
	//Belt																																																											  
	Data.push_back({ EItemID::WovenSash,		"Woven Sash",					EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		3,		35,		0,		0,		10,		0.f,	2,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::LeatherBelt,	"Leather Belt",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		36,		78,		0,		0,		17,		0.f,	2,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::IronBuckBelt,	"Iron Buckle Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		79,		153,	0,		0,		45,		0.f,	3,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::ReinfGirdle,	"Reinforced Girdle",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		154,	241,	0,		0,		86,		0.f,	3,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::ChainLBelt,		"Chain Link Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		242,	349,	0,		0,		129,		0.f,	4,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::PlatedWaist,	"Plated Waistguard",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		350,	475,	0,		0,		202,		0.f,	4,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::DragBelt,		"Dragonbone Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		476,	581,	0,		0,		283,		0.f,	5,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::PhoWaistwrap,	"Phoenix Waistwrap",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		582,	696,	0,		0,		427,		0.f,	5,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::AncGirdle,		"Ancient's Girdle",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		697,	837,	0,		0,		628,		0.f,	6,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::InfWaistguard,	"Infernal Waistguard",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		ESpellID::NONE,		838,	969,	0,		0,		867,		0.f,	7,		0.f,		0.f,	0 });
	//Feet																																																											  
	Data.push_back({ EItemID::TrailBBoots,	"Trailblazer Boots",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		3,		35,		0,		0,		10,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::CobStompers,	"Cobblestone Stompers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		36,		78,		0,		0,		23,		0.f,	0,		0.f,		0.f,	0 });
	Data.push_back({ EItemID::SerpSlip,		"Serpentcoil Slippers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		79,		153,	0,		0,		60,		0.f,	0,		0.07f,	0.f,	0 });
	Data.push_back({ EItemID::EmberGreaves,	"Emberstep Greaves",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		154,	241,	0,		0,		115,		0.f,	0,		0.14f,	0.f,	0 });
	Data.push_back({ EItemID::WhispBoots,		"Whisperwind Boots",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		242,	349,	0,		0,		172,		0.f,	0,		0.22f,	0.f,	0 });
	Data.push_back({ EItemID::NightFoot,		"Nightstalker Footgear",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		350,	475,	0,		0,		270,		0.f,	0,		0.31f,	0.f,	0 });
	Data.push_back({ EItemID::TitanTrek,		"Titan's Trekboots",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		476,	581,	0,		0,		377,		0.f,	0,		0.45f,	0.f,	0 });
	Data.push_back({ EItemID::FlameWalk,		"Flameborn Walkers",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		582,	696,	0,		0,		570,		0.f,	0,		0.56f,	0.f,	0 });
	Data.push_back({ EItemID::CelFoot,		"Celestial Footfalls",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		697,	837,	0,		0,		837,		0.f,	0,		0.68f,	0.f,	0 });
	Data.push_back({ EItemID::StarFoot,		"Starshard Footwear",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		ESpellID::NONE,		838,	969,	0,		0,		1156,	0.f,	0,		0.83f,	0.f,	0 });
	//jewlery																																																										  
																																																													  
	//weapons Bow																																																									  
	Data.push_back({ EItemID::ShortBow,		"Short Bow",					EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		3,		35,		10,		20,		0,		0.f,	0,		0.f,		0.03f,	0 });
	Data.push_back({ EItemID::LongBow,		"Long Bow",					EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		36,		78,		26,		42,		0,		0.f,	0,		0.f,		0.07f,	0 });
	Data.push_back({ EItemID::RecBow,			"Recurve Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		79,		153,	51,		65,		0,		0.f,	0,		0.f,		0.12f,	0 });
	Data.push_back({ EItemID::CompBow,		"Composite Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		154,	241,	78,		102,	0,		0.f,	0,		0.f,		0.18f,	0 });
	Data.push_back({ EItemID::RangBow,		"Ranger's Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		242,	349,	123,	156,	0,		0.f,	0,		0.f,		0.24f,	0 });
	Data.push_back({ EItemID::ElvenLBow,		"Elven Long Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		350,	475,	182,	246,	0,		0.f,	0,		0.f,		0.35f,	0 });
	Data.push_back({ EItemID::DragBow,		"Dragonwing Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		476,	581,	315,	390,	0,		0.f,	0,		0.f,		0.48f,	0 });
	Data.push_back({ EItemID::PhoBow,			"Phoenix Fire Bow",			EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		582,	696,	470,	595,	0,		0.f,	0,		0.f,		0.63f,	0 });
	Data.push_back({ EItemID::InfFurBow,		"Inferno Fury Bow",			EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		697,	837,	735,	890,	0,		0.f,	0,		0.f,		0.79f,	0 });
	Data.push_back({ EItemID::SoulBow,		"Soulreaper Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	ESpellID::NONE,		838,	969,	1100,	1405,	0,		0.f,	0,		0.f,		0.95f,	0 });
	//Staff																																																											  
	Data.push_back({ EItemID::WoodStf,		"Wooden Staff",				EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		3,		35,		6,		12,		0,		0.f,	0,		0.f,		0.07f,	0 });
	Data.push_back({ EItemID::AppStf,			"Apprentice's Staff",			EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		36,		78,		18,		27,		0,		0.f,	0,		0.f,		0.15f,	0 });
	Data.push_back({ EItemID::SageStick,		"Sage's Walking Stick",		EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		79,		153,	35,		47,		0,		0.f,	0,		0.f,		0.27f,	0 });
	Data.push_back({ EItemID::SorcCane,		"Sorcerer's Cane",			EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		154,	241,	46,		61,		0,		0.f,	0,		0.f,		0.45f,	0 });
	Data.push_back({ EItemID::SunStf,			"Sunwood Staff",				EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		242,	349,	73,		93,		0,		0.f,	0,		0.f,		0.60f,	0 });
	Data.push_back({ EItemID::GlacRod,		"Glacial Rod",				EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		350,	475,	109,	147,	0,		0.f,	0,		0.f,		0.82f,	0 });
	Data.push_back({ EItemID::EarthStf,		"Earthshaker Staff",			EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		476,	581,	189,	234,	0,		0.f,	0,		0.f,		1.05f,	0 });
	Data.push_back({ EItemID::CelScep,		"Celestial Scepter",			EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		582,	696,	282,	357,	0,		0.f,	0,		0.f,		1.43f,	0 });
	Data.push_back({ EItemID::EclStf,			"Eclipse Staff",				EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		697,	837,	441,	534,	0,		0.f,	0,		0.f,		2.01f,	0 });
	Data.push_back({ EItemID::VoidStf,		"Voidwalker's Staff",			EItemType::WEAPON,		EWeaponType::STAFF,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		838,	969,	660,	843,	0,		0.f,	0,		0.f,		2.69f,	0 });
	//Axe 2H																																																										  
	Data.push_back({ EItemID::TimbAxe,		"Timber Axe",					EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		3,		35,		16,		18,		0,		0.f,	0,		0.f,		0.04f,	0 });
	Data.push_back({ EItemID::IronSplt,		"Iron Splitter",				EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		36,		78,		35,		37,		0,		0.f,	0,		0.f,		0.09f,	0 });
	Data.push_back({ EItemID::BerHatch,		"Berserker Hatchet",			EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		79,		153,	55,		58,		0,		0.f,	0,		0.f,		0.14f,	0 });
	Data.push_back({ EItemID::StReav,			"Steel Reaver",				EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		154,	241,	87,		91,		0,		0.f,	0,		0.f,		0.23f,	0 });
	Data.push_back({ EItemID::RavCleav,		"Ravager's Cleaver",			EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		242,	349,	131,	140,	0,		0.f,	0,		0.f,		0.34f,	0 });
	Data.push_back({ EItemID::WarChop,		"Warlord's Chopper",			EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		350,	475,	207,	221,	0,		0.f,	0,		0.f,		0.45f,	0 });
	Data.push_back({ EItemID::NightEx,		"Nightfall Executioner",		EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		476,	581,	333,	351,	0,		0.f,	0,		0.f,		0.57f,	0 });
	Data.push_back({ EItemID::InfEdge,		"Inferno Edge",				EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		582,	696,	495,	535,	0,		0.f,	0,		0.f,		0.69f,	0 });
	Data.push_back({ EItemID::AbDestr,		"Abyssal Destroyer",			EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		697,	837,	775,	801,	0,		0.f,	0,		0.f,		0.81f,	0 });
	Data.push_back({ EItemID::SoulHarv,		"Soul Harvester",				EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		838,	969,	1184,	1264,	0,		0.f,	0,		0.f,		0.93f,	0 });
	//Mace 2H																																																										  
	Data.push_back({ EItemID::WoodClub,		"Wooden Club",				EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		3,		35,		17,		17,		0,		0.f,	0,		0.f,		0.09f,	0 });
	Data.push_back({ EItemID::IronMaul,		"Iron Maul",					EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		36,		78,		36,		36,		0,		0.f,	0,		0.f,		0.18f,	0 });
	Data.push_back({ EItemID::StoneSled,		"Stone Sledge",				EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		79,		153,	57,		57,		0,		0.f,	0,		0.f,		0.31f,	0 });
	Data.push_back({ EItemID::SpikBlud,		"Spiked Bludgeon",			EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		154,	241,	89,		89,		0,		0.f,	0,		0.f,		0.53f,	0 });
	Data.push_back({ EItemID::BoneMace,		"Boneshatter Mace",			EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		242,	349,	136,	136,	0,		0.f,	0,		0.f,		0.75f,	0 });
	Data.push_back({ EItemID::SkyHammer,		"Skyfall Hammer",				EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		350,	475,	213,	213,	0,		0.f,	0,		0.f,		1.03f,	0 });
	Data.push_back({ EItemID::TideBreak,		"Tidebreaker",				EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		476,	581,	344,	344,	0,		0.f,	0,		0.f,		1.56f,	0 });
	Data.push_back({ EItemID::SoulMaul,		"Soulreaver's Maul",			EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		582,	696,	520,	520,	0,		0.f,	0,		0.f,		2.12f,	0 });
	Data.push_back({ EItemID::PhantFlail,		"Phantom Monarch's Flail",	EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		697,	837,	792,	792,	0,		0.f,	0,		0.f,		2.95f,	0 });
	Data.push_back({ EItemID::DeathDcree,		"Deathbringer's Decree",		EItemType::WEAPON,		EWeaponType::MACE_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		838,	969,	1226,	1226,	0,		0.f,	0,		0.f,		4.10f,	0 });
	//Sword 2H																																																										  
	Data.push_back({ EItemID::GraveBld,		"Graveblade",					EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		3,		35,		11,		23,		0,		0.f,	0,		0.f,		0.04f,	0 });
	Data.push_back({ EItemID::IronReq,		"Iron Requiem",				EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		36,		78,		27,		45,		0,		0.f,	0,		0.f,		0.08f,	0 });
	Data.push_back({ EItemID::GhoulSlyr,		"Ghoulslayer",				EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		79,		153,	42,		72,		0,		0.f,	0,		0.f,		0.13f,	0 });
	Data.push_back({ EItemID::SoulClay,		"Soulthirst Claymore",		EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		154,	241,	63,		115,	0,		0.f,	0,		0.f,		0.23f,	0 });
	Data.push_back({ EItemID::PlagZwei,		"Plaguebearer's Zweihander",	EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		242,	349,	94,		178,	0,		0.f,	0,		0.f,		0.30f,	0 });
	Data.push_back({ EItemID::SpecEdge,		"Specter's Edge",				EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		350,	475,	142,	284,	0,		0.f,	0,		0.f,		0.38f,	0 });
	Data.push_back({ EItemID::EbonReap,		"Ebon Reaper",				EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		476,	581,	246,	442,	0,		0.f,	0,		0.f,		0.47f,	0 });
	Data.push_back({ EItemID::DreadDecap,		"Dreadlord's Decapitator",	EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		582,	696,	377,	663,	0,		0.f,	0,		0.f,		0.57f,	0 });
	Data.push_back({ EItemID::EclDeathBld,	"Eclipse Deathblade",			EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		697,	837,	581,	1003,	0,		0.f,	0,		0.f,		0.68f,	0 });
	Data.push_back({ EItemID::ShadMonGuil,	"Shadow Monarch's Guillotine",EItemType::WEAPON,		EWeaponType::SWORD_2H,	EItemSlot::WPN_MAIN,	ESpellID::NONE,		838,	969,	860,	1592,	0,		0.f,	0,		0.f,		0.80f,	0 });
	//Axe 1H																																																										  
	Data.push_back({ EItemID::HatchAxe,		"Hatchling's Axe",			EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		3,		35,		5,		6,		0,		0.f,	0,		0.f,		0.04f,	0 });
	Data.push_back({ EItemID::TimbCut,		"Timber Cutter",				EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		36,		78,		11,		12,		0,		0.f,	0,		0.f,		0.09f,	0 });
	Data.push_back({ EItemID::IronTom,		"Iron Tomahawk",				EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		79,		153,	18,		19,		0,		0.f,	0,		0.f,		0.14f,	0 });
	Data.push_back({ EItemID::BronzeHat,		"Bronze Hatchet",				EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		154,	241,	30,		31,		0,		0.f,	0,		0.f,		0.23f,	0 });
	Data.push_back({ EItemID::MercAxe,		"Mercenary's Axe",			EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		242,	349,	44,		45,		0,		0.f,	0,		0.f,		0.34f,	0 });
	Data.push_back({ EItemID::SteelChop,		"Steel Chopper",				EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		350,	475,	70,		71,		0,		0.f,	0,		0.f,		0.45f,	0 });
	Data.push_back({ EItemID::SilvHat,		"Silver Hatcher",				EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		476,	581,	113,	119,	0,		0.f,	0,		0.f,		0.57f,	0 });
	Data.push_back({ EItemID::BloodAxe,		"Bloodmoon Axe",				EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		582,	696,	168,	181,	0,		0.f,	0,		0.f,		0.69f,	0 });
	Data.push_back({ EItemID::RavenAxe,		"Ravenwing Axe",				EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		697,	837,	263,	272,	0,		0.f,	0,		0.f,		0.81f,	0 });
	Data.push_back({ EItemID::PhantBane,		"Phantom's Bane",				EItemType::WEAPON,		EWeaponType::AXE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		838,	969,	402,	429,	0,		0.f,	0,		0.f,		0.93f,	0 });
	//Mace 1H																																																											
	Data.push_back({ EItemID::NovMac,			"Novice Mace",				EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		3,		35,		6,		6,		0,		0.f,	0,		0.f,		0.01f,	0 });
	Data.push_back({ EItemID::IronClub,		"Iron Club",					EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		36,		78,		12,		12,		0,		0.f,	0,		0.f,		0.02f,	0 });
	Data.push_back({ EItemID::StoneMallet,	"Stone Mallet",				EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		79,		153,	19,		19,		0,		0.f,	0,		0.f,		0.03f,	0 });
	Data.push_back({ EItemID::BrassKnob,		"Brass Knobkierie",			EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		154,	241,	30,		30,		0,		0.f,	0,		0.f,		0.05f,	0 });
	Data.push_back({ EItemID::ThornMace,		"Thorn Mace",					EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		242,	349,	46,		46,		0,		0.f,	0,		0.f,		0.08f,	0 });
	Data.push_back({ EItemID::SilverSc,		"Silver Scepter",				EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		350,	475,	72,		72,		0,		0.f,	0,		0.f,		0.10f,	0 });
	Data.push_back({ EItemID::CrystSc,		"Crystalline Scepter",		EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		476,	581,	117,	117,	0,		0.f,	0,		0.f,		0.12f,	0 });
	Data.push_back({ EItemID::NightMaul,		"Nightshade Maul",			EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		582,	696,	176,	176,	0,		0.f,	0,		0.f,		0.14f,	0 });
	Data.push_back({ EItemID::VoidCudg,		"Voidheart Cudgel",			EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		697,	837,	269,	269,	0,		0.f,	0,		0.f,		0.16f,	0 });
	Data.push_back({ EItemID::EthCrush,		"Ethereal Crusher",			EItemType::WEAPON,		EWeaponType::MACE_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		838,	969,	416,	416,	0,		0.f,	0,		0.f,		0.18f,	0 });
	//Sword 1H																																																										  
	Data.push_back({ EItemID::TrainBlade,		"Trainee's Blade",			EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		3,		35,		4,		8,		0,		0.f,	0,		0.f,		0.03f,	0 });
	Data.push_back({ EItemID::IronShortS,		"Iron Shortsword",			EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		36,		78,		9,		15,		0,		0.f,	0,		0.f,		0.06f,	0 });
	Data.push_back({ EItemID::MercSabre,		"Mercenary's Sabre",			EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		79,		153,	14,		24,		0,		0.f,	0,		0.f,		0.10f,	0 });
	Data.push_back({ EItemID::BronzeCut,		"Bronze Cutlass",				EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		154,	241,	21,		39,		0,		0.f,	0,		0.f,		0.17f,	0 });
	Data.push_back({ EItemID::TGuardRap,		"Town Guard's Rapier",		EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		242,	349,	32,		60,		0,		0.f,	0,		0.f,		0.24f,	0 });
	Data.push_back({ EItemID::BersScim,		"Berserker's Scimitar",		EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		350,	475,	48,		97,		0,		0.f,	0,		0.f,		0.30f,	0 });
	Data.push_back({ EItemID::DragSabre,		"Dragonfang Sabre",			EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		476,	581,	84,		150,	0,		0.f,	0,		0.f,		0.38f,	0 });
	Data.push_back({ EItemID::SunShort,		"Sunforged Shortsword",		EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		582,	696,	128,	225,	0,		0.f,	0,		0.f,		0.47f,	0 });
	Data.push_back({ EItemID::ShadBlade,		"Shadowsteel Blade",			EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		697,	837,	197,	341,	0,		0.f,	0,		0.f,		0.58f,	0 });
	Data.push_back({ EItemID::PhantRap,		"Phantom Rapier",				EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		838,	969,	292,	541,	0,		0.f,	0,		0.f,		0.69f,	0 });
	//Dagger																																																										  
	Data.push_back({ EItemID::RogShiv,		"Rogue's Shiv",				EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		3,		35,		7,		11,		0,		0.f,	0,		0.f,		0.12f,	0 });
	Data.push_back({ EItemID::IronStil,		"Iron Stilleto",				EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		36,		78,		16,		23,		0,		0.f,	0,		0.f,		0.20f,	0 });
	Data.push_back({ EItemID::HuntKnife,		"Hunter's Knife",				EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		79,		153,	29,		41,		0,		0.f,	0,		0.f,		0.25f,	0 });
	Data.push_back({ EItemID::ThiefBlad,		"Thief's Blade",				EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		154,	241,	47,		62,		0,		0.f,	0,		0.f,		0.30f,	0 });
	Data.push_back({ EItemID::SerpFang,		"Serpent Fang",				EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		242,	349,	75,		91,		0,		0.f,	0,		0.f,		0.37f,	0 });
	Data.push_back({ EItemID::AssNeedle,		"Assassin's Needle",			EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		350,	475,	101,	133,	0,		0.f,	0,		0.f,		0.45f,	0 });
	Data.push_back({ EItemID::VipersKiss,		"Viper's Kiss",				EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		476,	581,	145,	189,	0,		0.f,	0,		0.f,		0.54f,	0 });
	Data.push_back({ EItemID::WindShank,		"Windrunner's Shank",			EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		582,	696,	209,	311,	0,		0.f,	0,		0.f,		0.66f,	0 });
	Data.push_back({ EItemID::ShadPierc,		"Shadow Piercer",				EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		697,	837,	352,	505,	0,		0.f,	0,		0.f,		0.78f,	0 });
	Data.push_back({ EItemID::EthRipp,		"Ethereal Ripper",			EItemType::WEAPON,		EWeaponType::DAGGER,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		838,	969,	551,	725,	0,		0.f,	0,		0.f,		0.90f,	0 });
	//Orb																																																											  
	Data.push_back({ EItemID::NovSph,			"Novice's Sphere",			EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		3,		35,		2,		3,		0,		0.f,	0,		0.f,		0.02f,	0 });
	Data.push_back({ EItemID::MystMarb,		"Mystic Marble",				EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		36,		78,		5,		8,		0,		0.f,	0,		0.f,		0.06f,	0 });
	Data.push_back({ EItemID::DruidStone,		"Druid's Stone",				EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		79,		153,	11,		17,		0,		0.f,	0,		0.f,		0.1f,	0 });
	Data.push_back({ EItemID::AethOrb,		"Aether Orb",					EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		154,	241,	22,		33,		0,		0.f,	0,		0.f,		0.14f,	0 });
	Data.push_back({ EItemID::WizEye,			"Wizard's Eye",				EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		242,	349,	38,		49,		0,		0.f,	0,		0.f,		0.19f,	0 });
	Data.push_back({ EItemID::DivGlobe,		"Divination Globe",			EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		350,	475,	55,		68,		0,		0.f,	0,		0.f,		0.25f,	0 });
	Data.push_back({ EItemID::NethStone,		"Netherstone",				EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		476,	581,	79,		92,		0,		0.f,	0,		0.f,		0.32f,	0 });
	Data.push_back({ EItemID::PhoTear,		"Phoenix Tear",				EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		582,	696,	100,	137,	0,		0.f,	0,		0.f,		0.40f,	0 });
	Data.push_back({ EItemID::RealShard,		"Reality Shard",				EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		697,	837,	155,	199,	0,		0.f,	0,		0.f,		0.49f,	0 });
	Data.push_back({ EItemID::CosSphere,		"Cosmosphere",				EItemType::WEAPON,		EWeaponType::ORB,		EItemSlot::WPN_OFF,		ESpellID::NONE,		838,	969,	231,	300,	0,		0.f,	0,		0.f,		0.60f,	0 });
	//Shield

	//relic

	//Basic Item
	//_data.push_back({ EItemID::TrainBlade,		"Trainee's Blade",				EItemType::WEAPON,		EWeaponType::SWORD_1H,	EItemSlot::WPN_BOTH,	ESpellID::NONE,		3,		35,		4,		8,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.03f,	0.f,	0.f,	0.f,	0.f,	0,		false,     false });
};
ItemDb::ItemDataConstructor ItemDb::ItemData;