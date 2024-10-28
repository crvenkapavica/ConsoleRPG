#include "Character.h"
#include "../Spells/SpellManager.h"
#include "../GameplayStatics.h"
#include "../Spells/PassiveSpell.h"
#include "../Spells/ActiveSpell.h"

Character::Character(CharacterData Data, PlayerAttributes Attributes, char Alias)
	: CharacterAttributes(Attributes)
	, Alias(Alias)
	, Team(1)
{
	CharacterClass = Data._class;
	Health = Data.Health;
	Essence = Data.Essence;
	Stamina = Data.Stamina;
	Armor = Data.Armor;
	AttackPower = Data.AttackPower;
	CritChance = Data.CritChance;
	CritDamage = Data.CritDamage;
	SpellPower = Data.SpellPower;
	SpellCritChance = Data.SpellCritChance;
	SpellCritDamage = Data.SpellCritDamage;
	DamageType = Data.DamageType;
	CharacterResistances = Data.Resistances;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CreateActiveSpell(this, Data.Spell1.first);
	sm.CreateActiveSpell(this, Data.Spell2.first);
	sm.CreateActiveSpell(this, Data.Spell3.first);
	sm.CreateActiveSpell(this, Data.Spell4.first);

	sm.CreateActiveSpell(this, ESpellID::MELEE);
	sm.CreateActiveSpell(this, ESpellID::RANGED);

	sm.CreatePassiveSpell(this, Data.Passive1.first);
	sm.CreatePassiveSpell(this, Data.Passive2.first);

	// TODO === ADD MAIN_HAND AND OFF_HAND WEAPON SLOTS TO CHARACTER


	// For player Characters
	InitStatsPerAttribute();
	InitStats();
}

Character::Character(CharacterData Data, char Alias)
	: Alias(Alias)
	, Team(2)
{
	CharacterClass = Data._class;
	Health = Data.Health;
	Essence = Data.Essence;
	Stamina = Data.Stamina;
	Armor = Data.Armor;
	AttackPower = Data.AttackPower;
	CritChance = Data.CritChance;
	CritDamage = Data.CritDamage;
	SpellPower = Data.SpellPower;
	SpellCritChance = Data.SpellCritChance;
	SpellCritDamage = Data.SpellCritDamage;
	DamageType = Data.DamageType;
	CharacterResistances = Data.Resistances;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CreateActiveSpell(this, Data.Spell1.first);
	sm.CreateActiveSpell(this, Data.Spell2.first);
	sm.CreateActiveSpell(this, Data.Spell3.first);
	sm.CreateActiveSpell(this, Data.Spell4.first);

	//sm.CreateActiveSpell(this, ESpellID::MELEE);
	//sm.CreateActiveSpell(this, ESpellID::RANGED);

	sm.CreatePassiveSpell(this, Data.Passive1.first);
	sm.CreatePassiveSpell(this, Data.Passive2.first);

	// TODO === ADD MAIN_HAND AND OFF_HAND WEAPON SLOTS TO CHARACTER
}

