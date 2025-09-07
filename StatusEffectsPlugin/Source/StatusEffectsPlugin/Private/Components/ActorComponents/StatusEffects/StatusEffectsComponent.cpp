/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#include "StatusEffectsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LogStatusEffectsPlugin.h"
#include "StatusEffectsData.h"
#include "UObject/ConstructorHelpers.h"


// Setup

UStatusEffectsComponent::UStatusEffectsComponent() {
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Tick

void UStatusEffectsComponent::TickComponent(float InDeltaTime, enum ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction) {
	TickUpdateModifiers(InDeltaTime);
}

void UStatusEffectsComponent::TickUpdateModifiers(float InDeltaTime) {
	TArray<FName> KeysToRemove;
	for (const TPair<FName, FS_StatusEffectModifierData>& ModifierX : StatusEffectModifiers) {
		UStatusEffect* Stat = GetStatusEffect(ModifierX.Value.StatusEffectId);
		if (!IsValid(Stat)) {
			UE_LOG(LogStatusEffectsPlugin, Warning, TEXT("TickUpdateModifiers: Could not process a modifier, because there is no inserted stat of a matching effect type."));
			continue;
		}

		// A modifier registers two types of amounts. 1. X% over total, 2. constant amount. 
		float AmountFromPercentage = ((Stat->GetMaxAmount() / 100) * ModifierX.Value.AmountInPercentage);
		float Amount = ModifierX.Value.Amount;

		const float TotalDuration = FMath::Max(ModifierX.Value.OverDurationSeconds, 0.f);
		if (FMath::IsNearlyZero(TotalDuration)) {
			// Apply all at once. Nothing to see here, the amounts have been set.
		}
		else {
			// Amount per second.
			AmountFromPercentage *= 1.f / TotalDuration;
			Amount *= 1.f / TotalDuration;
			// Delta time is used as a multiplier to get that value per frame.
			float DurationFrame = InDeltaTime;
			if (!ModifierX.Value.bLoopInfinitely) {
				// When not looping infinitely, it is possible the delta time is larger than the remaining modifier time. This must be clamped.
				// Expected is that remaining duration here is always less than total.
				float& RemainingDurationRef = RemainingModifiersDurationSeconds.FindChecked(ModifierX.Key);
				DurationFrame = FMath::Min(InDeltaTime, RemainingDurationRef);
				// Reduce remaining time when not looping. This should not get below 0.
				RemainingDurationRef -= DurationFrame;
			}

			// Multiplied with DurationFrame, we get the value to apply for this frame.
			AmountFromPercentage *= DurationFrame;
			Amount *= DurationFrame;
		}

		// Modify the stat with the combined (constant and %) amount.
		Stat->ModifyAmount(AmountFromPercentage + Amount);

		// If a non looping modifier has no remaining duration, remove it.
		if (!ModifierX.Value.bLoopInfinitely) {
			if (FMath::IsNearlyZero(RemainingModifiersDurationSeconds.FindChecked(ModifierX.Key))) {
				KeysToRemove.Add(ModifierX.Key);
			}
		}
	}

	for (const FName& KeyX : KeysToRemove) {
		// Out of the loop, we can do the deletions.
		RemoveModifier(KeyX);
	}
}

// StatusEffects

const TMap<FName, UStatusEffect*>& UStatusEffectsComponent::GetStatusEffects() const {
	return StatusEffects;
}

UStatusEffect* UStatusEffectsComponent::GetStatusEffect(const FName& InStatusEffectId) const {
	UStatusEffect* const* StatPtr = StatusEffects.Find(InStatusEffectId);
	if (StatPtr == nullptr) {
		UE_LOG(LogStatusEffectsPlugin, Warning, TEXT("GetStatusEffect: Stat with identifier %s was not found."), *InStatusEffectId.ToString());
	}
	return StatPtr != nullptr ? *StatPtr : nullptr;
}

void UStatusEffectsComponent::InsertStatusEffects(const TMap<FName, FS_StatusEffectData>& InStats) {
	for (const TPair<FName, FS_StatusEffectData>& StatX : InStats) {
		// Remove a stat in case the key is already present.
		RemoveStatusEffect(StatX.Key);
		
		// Add new data
		UStatusEffect* NewStat = NewObject<UStatusEffect>(this);
		check(IsValid(NewStat));

		NewStat->SetMaxAmount(StatX.Value.MaxAmount);
		NewStat->SetAmount(StatX.Value.Amount);
		StatusEffects.Add(StatX.Key, NewStat);
		OnInsertedStatusEffect.Broadcast(StatX.Key);
	}
}

void UStatusEffectsComponent::RemoveStatusEffect(const FName& InStatusEffectId) {
	if (StatusEffects.Remove(InStatusEffectId) > 0) {
		OnRemovedStatusEffect.Broadcast(InStatusEffectId);
	}
}

// Modifiers

const TMap<FName, FS_StatusEffectModifierData>& UStatusEffectsComponent::GetModifiers() const {
	return StatusEffectModifiers;
}

void UStatusEffectsComponent::AddModifiers(const TMap<FName, FS_StatusEffectModifierData>& InStatModifiers) {
	for (const TPair<FName, FS_StatusEffectModifierData>& ModifierDataX : InStatModifiers) {
		// Clean up if required.
		RemoveModifier(ModifierDataX.Key);

		// Add new data
		StatusEffectModifiers.Add(ModifierDataX);
		// Always add this, even if OverDurationSeconds is 0.
		RemainingModifiersDurationSeconds.Add(ModifierDataX.Key, FMath::Max(ModifierDataX.Value.OverDurationSeconds, 0.f));
		OnAddedModifier.Broadcast(ModifierDataX.Key);
	}
}

void UStatusEffectsComponent::RemoveModifier(const FName& InModifierId) {
	int32 Removed = 0;
	Removed += RemainingModifiersDurationSeconds.Remove(InModifierId);
	Removed += StatusEffectModifiers.Remove(InModifierId);
	if (Removed > 0) {
		OnRemovedModifier.Broadcast(InModifierId);
	}
}
