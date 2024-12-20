#include "Character.h"
#include "../GameplayStatics.h"
#include "../Combat/CombatManager.h"
#include "../Spells/ActiveSpell.h"
#include "../Spells/PassiveSpell.h"
//#include "../Spells/SpellManager.h"


Character::Character(const CharacterData& InCharacterData, const PlayerAttributes& InAttributes, const char InAlias)
	: CharacterAttributes(InAttributes)
	, Alias(InAlias)
	, Team(1)
{
	CharacterClass = InCharacterData.CharacterClass;
	Health = InCharacterData.Health;
	Essence = InCharacterData.Essence;
	Stamina = InCharacterData.Stamina;
	Armor = InCharacterData.Armor;
	AttackPower = InCharacterData.AttackPower;
	CritChance = InCharacterData.CritChance;
	CritDamage = InCharacterData.CritDamage;
	SpellPower = InCharacterData.SpellPower;
	SpellCritChance = InCharacterData.SpellCritChance;
	SpellCritDamage = InCharacterData.SpellCritDamage;
	DamageType = InCharacterData.DamageType;
	CharacterResistances = InCharacterData.Resistances;

	// using sm = SpellManager;
	// sm::CreateActiveSpell(this, InCharacterData.Spell1.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell2.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell3.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell4.first);
	//
	// sm::CreateActiveSpell(this, ESpellID::MELEE);
	// sm::CreateActiveSpell(this, ESpellID::RANGED);
	//
	// sm::CreatePassiveSpell(this, InCharacterData.Passive1.first);
	// sm::CreatePassiveSpell(this, InCharacterData.Passive2.first);

	// TODO === ADD MAIN_HAND AND OFF_HAND WEAPON SLOTS TO CHARACTER
	
	// For player Characters
	InitStatsPerAttribute();
	InitStats();
}

Character::Character(const CharacterData& InCharacterData, const char InAlias)
	: Alias(InAlias)
	, Team(2)
{
	CharacterClass = InCharacterData.CharacterClass;
	Health = InCharacterData.Health;
	Essence = InCharacterData.Essence;
	Stamina = InCharacterData.Stamina;
	Armor = InCharacterData.Armor;
	AttackPower = InCharacterData.AttackPower;
	CritChance = InCharacterData.CritChance;
	CritDamage = InCharacterData.CritDamage;
	SpellPower = InCharacterData.SpellPower;
	SpellCritChance = InCharacterData.SpellCritChance;
	SpellCritDamage = InCharacterData.SpellCritDamage;
	DamageType = InCharacterData.DamageType;
	CharacterResistances = InCharacterData.Resistances;

	// using sm = SpellManager;
	// sm::CreateActiveSpell(this, InCharacterData.Spell1.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell2.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell3.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell4.first);
	//
	// //sm.CreateActiveSpell(this, ESpellID::MELEE);
	// //sm.CreateActiveSpell(this, ESpellID::RANGED);
	//
	// sm::CreatePassiveSpell(this, InCharacterData.Passive1.first);
	// sm::CreatePassiveSpell(this, InCharacterData.Passive2.first);

	// TODO === ADD MAIN_HAND AND OFF_HAND WEAPON SLOTS TO CHARACTER
}

Character::Character(const CharacterData& InCharacterData, const int InTeam, const std::function<char(void)>& InAlias)
	: Alias(InAlias())
	, Team(InTeam)
{
	CharacterClass = InCharacterData.CharacterClass;
	Health = InCharacterData.Health;
	Essence = InCharacterData.Essence;
	Stamina = InCharacterData.Stamina;
	Armor = InCharacterData.Armor;
	AttackPower = InCharacterData.AttackPower;
	CritChance = InCharacterData.CritChance;
	CritDamage = InCharacterData.CritDamage;
	SpellPower = InCharacterData.SpellPower;
	SpellCritChance = InCharacterData.SpellCritChance;
	SpellCritDamage = InCharacterData.SpellCritDamage;
	DamageType = InCharacterData.DamageType;
	CharacterResistances = InCharacterData.Resistances;

	// using sm = SpellManager;
	// sm::CreateActiveSpell(this, InCharacterData.Spell1.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell2.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell3.first);
	// sm::CreateActiveSpell(this, InCharacterData.Spell4.first);
	//
	// sm::CreateActiveSpell(this, ESpellID::MELEE);
	// sm::CreateActiveSpell(this, ESpellID::RANGED);
	//
	// sm::CreatePassiveSpell(this, InCharacterData.Passive1.first);
	// sm::CreatePassiveSpell(this, InCharacterData.Passive2.first);
}

Character::Character(const Character& Other)
	: CharacterClass(Other.CharacterClass)
	, Health(Other.Health)
	, Essence(Other.Essence)
	, Stamina(Other.Stamina)
	, Armor(Other.Armor)
	, AttackPower(Other.AttackPower)
	, CritChance(Other.CritChance)
	, CritDamage(Other.CritDamage)
	, SpellPower(Other.SpellPower)
	, SpellCritChance(Other.SpellCritChance)
	, SpellCritDamage(Other.SpellCritDamage)
	, CharacterAttributes(Other.CharacterAttributes)
	, DamageType(Other.DamageType)
	, CharacterResistances(Other.CharacterResistances)
	, Alias(Other.Alias)
	, Team(Other.Team)
{}

