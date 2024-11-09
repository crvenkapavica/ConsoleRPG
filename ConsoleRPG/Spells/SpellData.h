#pragma once

#include "../RPGTypes.h"

struct SpellData {
	float   ApplyMin;
	float   ApplyMax;
	float	EffectMin;
	float	EffectMax;
	int		Duration;
	int		Cooldown;
	int		EssenceCost; 
	int		StaminaCost;
};

struct ActiveConstData {
	EDamageType		DamageType;
	ESpellType		SpellType;
	ESpellRarity	Rarity;
	ESpellClass		Class;
	int				RequiredLevel;
	int				PowerLevel;
};

struct PassiveConstData {
	ECombatEvent	CombatEvent;
	ESpellRarity	Rarity;
	ESpellClass		Class;
	int				RequiredLevel;
	int				PowerLevel;
};

class SpellDb {
public:
	SpellDb() = delete;
	
	static std::unordered_map<ESpellID, std::vector<SpellData>> Data;
	static std::unordered_map<ESpellID, ActiveConstData> ActiveConstMap;
	static std::unordered_map<ESpellID, PassiveConstData> PassiveConstMap;

private:
	static struct SpellDataConstructor {
		SpellDataConstructor();
	} SpellDataCtor;
	static struct ActiveConstDataConstructor {
		ActiveConstDataConstructor();
	} ActiveConstDataCtor;
	static struct PassiveConstDataConstructor {
		PassiveConstDataConstructor();
	} PassiveConstDataCtor;
};