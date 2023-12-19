#pragma once

#include "RPGTypes.h"

class Resistances {

public:
	Resistances();
	Resistances(float arcane, float fire, float lightning, float cold, float poison, float necrotic);

	static float CalculateDamage(float damage, EDamageType damage_type, Character* target);

	inline float& GetArcaneRes() { return _arcane_res; }
	inline float& GetFireRes() { return _fire_res; }
	inline float& GetLightningRes() { return _lightning_res; }
	inline float& GetColdRes() { return _cold_res; }
	inline float& GetPoisonRes() { return _poison_res; }
	inline float& GetNecroticRes() { return _necrotic_res; }

	inline void UpdateArcaneRes(const float value) { _arcane_res += value; }
	inline void UpdateFireRes(const float value) { _fire_res += value; }
	inline void UpdateLightningRes(const float value) { _lightning_res += value; }
	inline void UpdateColdRes(const float value) { _cold_res += value; }
	inline void UpdatePoisonRes(const float value) { _poison_res += value; }
	inline void UpdateNecroticRes(const float value) { _necrotic_res += value; }

private:
	float	_arcane_res;
	float	_fire_res;
	float	_lightning_res;
	float	_cold_res;
	float	_poison_res;
	float	_necrotic_res;
};