#include "ItemData.h"

std::vector<ItemData> ItemDatabase::_item_data;

ItemDatabase::ItemDataConstructor::ItemDataConstructor() {
	//																																					  					  <lvl    >lvl    <dmg    >dmg     arm      value  s_slt   m_dmg  c_slt    m_res   ex_dmg  scdmg   armpen  cdmg     2x%    crit%    bld%   block   %chnc   non_combat                                                        
	//consumables
	_item_data.push_back({ EItemID::HPotion,		"Health potion",			EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,		0,		0,		0,		0,		0,		0.25f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	200,	false });
	_item_data.push_back({ EItemID::EPotion,		"Essence potion",			EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,		0,		0,		0,		0,		0,		0.15f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	200,	false });
	_item_data.push_back({ EItemID::FortElixir,		"Elixir of Fortitude",		EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,		0,		0,		0,		0,		0,		0.20f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	200,	false });
	_item_data.push_back({ EItemID::StrElixir,		"Elixir of Strength",		EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,		0,		0,		0,		0,		0,		5.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	200,	false });
	_item_data.push_back({ EItemID::AgiElixir,		"Elixir of Agility",		EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,		0,		0,		0,		0,		0,		5.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	200,	false });
	//scrolls
	_item_data.push_back({ EItemID::Scroll,			"Scroll",					EItemType::SCROLL,		EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,		0,		0,		0,		0,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	1000,	false });
	//armor Head
	_item_data.push_back({ EItemID::ClothCap,		"Cloth Cap",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		3,		35,		0,		0,		40,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::LeatherHood,	"Leather Hood",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		36,		78,		0,		0,		95,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::RusticHelm,		"Rustic Helm",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		79,		153,	0,		0,		240,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::IronCasque,		"Iron Casque",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		154,	241,	0,		0,		460,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::HardColf,		"Hardened Colf",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		242,	349,	0,		0,		690,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::GoldDiadem,		"Golden Diadem",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		350,	475,	0,		0,		1080,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::WarCasque,		"Warlord's Casque",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		476,	581,	0,		0,		1510,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoCrown,		"Phoenix Crown",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		582,	696,	0,		0,		2280,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelDiadem,		"Celestial Diadem",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		697,	837,	0,		0,		3350,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CrownCosmos,	"Crown of the Cosmos",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,		838,	969,	0,		0,		4625,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//Chest
	_item_data.push_back({ EItemID::ClothTunic,		"Cloth Tunic",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		3,		35,		0,		0,		50,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PadVest,		"Padded Vest",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		36,		78,		0,		0,		118,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::StudLeather,	"Studded Leather",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		79,		153,	0,		0,		300,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::HardCuirass,	"Hardened Cuirass",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		154,	241,	0,		0,		575,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::ScaleMail,		"Scale Mail",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		242,	349,	0,		0,		860,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::KnightPlate,	"Knight's Plate",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		350,	475,	0,		0,		1350,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::DraghideArm,	"Dragonhide Armor",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		476,	581,	0,		0,		1880,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoMail,		"Phoenix Mail",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		582,	696,	0,		0,		2850,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelAegis,		"Celestial Aegis",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		697,	837,	0,		0,		4180,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::ArmAncient,		"Armor of the Ancients",	EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,		838,	969,	0,		0,		5780,	0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//Hands
	_item_data.push_back({ EItemID::ClothWraps,		"Cloth Wraps",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		3,		35,		0,		0,		10,		0.f,	1,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::StudGauntlets,	"Studded Gauntlets",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		36,		78,		0,		0,		23,		0.f,	1,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::IronBracers,	"Iron Bracers",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		79,		153,	0,		0,		32,		0.f,	2,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::ChainGloves,	"Chain Gloves",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		154,	241,	0,		0,		115,	0.f,	2,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::SilvGauntlets,	"Silvered Gauntlets",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		242,	349,	0,		0,		172,	0.f,	3,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::KnightVambrace,	"Knight's Vambrace",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		350,	475,	0,		0,		270,	0.f,	3,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::TitanGrasp,		"Titan's Grasp",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		476,	581,	0,		0,		377,	0.f,	4,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoTalons,		"Phoenix Talons",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		582,	696,	0,		0,		570,	0.f,	4,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelHandwraps,	"Celestial Handwrapds",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		697,	837,	0,		0,		837,	0.f,	5,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::InfHandGrds,	"Infernal Handguards",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,		838,	969,	0,		0,		1156,	0.f,	5,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//Legs
	_item_data.push_back({ EItemID::ThistTrous,		"Thistleweave Trousers",	EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		3,		35,		0,		0,		15,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::IronGreaves,	"Ironscale Greaves",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		36,		78,		0,		0,		35,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::RunicTrTr,		"Runic Trail-Trousers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		79,		153,	0,		0,		90,		0.f,	0,		0.10f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::MoonLegg,		"Moonbark Legguards",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		154,	241,	0,		0,		172,	0.f,	0,		0.17f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhantStriders,	"Phantom Striders",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		242,	349,	0,		0,		258,	0.f,	0,		0.24f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::TempWalkers,	"Tempest-Walkers",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		350,	475,	0,		0,		405,	0.f,	0,		0.33f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::TitanMarch,		"Titan's Marchers",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		476,	581,	0,		0,		566,	0.f,	0,		0.46f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::AbStrGrvs,		"Abyssal Stride Greaves",	EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		582,	696,	0,		0,		855,	0.f,	0,		0.59f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PyrWarKilt,		"Pyreborn War-Kilt",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		697,	837,	0,		0,		1256,	0.f,	0,		0.72f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CosmosGreaves,	"Cosmosforged Greaves",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,		838,	969,	0,		0,		1734,	0.f,	0,		0.95f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//Belt
	_item_data.push_back({ EItemID::WovenSash,		"Woven Sash",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		3,		35,		0,		0,		10,		0.f,	0,		0.f,	2,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::LeatherBelt,	"Leather Belt",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		36,		78,		0,		0,		17,		0.f,	0,		0.f,	2,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::IronBuckBelt,	"Iron Buckle Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		79,		153,	0,		0,		45,		0.f,	0,		0.f,	3,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::ReinfGirdle,	"Reinforced Girdle",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		154,	241,	0,		0,		86,		0.f,	0,		0.f,	3,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::ChainLBelt,		"Chain Link Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		242,	349,	0,		0,		129,	0.f,	0,		0.f,	4,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PlatedWaist,	"Plated Waistguard",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		350,	475,	0,		0,		202,	0.f,	0,		0.f,	4,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::DragBelt,		"Dragonbone Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		476,	581,	0,		0,		283,	0.f,	0,		0.f,	5,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoWaistwrap,	"Phoenix Waistwrap",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		582,	696,	0,		0,		427,	0.f,	0,		0.f,	5,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::AncGirdle,		"Ancient's Girdle",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		697,	837,	0,		0,		628,	0.f,	0,		0.f,	6,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::InfWaistguard,	"Infernal Waistguard",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,		838,	969,	0,		0,		867,	0.f,	0,		0.f,	7,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//Feet
	_item_data.push_back({ EItemID::TrailBBoots,	"Trailblazer Boots",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		3,		35,		0,		0,		10,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CobStompers,	"Cobblestone Stompers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		36,		78,		0,		0,		23,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::SerpSlip,		"Serpentcoil Slippers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		79,		153,	0,		0,		60,		0.f,	0,		0.f,	0,		0.07f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::EmberGreaves,	"Emberstep Greaves",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		154,	241,	0,		0,		115,	0.f,	0,		0.f,	0,		0.14f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::WhispBoots,		"Whisperwind Boots",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		242,	349,	0,		0,		172,	0.f,	0,		0.f,	0,		0.22f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::NightFoot,		"Nightstalker Footgear",	EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		350,	475,	0,		0,		270,	0.f,	0,		0.f,	0,		0.31f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::TitanTrek,		"Titan's Trekboots",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		476,	581,	0,		0,		377,	0.f,	0,		0.f,	0,		0.45f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::FlameWalk,		"Flameborn Walkers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		582,	696,	0,		0,		570,	0.f,	0,		0.f,	0,		0.56f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelFoot,		"Celestial Footfalls",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		697,	837,	0,		0,		837,	0.f,	0,		0.f,	0,		0.68f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::StarFoot,		"Starshard Footwear",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,		838,	969,	0,		0,		1156,	0.f,	0,		0.f,	0,		0.83f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//jewlery
	
	//weapons Bow
	_item_data.push_back({ EItemID::ShortBow,		"Short Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		3,		35,		10,		20,		0,		0.f,	0,		0.f,	0,		0.f,	0.03f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::LongBow,		"Long Bow",					EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		36,		78,		26,		42,		0,		0.f,	0,		0.f,	0,		0.f,	0.07f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::RecBow,			"Recurve Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		79,		153,	51,		65,		0,		0.f,	0,		0.f,	0,		0.f,	0.12f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CompBow,		"Composite Bow",			EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		154,	241,	78,		102,	0,		0.f,	0,		0.f,	0,		0.f,	0.18f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::RangBow,		"Ranger's Bow",				EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		242,	349,	123,	156,	0,		0.f,	0,		0.f,	0,		0.f,	0.24f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::ElvenLBow,		"Elven Long Bow",			EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		350,	475,	182,	246,	0,		0.f,	0,		0.f,	0,		0.f,	0.35f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::DragBow,		"Dragonwing Bow",			EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		476,	581,	315,	390,	0,		0.f,	0,		0.f,	0,		0.f,	0.48f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoBow,			"Phoenix Fire Bow",			EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		582,	696,	470,	595,	0,		0.f,	0,		0.f,	0,		0.f,	0.63f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::InfFurBow,		"Inferno Fury Bow",			EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		697,	837,	735,	890,	0,		0.f,	0,		0.f,	0,		0.f,	0.79f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::SoulBow,		"Soulreaper Bow",			EItemType::WEAPON,		EWeaponType::BOW,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		838,	969,	1100,	1405,	0,		0.f,	0,		0.f,	0,		0.f,	0.95f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//Staff
	_item_data.push_back({ EItemID::WoodStf,		"Wooden Staff",				EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		3,		35,		6,		12,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.7f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::AppStf,			"Apprentice's Staff",		EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		36,		78,		18,		27,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.15f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::SageStick,		"Sage's Walking Stick",		EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		79,		153,	35,		47,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.27f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::SorcCane,		"Sorcerer's Cane",			EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		154,	241,	46,		61,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.45f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::SunStf,			"Sunwood Staff",			EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		242,	349,	73,		93,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.60f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::GlacRod,		"Glacial Rod",				EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		350,	475,	109,	147,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.82f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::EarthStf,		"Earthshaker Staff",		EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		476,	581,	189,	234,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	1.05f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelScep,		"Celestial Scepter",		EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		582,	696,	282,	357,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	1.43f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::EclStf,			"Eclipse Staff",			EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		697,	837,	441,	534,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	2.01f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::VoidStf,		"Voidwalker's Staff",		EItemType::WEAPON,		EWeaponType::STAFF,		EItemSlot::WPN_MAIN,	EEffectID::NONE,		838,	969,	660,	843,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	2.69f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//Axe 2H
	_item_data.push_back({ EItemID::TimbAxe,		"Timber Axe",				EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		3,		35,		16,		18,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::IronSplt,		"Iron Splitter",			EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		36,		78,		35,		37,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::BerHatch,		"Berserker Hatchet",		EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		79,		153,	55,		58,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::StReav,			"Steel Reaver",				EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		154,	241,	87,		91,		0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::RavCleav,		"Ravager's Cleaver",		EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		242,	349,	131,	140,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::WarChop,		"Warlord's Chopper",		EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		350,	475,	207,	221,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::NightEx,		"Nightfall Executioner",	EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		476,	581,	333,	351,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	1.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::InfEdge,		"Inferno Edge",				EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		582,	696,	495,	535,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	1.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::AbDestr,		"Abyssal Destroyer",		EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		697,	837,	775,	801,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	2.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	_item_data.push_back({ EItemID::SoulHarv,		"Soul Harvester",			EItemType::WEAPON,		EWeaponType::AXE_2H,	EItemSlot::WPN_MAIN,	EEffectID::NONE,		838,	969,	1184,	1264,	0,		0.f,	0,		0.f,	0,		0.f,	0.f,	2.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0,		false });
	//Mace 2H

	//Sword 2H



	//relic

};
ItemDatabase::ItemDataConstructor ItemDatabase::_item_data_constructor;