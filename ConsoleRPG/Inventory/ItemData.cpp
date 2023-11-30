#include "ItemData.h"

std::vector<ItemData> ItemDatabase::_item_data;

ItemDatabase::ItemDataConstructor::ItemDataConstructor() {
	//																																					  			      <lvl    >lvl    <dmg    >dmg     arm      value  %chnc   non_combat                                                        
	//consumables
	_item_data.push_back({ EItemID::HPotion,		"Health potion",			EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,	0,		0,		0,		0,		0,		0.25f,	200,	false });
	_item_data.push_back({ EItemID::EPotion,		"Essence potion",			EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,	0,		0,		0,		0,		0,		0.15f,	200,	false });
	_item_data.push_back({ EItemID::FortElixir,		"Elixir of Fortitude",		EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,	0,		0,		0,		0,		0,		0.20f,	200,	false });
	_item_data.push_back({ EItemID::StrElixir,		"Elixir of Strength",		EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,	0,		0,		0,		0,		0,		5.f,	200,	false });
	_item_data.push_back({ EItemID::AgiElixir,		"Elixir of Agility",		EItemType::CONSUMABLE,	EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,	0,		0,		0,		0,		0,		5.f,	200,	false });
	//scrolls
	_item_data.push_back({ EItemID::Scroll,			"Scroll",					EItemType::SCROLL,		EWeaponType::NONE,		EItemSlot::NONE,		EEffectID::NONE,	0,		0,		0,		0,		0,		0.f,	1000,	false });
	//armor Head
	_item_data.push_back({ EItemID::ClothCap,		"Cloth Cap",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	3,		35,		0,		0,		40,		0.f,	0,		false });
	_item_data.push_back({ EItemID::LeatherHood,	"Leather Hood",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	36,		78,		0,		0,		95,		0.f,	0,		false });
	_item_data.push_back({ EItemID::RusticHelm,		"Rustic Helm",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	79,		153,	0,		0,		240,	0.f,	0,		false });
	_item_data.push_back({ EItemID::IronCasque,		"Iron Casque",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	154,	241,	0,		0,		460,	0.f,	0,		false });
	_item_data.push_back({ EItemID::HardColf,		"Hardened Colf",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	242,	349,	0,		0,		690,	0.f,	0,		false });
	_item_data.push_back({ EItemID::GoldDiadem,		"Golden Diadem",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	350,	475,	0,		0,		1080,	0.f,	0,		false });
	_item_data.push_back({ EItemID::WarCasque,		"Warlord's Casque",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	476,	581,	0,		0,		1510,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoCrown,		"Phoenix Crown",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	582,	696,	0,		0,		2280,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelDiadem,		"Celestial Diadem",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	697,	837,	0,		0,		3350,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CrownCosmos,	"Crown of the Cosmos",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	838,	969,	0,		0,		4625,	0.f,	0,		false });
	//Chest
	_item_data.push_back({ EItemID::ClothTunic,		"Cloth Tunic",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	3,		35,		0,		0,		50,		0.f,	0,		false });
	_item_data.push_back({ EItemID::PadVest,		"Padded Vest",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	36,		78,		0,		0,		118,	0.f,	0,		false });
	_item_data.push_back({ EItemID::StudLeather,	"Studded Leather",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	79,		153,	0,		0,		300,	0.f,	0,		false });
	_item_data.push_back({ EItemID::HardCuirass,	"Hardened Cuirass",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	154,	241,	0,		0,		575,	0.f,	0,		false });
	_item_data.push_back({ EItemID::ScaleMail,		"Scale Mail",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	242,	349,	0,		0,		860,	0.f,	0,		false });
	_item_data.push_back({ EItemID::KnightPlate,	"Knight's Plate",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	350,	475,	0,		0,		1350,	0.f,	0,		false });
	_item_data.push_back({ EItemID::DraghideArm,	"Dragonhide Armor",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	476,	581,	0,		0,		1880,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoMail,		"Phoenix Mail",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	582,	696,	0,		0,		2850,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelAegis,		"Celestial Aegis",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	697,	837,	0,		0,		4180,	0.f,	0,		false });
	_item_data.push_back({ EItemID::ArmAncient,		"Armor of the Ancients",	EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::CHEST,		EEffectID::NONE,	838,	969,	0,		0,		5780,	0.f,	0,		false });
	//Hands
	_item_data.push_back({ EItemID::ClothWraps,		"Cloth Wraps",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	3,		35,		0,		0,		10,		0.f,	0,		false });
	_item_data.push_back({ EItemID::StudGauntlets,	"Studded Gauntlets",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	36,		78,		0,		0,		23,		0.f,	0,		false });
	_item_data.push_back({ EItemID::IronBracers,	"Iron Bracers",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	79,		153,	0,		0,		32,		0.f,	0,		false });
	_item_data.push_back({ EItemID::ChainGloves,	"Chain Gloves",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	154,	241,	0,		0,		115,	0.f,	0,		false });
	_item_data.push_back({ EItemID::SilvGauntlets,	"Silvered Gauntlets",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	242,	349,	0,		0,		172,	0.f,	0,		false });
	_item_data.push_back({ EItemID::KnightVambrace,	"Knight's Vambrace",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	350,	475,	0,		0,		270,	0.f,	0,		false });
	_item_data.push_back({ EItemID::TitanGrasp,		"Titan's Grasp",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	476,	581,	0,		0,		377,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoTalons,		"Phoenix Talons",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	582,	696,	0,		0,		570,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelHandwraps,	"Celestial Handwrapds",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	697,	837,	0,		0,		837,	0.f,	0,		false });
	_item_data.push_back({ EItemID::InfHandGrds,	"Infernal Handguards",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HANDS,		EEffectID::NONE,	838,	969,	0,		0,		1156,	0.f,	0,		false });
	//Belt
	_item_data.push_back({ EItemID::WovenSash,		"Woven Sash",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	3,		35,		0,		0,		10,		0.f,	0,		false });
	_item_data.push_back({ EItemID::LeatherBelt,	"Leather Belt",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	36,		78,		0,		0,		17,		0.f,	0,		false });
	_item_data.push_back({ EItemID::IronBuckBelt,	"Iron Buckle Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	79,		153,	0,		0,		45,		0.f,	0,		false });
	_item_data.push_back({ EItemID::ReinfGirdle,	"Reinforced Girdle",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	154,	241,	0,		0,		86,		0.f,	0,		false });
	_item_data.push_back({ EItemID::ChainLBelt,		"Chain Link Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	242,	349,	0,		0,		129,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PlatedWaist,	"Plated Waistguard",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	350,	475,	0,		0,		202,	0.f,	0,		false });
	_item_data.push_back({ EItemID::DragBelt,		"Dragonbone Belt",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	476,	581,	0,		0,		283,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoWaistwrap,	"Phoenix Waistwrap",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	582,	696,	0,		0,		427,	0.f,	0,		false });
	_item_data.push_back({ EItemID::AncGirdle,		"Ancient's Girdle",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	697,	837,	0,		0,		628,	0.f,	0,		false });
	_item_data.push_back({ EItemID::InfWaistguard,	"Infernal Waistguard",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::BELT,		EEffectID::NONE,	838,	969,	0,		0,		867,	0.f,	0,		false });
	//Legs
	_item_data.push_back({ EItemID::ThistTrous,		"Thistleweave Trousers",	EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	3,		35,		0,		0,		15,		0.f,	0,		false });
	_item_data.push_back({ EItemID::IronGreaves,	"Ironscale Greaves",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	36,		78,		0,		0,		35,		0.f,	0,		false });
	_item_data.push_back({ EItemID::RunicTrTr,		"Runic Trail-Trousers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	79,		153,	0,		0,		90,		0.f,	0,		false });
	_item_data.push_back({ EItemID::MoonLegg,		"Moonbark Legguards",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	154,	241,	0,		0,		172,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhantStriders,	"Phantom Striders",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	242,	349,	0,		0,		258,	0.f,	0,		false });
	_item_data.push_back({ EItemID::TempWalkers,	"Tempest-Walkers",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	350,	475,	0,		0,		405,	0.f,	0,		false });
	_item_data.push_back({ EItemID::TitanMarch,		"Titan's Marchers",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	476,	581,	0,		0,		566,	0.f,	0,		false });
	_item_data.push_back({ EItemID::AbStrGrvs,		"Abyssal Stride Greaves",	EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	582,	696,	0,		0,		855,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PyrWarKilt,		"Pyreborn War-Kilt",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	697,	837,	0,		0,		1256,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CosmosGreaves,	"Cosmosforged Greaves",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::LEGS,		EEffectID::NONE,	838,	969,	0,		0,		1734,	0.f,	0,		false });
	//Feet
	_item_data.push_back({ EItemID::TrailBBoots,	"Trailblazer Boots",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	3,		35,		0,		0,		10,		0.f,	0,		false });
	_item_data.push_back({ EItemID::CobStompers,	"Cobblestone Stompers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	36,		78,		0,		0,		23,		0.f,	0,		false });
	_item_data.push_back({ EItemID::SerpSlip,		"Serpentcoil Slippers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	79,		153,	0,		0,		60,		0.f,	0,		false });
	_item_data.push_back({ EItemID::EmberGreaves,	"Emberstep Greaves",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	154,	241,	0,		0,		115,	0.f,	0,		false });
	_item_data.push_back({ EItemID::WhispBoots,		"Whisperwind Boots",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	242,	349,	0,		0,		172,	0.f,	0,		false });
	_item_data.push_back({ EItemID::NightFoot,		"Nightstalker Footgear",	EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	350,	475,	0,		0,		270,	0.f,	0,		false });
	_item_data.push_back({ EItemID::TitanTrek,		"Titan's Trekboots",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	476,	581,	0,		0,		377,	0.f,	0,		false });
	_item_data.push_back({ EItemID::FlameWalk,		"Flameborn Walkers",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	582,	696,	0,		0,		570,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelFoot,		"Celestial Footfalls",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	697,	837,	0,		0,		837,	0.f,	0,		false });
	_item_data.push_back({ EItemID::StarFoot,		"Starshard Footwear",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::FEET,		EEffectID::NONE,	838,	969,	0,		0,		1156,	0.f,	0,		false });
	//jewlery
	
	//weapons




	//relic

};
ItemDatabase::ItemDataConstructor ItemDatabase::_item_data_constructor;