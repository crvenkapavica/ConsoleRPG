#pragma once

#include "CharacterData.h"
#include "../Resistances.h"
#include "../RPGTypes.h"

class PassiveSpell;
class ActiveSpell;

class Character {

public:
	Character() = delete;
	Character(const Character& Other);
	Character(Character&& InCharacter) noexcept;
	// Player 
	Character(const CharacterData& InCharacterData, const PlayerAttributes& InAttributes, char InAlias);
	// Enemy
	Character(const CharacterData& InCharacterData, char InAlias);
	// Summon
	Character(const CharacterData& InCharacterData, int InTeam, const std::function<char(void)>& InAlias);
	virtual ~Character() = default;
	
	Character& operator=(const Character& Other) {
		// "reset" all stats that effects might modify
		Health.SetMax(Other.Health.GetMax()); // treba testirati, dali u combatu, nakon bonus gaina, koji povecaju actual, i tako i maximum, da li se maximum restarta na prijasnji nakon bonus expire
		Essence = Other.Essence;
		Stamina = Other.Stamina;
		Armor = Other.Armor;
		AttackPower = Other.AttackPower;
		CritChance = Other.CritChance;
		CritDamage = Other.CritDamage;
		SpellPower = Other.SpellPower;
		SpellCritChance = Other.SpellCritChance;
		SpellCritDamage = Other.SpellCritDamage;
		CharacterResistances = Other.CharacterResistances;
		return *this;
	}
	Character& operator=(Character&& Other) noexcept = default;
	
	virtual void TakeTurn() = 0;

public:
	class Stat {
		float Base;
		float Actual;
		float Max;

	public:
		Stat()
			: Base(0.f)
			, Actual(0.f)
			, Max(0.f)
		{}

		const float& operator=(const float& Value) {
			Base = Value;
			Actual = Value;
			Max = Value;

			return Base;	//TODO: FIXME 
		}

		inline const float& GetBase() const { return Base; }
		inline float& GetActual() { return Actual; }
		inline const float& GetMax() const { return Max; }

		void UpdateBase(const float Value);
		void UpdateActual(const float Value, Character* InCharacter);
		void UpdateMax(const float Value);

		void SetActual(const float Value);
		void SetMax(float Value);

	private:
		//void OnStatChanged();
	};

public:
	void UpdateAttribute(attribute& Attribute, int Amount) const;

	void AddActiveSpell(std::unique_ptr<ActiveSpell>& SpellToAdd);
	void AddPassiveSpell(std::unique_ptr<PassiveSpell>& SpellToAdd);

	inline std::vector<std::unique_ptr<ActiveSpell>>& GetActiveSpells() { return ActiveSpells; }
	inline std::vector<std::unique_ptr<PassiveSpell>>& GetPassiveSpells() { return PassiveSpells; }

	inline void AddEffectId(const ESpellID EffectId) { EffectIds.push_back(EffectId); }
	void RemoveEffectById(ESpellID EffectId);
	inline const std::vector<ESpellID>& GetEffectIds() { return EffectIds; }

public:
	inline Stat& GetHealth() { return Health; }
	inline Stat& GetEssence() { return Essence; }
	inline Stat& GetStamina() { return Stamina; }
	inline Stat& GetArmor() { return Armor; }
	inline Stat& GetAP() { return AttackPower; }
	inline Stat& GetCritChance() { return CritChance; }
	inline Stat& GetCritDmg() { return CritDamage; } 
	inline Stat& GetSP() { return SpellPower; }
	inline Stat& GetSpellCritChance() { return SpellCritChance; }
	inline Stat& GetSpellCritDmg() { return SpellCritDamage; }

	inline ECharacterClass GetClass() const { return CharacterClass; }
	inline char GetAlias() const { return Alias; }
	inline void SetAlias(const char Value) { Alias = Value; }
	inline int GetLevel() const { return Level; }
	inline bool IsInCombat() const { return bIsInCombat; }
	inline bool IsAlive() const { return bIsAlive; }
	inline bool IsOnTurn() const { return bIsOnTurn; }
	inline int GetTeam() const { return Team; }
	inline void SetIsOnTurn(const bool bValue) { bIsOnTurn = bValue; }
	inline void SetIsInCombat(const bool bInCombat) { bIsInCombat = bInCombat; }
	inline const PlayerAttributes& GetPlayerAttributes() const { return CharacterAttributes; }
	inline Resistances& GetResistances() { return CharacterResistances; }

	// Check if targets Health is below 0 and mark it as bIsAlive = false
	bool CheckDie();

public:
	// EXTRA STATS COMBAT
	//////////////////////////////////////////
	// item attributes
	int Item_Strength = 0;
	int Item_Agility = 0;
	int Item_Intelligence = 0;
	int Item_Vitality = 0;
	int Item_Consciousness = 0;
	int Item_Endurance = 0;

	int MultiStrike = 0;
	// damage
	float ArcaneDamage = 0.f;
	float FireDamage = 0.f;
	float LightningDamage = 0.f;
	float ColdDamage = 0.f;
	float PoisonDamage = 0.f;
	float NecroticDamage = 0.f;
	float PhysicalDamage = 0.f;
	
	int	MinDamage = 0;
	int MaxDamage = 0;
	int AvgDamage = 0;

	// healing
	float Healing = 0.f;

	// duration
	int BuffDuration = 0;
	int DebuffDuration = 0;
	/////////////////////////////////////////

	
	// EXTRA STATS OUT OF COMBAT
	/////////////////////////////////////////
	float MagicFind = 1.f;
	int LightRadius = 3;

	int nConsumableSlots = 0;
	int nSpellSlots = 0;
	int nPassiveSlots = 0;
	///////////////////////////////////////////

protected:
	// Apply stat change per attribute
	void InitStats() const;

	// Set stat change per attribute for each class
	void InitStatsPerAttribute();

	void InitStatsPerAttribute_Barbarian();
	void InitStatsPerAttribute_Warlock();

protected:
	ECharacterClass	CharacterClass;

	Stat Health;
	Stat Essence;
	Stat Stamina;

	Stat Armor;
	Stat AttackPower;
	Stat CritChance;
	Stat CritDamage;
	Stat SpellPower;
	Stat SpellCritChance;
	Stat SpellCritDamage;

	PlayerAttributes CharacterAttributes;

	EDamageType	DamageType;

	Resistances	CharacterResistances;

	char Alias;
	int Team;

	int	 Level = 1;

	bool bIsInCombat = false;
	bool bIsAlive = true;
	bool bIsOnTurn = false;

	std::vector<std::unique_ptr<ActiveSpell>> ActiveSpells;
	std::vector<std::unique_ptr<PassiveSpell>> PassiveSpells;

	// ID's (tags) of spell effects
	std::vector<ESpellID> EffectIds;

	using stat_pair = std::vector<std::pair<Stat*, float>>;
	// Stat change per attribute
	std::vector<std::pair<attribute*, stat_pair>> StatPerAttribute;

public:
	void OnMagicReceivedBegin();
	void OnMagicReceivedEnd();
	void OnMeleeReceivedBegin();
	void OnMeleeReceivedEnd();
	void OnRangedReceivedBegin();
	void OnRangedReceivedEnd();
};