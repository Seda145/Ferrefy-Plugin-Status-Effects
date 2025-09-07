/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#include "StatusEffect.h"
#include "LogStatusEffectsPlugin.h"


// Stat

float UStatusEffect::GetAmount() const {
	return Amount;
}

void UStatusEffect::SetAmount(float InAmount) {
	const float OldValue = GetAmount();
	Amount = FMath::Clamp(InAmount, 0.f, GetMaxAmount());
	
	if (!FMath::IsNearlyEqual(GetAmount(), OldValue)) {
		OnStatusDataChanged.Broadcast(this);

		if (FMath::IsNearlyZero(GetAmount())) {
			OnStatusAmountIsZero.Broadcast(this);
		}
		else if (FMath::IsNearlyEqual(GetAmount(), GetMaxAmount())) {
			OnStatusAmountIsMax.Broadcast(this);
		}
	}
}

float UStatusEffect::GetMaxAmount() const {
	return MaxAmount;
}

void UStatusEffect::SetMaxAmount(float InMaxAmount) {
	const float OldValue = GetMaxAmount();
	MaxAmount = FMath::Max(InMaxAmount, 0.f);
	
	if (!FMath::IsNearlyEqual(GetMaxAmount(), OldValue)) {
		OnStatusDataChanged.Broadcast(this);
	}
	
	SetAmount(FMath::Clamp(GetAmount(), 0.f, GetMaxAmount()));
}

void UStatusEffect::DecreaseAmount(float InAmount) {
	// Send a negative value to ModifyAmount.
	ModifyAmount(FMath::Max(InAmount, 0.f) * -1.f);
}

void UStatusEffect::IncreaseAmount(float InAmount) {
	// Send a positive value to ModifyAmount.
	ModifyAmount(FMath::Max(InAmount, 0.f));
}

void UStatusEffect::ModifyAmount(float InAmount) {
	SetAmount(GetAmount() + InAmount);
}