Character::Character(CharacterData Data, int Team, std::function<char(void)> Alias)
	: Alias(Alias())
	, Team(Team)
{
	CharacterClass = Data._class;
	Health = Data.Health;
	Essence = Data.Essence;
	Stamina = Data.Stamina;
	Armor = Data.Armor;
	AttackPower = Data.AttackPower;
	CritChance = Data.CritChance;
	CritDamage = Data.CritDamage;
	SpellPower = Data.SpellPower;
	SpellCritChance = Data.SpellCritChance;
	SpellCritDamage = Data.SpellCritDamage;
	DamageType = Data.DamageType;
	CharacterResistances = Data.Resistances;

	SpellManager& sm = SpellManager::GetInstance();
	sm.CreateActiveSpell(this, Data.Spell1.first);
	sm.CreateActiveSpell(this, Data.Spell2.first);
	sm.CreateActiveSpell(this, Data.Spell3.first);
	sm.CreateActiveSpell(this, Data.Spell4.first);

	sm.CreateActiveSpell(this, ESpellID::MELEE);
	sm.CreateActiveSpell(this, ESpellID::RANGED);

	sm.CreatePassiveSpell(this, Data.Passive1.first);
	sm.CreatePassiveSpell(this, Data.Passive2.first);
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

Character::Character(Character&& character) noexcept
	: CharacterClass(character.CharacterClass)
	, CharacterAttributes(character.CharacterAttributes)
	, DamageType(character.DamageType)
	, Alias(character.Alias)
	, Team(character.Team)
{}

Character::~Character() {}

void Character::Stat::UpdateBase(const float Value) {
	Base += Value;
	Actual += Value;
	Max += Value;
}

void Character::Stat::SetActual(const float Value) {
	Actual = Value;
}

void Character::Stat::SetMax(float Value) {
	Max = Value;
	
	if (Actual > Max)
		Actual = Max;
}

void Character::Stat::UpdateActual(const float Value, Character* character) {
	Actual += Value;
	if (Actual > Max)
		Actual = Max;
}

void Character::Stat::UpdateMax(const float Value) {
	Max += Value;

	if (Value > 0)
		Actual += Value;
	
	if (Max < Actual)
		Actual = Max;
}

void Character::InitStats() {

	for (int idx = 0; idx < StatPerAttribute.size(); ++idx)
		if (StatPerAttribute[idx].first == &CharacterAttributes.Strength)
			for (int i = 0; i < StatPerAttribute[idx].second.size(); i++)
				StatPerAttribute[idx].second[i].first->UpdateBase(StatPerAttribute[idx].second[i].second * (CharacterAttributes.Strength + Item_Strength));
		else if (StatPerAttribute[idx].first == &CharacterAttributes.Agility)
			for (int i = 0; i < StatPerAttribute[idx].second.size(); i++)
				StatPerAttribute[idx].second[i].first->UpdateBase(StatPerAttribute[idx].second[i].second * (CharacterAttributes.Agility + Item_Agility));
		else if (StatPerAttribute[idx].first == &CharacterAttributes.Intelligence)
			for (int i = 0; i < StatPerAttribute[idx].second.size(); i++)
				StatPerAttribute[idx].second[i].first->UpdateBase(StatPerAttribute[idx].second[i].second * (CharacterAttributes.Intelligence + Item_Intelligence));
		else if (StatPerAttribute[idx].first == &CharacterAttributes.Vitality)
			for (int i = 0; i < StatPerAttribute[idx].second.size(); i++)
				StatPerAttribute[idx].second[i].first->UpdateBase(StatPerAttribute[idx].second[i].second * (CharacterAttributes.Vitality + Item_Vitality));
		else if (StatPerAttribute[idx].first == &CharacterAttributes.Consciousness)
			for (int i = 0; i < StatPerAttribute[idx].second.size(); i++)
				StatPerAttribute[idx].second[i].first->UpdateBase(StatPerAttribute[idx].second[i].second * (CharacterAttributes.Consciousness + Item_Consciousness));
		else if (StatPerAttribute[idx].first == &CharacterAttributes.Endurance)
			for (int i = 0; i < StatPerAttribute[idx].second.size(); i++)
				StatPerAttribute[idx].second[i].first->UpdateBase(StatPerAttribute[idx].second[i].second * (CharacterAttributes.Endurance + Item_Endurance));
}

void Character::UpdateAttribute(attribute& Attribute, int Amount) {

	for (int idx = 0; idx < StatPerAttribute.size(); ++idx)
		if (StatPerAttribute[idx].first == &Attribute) 
			for (int i = 0; i < StatPerAttribute[idx].second.size(); i++) 
				StatPerAttribute[idx].second[i].first->UpdateBase(StatPerAttribute[idx].second[i].second * Amount);

	Attribute += Amount;
}

void Character::AddActiveSpell(unique_ptr<ActiveSpell>& Spell) {
	ActiveSpells.push_back(move(Spell));
}

void Character::AddPassiveSpell(unique_ptr<PassiveSpell>& spell) {
	PassiveSpells.push_back(move(spell));
}

void Character::RemoveEffectById(ESpellID EffectId) {
	for (auto it = EffectIds.begin(); it != EffectIds.end();)
		if (*it == EffectId)
			it = EffectIds.erase(it);
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

	stat_pair stat_vector;

	stat_vector.push_back(make_pair(&CritDamage, 5.f));
	stat_vector.push_back(make_pair(&AttackPower, 12.f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Strength, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&Armor, 20.f));
	stat_vector.push_back(make_pair(&CritChance, 0.002f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Agility, stat_vector));

	stat_vector.clear();
	//stat_vector.push_back(make_pair(&_spell_crit_chance, 0.00125f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Intelligence, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&Health, 10.f));
	stat_vector.push_back(make_pair(&Stamina, 2.f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Vitality, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&Essence, 5.f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Consciousness, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&Stamina, 15.f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Endurance, stat_vector));
}

void Character::InitStatsPerAttribute_Warlock() {
	stat_pair stat_vector;

	stat_vector.push_back(make_pair(&CritDamage, 1.f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Strength, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&Armor, 5.f));
	stat_vector.push_back(make_pair(&CritChance, 0.002f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Agility, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&SpellCritChance, 0.005f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Intelligence, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&Health, 8.f));
	stat_vector.push_back(make_pair(&Stamina, 3.f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Vitality, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&Essence, 15.f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Consciousness, stat_vector));

	stat_vector.clear();
	stat_vector.push_back(make_pair(&Stamina, 5.f));
	StatPerAttribute.push_back(make_pair(&CharacterAttributes.Endurance, stat_vector));
}

void Character::EndTurn() {
	GameplayStatics::EndTurn(this);
}

bool Character::CheckDie() {
	if (GetHealth().GetActual() <= 0.005f) {
		bIsAlive = false;
		return true;
	}
	return false;
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