Character::Character(Character&& Other) noexcept
	: CharacterClass(Other.CharacterClass)
	, CharacterAttributes(Other.CharacterAttributes)
	, DamageType(Other.DamageType)
	, Alias(Other.Alias)
	, Team(Other.Team)
{}

void Character::Stat::UpdateBase(const float Value) {
	Base += Value;
	Actual += Value;
	Max += Value;
}

void Character::Stat::SetActual(const float Value) {
	Actual = Value;
}

void Character::Stat::SetMax(const float Value) {
	Max = Value;
	Actual = std::min(Actual, Max);
}

// TODO: FIXME CharPtr
void Character::Stat::UpdateActual(const float Value, Character* InCharacter) {
	Actual += Value;
	Actual = std::min(Actual, Max);
}

void Character::Stat::UpdateMax(const float Value) {
	Max += Value;
	if (Value > 0) Actual += Value;
	Actual = std::min(Max, Actual);
}

void Character::InitStats() const {
	for (const auto& [Attr, StatPair] : StatPerAttribute)
		if (Attr == &CharacterAttributes.Strength)
			for (auto [Stat, Value] : StatPair)
				Stat->UpdateBase(Value * static_cast<float>((CharacterAttributes.Strength + Item_Strength)));
		else if (Attr == &CharacterAttributes.Agility)
			for (auto [Stat, Value] : StatPair)
				Stat->UpdateBase(Value * static_cast<float>((CharacterAttributes.Agility + Item_Agility)));
		else if (Attr == &CharacterAttributes.Intelligence)
			for (auto [Stat, Value] : StatPair)
				Stat->UpdateBase(Value * static_cast<float>((CharacterAttributes.Intelligence + Item_Intelligence)));
		else if (Attr == &CharacterAttributes.Vitality)
			for (auto [Stat, Value] : StatPair)
				Stat->UpdateBase(Value * static_cast<float>((CharacterAttributes.Vitality + Item_Vitality)));
		else if (Attr == &CharacterAttributes.Consciousness)
			for (auto [Stat, Value] : StatPair)
				Stat->UpdateBase(Value * static_cast<float>((CharacterAttributes.Consciousness + Item_Consciousness)));
		else if (Attr == &CharacterAttributes.Endurance)
			for (auto [Stat, Value] : StatPair)
				Stat->UpdateBase(Value * static_cast<float>((CharacterAttributes.Endurance + Item_Endurance)));
}

void Character::UpdateAttribute(attribute& Attribute, const int Amount) const {
	for (const auto& [Attr, StatPair] : StatPerAttribute)
		if (Attr == &Attribute) 
			for (const auto& [Stat, Value] : StatPair)
				Stat->UpdateBase(Value * static_cast<float>(Amount));

	Attribute += Amount;
}

void Character::AddActiveSpell(std::unique_ptr<ActiveSpell>&& SpellToAdd) {
	ActiveSpells.push_back(std::move(SpellToAdd));
}

void Character::AddPassiveSpell(std::unique_ptr<PassiveSpell>&& SpellToAdd) {
	PassiveSpells.push_back(std::move(SpellToAdd));
}

void Character::RemoveEffectById(const ESpellID EffectId) {
	for (auto it = EffectIds.begin(); it != EffectIds.end();)
		if (*it == EffectId) it = EffectIds.erase(it);
		else ++it;
}

void Character::InitStatsPerAttribute() {

	switch (CharacterClass) {
	case ECharacterClass::BARBARIAN:
		InitStatsPerAttribute_Barbarian();
		break;
	case ECharacterClass::WARLOCK:
		InitStatsPerAttribute_Warlock();
		break;
	default:
		break;
	}
}

void Character::InitStatsPerAttribute_Barbarian() {

	stat_pair StatVector;

	StatVector.emplace_back(&CritDamage, 5.f);
	StatVector.emplace_back(&AttackPower, 12.f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Strength, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&Armor, 20.f);
	StatVector.emplace_back(&CritChance, 0.002f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Agility, StatVector);

	StatVector.clear();
	//stat_vector.push_back(make_pair(&_spell_crit_chance, 0.00125f));
	StatPerAttribute.emplace_back(&CharacterAttributes.Intelligence, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&Health, 10.f);
	StatVector.emplace_back(&Stamina, 2.f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Vitality, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&Essence, 5.f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Consciousness, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&Stamina, 15.f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Endurance, StatVector);
}

void Character::InitStatsPerAttribute_Warlock() {
	stat_pair StatVector;

	StatVector.emplace_back(&CritDamage, 1.f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Strength, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&Armor, 5.f);
	StatVector.emplace_back(&CritChance, 0.002f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Agility, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&SpellCritChance, 0.005f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Intelligence, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&Health, 8.f);
	StatVector.emplace_back(&Stamina, 3.f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Vitality, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&Essence, 15.f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Consciousness, StatVector);

	StatVector.clear();
	StatVector.emplace_back(&Stamina, 5.f);
	StatPerAttribute.emplace_back(&CharacterAttributes.Endurance, StatVector);
}

bool Character::IsAlive() {
	if (GetHealth().GetActual() <= 0.005f)
		return false;
	return true;
}

void Character::OnMagicReceivedBegin() {

}

void Character::OnMagicReceivedEnd() {

}

void Character::OnMeleeReceivedBegin() {

}

void Character::OnMeleeReceivedEnd() {
	GetPassiveSpells()[0]->Apply();
}

void Character::OnRangedReceivedBegin() {

}

void Character::OnRangedReceivedEnd() {

}
