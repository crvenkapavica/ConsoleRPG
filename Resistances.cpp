#include "Resistances.h"
#include "Characters/Character.h"

Resistances::Resistances()
	: _arcane_res(0.f)
	, _fire_res(0.f)
	, _lightning_res(0.f)
	, _cold_res(0.f)
	, _poison_res(0.f)
	, _necrotic_res(0.f)
{}

Resistances::Resistances(float arcane, float fire, float lightning, float cold, float poison, float necrotic)
	: _arcane_res(arcane)
	, _fire_res(fire)
	, _lightning_res(lightning)
	, _cold_res(cold)
	, _poison_res(poison)
	, _necrotic_res(necrotic)
{}

float Resistances::CalculateDamage(float damage, EDamageType damage_type, Character* target) {

	float resistance;
	switch (damage_type) {
		case EDamageType::ARCANE: {
			resistance = target->GetResistances().GetArcaneRes();
			break;
		}
		case EDamageType::FIRE: {
			resistance = target->GetResistances().GetFireRes();
			break;
		}
		case EDamageType::LIGHTNING: {
			resistance = target->GetResistances().GetLightningRes();
			break;
		}
		case EDamageType::COLD: {
			resistance = target->GetResistances().GetColdRes();
			break;
		}
		case EDamageType::POISON: {
			resistance = target->GetResistances().GetColdRes();
			break;
		}
		case EDamageType::NECROTIC: {
			resistance = target->GetResistances().GetNecroticRes();
		}
		case EDamageType::PHYSICAL: { // this has to be taken care of differently, different formula
			resistance = target->GetArmor().GetActual();
			break;
		}
		case EDamageType::HEALING: {
			resistance = 0.f;
			break;
		}
		default:
			resistance = 0.f;
			break;
	}
	resistance /= 100;
	float resisted = damage * resistance;

	return damage - resisted;
}