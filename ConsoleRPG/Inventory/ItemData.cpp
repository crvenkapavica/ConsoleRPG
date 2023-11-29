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
	//armor
	_item_data.push_back({ EItemID::ClothCap,		"Cloth Cap",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	3,		35,		0,		0,		40,		0.f,	0,		false });
	_item_data.push_back({ EItemID::LeatherHood,	"Leather Hood",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	36,		78,		0,		0,		95,		0.f,	0,		false });
	_item_data.push_back({ EItemID::RusticHelm,		"Rustic Helm",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	79,		153,	0,		0,		240,	0.f,	0,		false });
	_item_data.push_back({ EItemID::IronCasque,		"Iron Casque",				EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	154,	241,	0,		0,		460,	0.f,	0,		false });
	_item_data.push_back({ EItemID::HardColf,		"Hardened Colf",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	242,	389,	0,		0,		690,	0.f,	0,		false });
	_item_data.push_back({ EItemID::GoldDiadem,		"Golden Diadem",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	399,	445,	0,		0,		980,	0.f,	0,		false });
	_item_data.push_back({ EItemID::WarCasque,		"Warlord's Casque",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	446,	581,	0,		0,		1120,	0.f,	0,		false });
	_item_data.push_back({ EItemID::PhoCrown,		"Phoenix Crown",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	582,	696,	0,		0,		1430,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CelDiadem,		"Celestial Diadem",			EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	697,	855,	0,		0,		1905,	0.f,	0,		false });
	_item_data.push_back({ EItemID::CrownCosmos,	"Crown of the Cosmos",		EItemType::ARMOR,		EWeaponType::NONE,		EItemSlot::HEAD,		EEffectID::NONE,	856,	969,	0,		0,		2680,	0.f,	0,		false });
	//weapons

};
ItemDatabase::ItemDataConstructor ItemDatabase::_item_data_constructor;