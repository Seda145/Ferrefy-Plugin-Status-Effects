/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "StatusEffect.h"
#include "StatusEffectsData.h"
#include "StatusEffectData.h"
#include "StatusEffectModifierData.h"

#include "StatusEffectsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectInsertedOrRemoved, const FName&, InStatusEffectId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectModifierChanged, const FName&, InModifierId);


UCLASS(editinlinenew, meta = (BlueprintSpawnableComponent))
class STATUSEFFECTSPLUGIN_API UStatusEffectsComponent : public UActorComponent {
	GENERATED_BODY()

private:

	// Data

	UPROPERTY(Transient)
		TMap<FName, UStatusEffect*> StatusEffects;

	UPROPERTY(Transient)
		TMap<FName, FS_StatusEffectModifierData> StatusEffectModifiers;

	/* Remaining duration in seconds per modifier. */
	UPROPERTY(Transient)
		TMap<FName, float> RemainingModifiersDurationSeconds;

protected:

public:

	// Delegates

	UPROPERTY(BlueprintAssignable, Category = "StatusEffects|Delegates")
		FOnStatusEffectInsertedOrRemoved OnInsertedStatusEffect;
		
	UPROPERTY(BlueprintAssignable, Category = "StatusEffects|Delegates")
		FOnStatusEffectInsertedOrRemoved OnRemovedStatusEffect;

	UPROPERTY(BlueprintAssignable, Category = "StatusEffects|Delegates")
		FOnStatusEffectModifierChanged OnAddedModifier;
		
	UPROPERTY(BlueprintAssignable, Category = "StatusEffects|Delegates")
		FOnStatusEffectModifierChanged OnRemovedModifier;
		
private:

	// Tick

	void TickUpdateModifiers(float InDeltaTime);

protected:

public:

	// Setup

	UStatusEffectsComponent();

	// Tick

	virtual void TickComponent(float InDeltaTime, enum ELevelTick InTickType, FActorComponentTickFunction* InThisTickFunction) override;

	// Stat

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatusEffects|Effects")
		const TMap<FName, UStatusEffect*>& GetStatusEffects() const;

	/* Gets a stat by its type. The requested stat must be inserted. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatusEffects|Effects")
		UStatusEffect* GetStatusEffect(const FName& InStatusEffectId) const;

	/* Insert multiple stats at once. A stat can only be inserted once, mapped to its identifier. This method should be used to set up the initial stats. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects|Effects")
		void InsertStatusEffects(const TMap<FName, FS_StatusEffectData>& InStats);

	/* Remove a stat by its identifier. Can only remove a stat if one is inserted with this identifier. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects|Effects")
		void RemoveStatusEffect(const FName& InStatusEffectId);

	// Modifiers

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatusEffects|Modifiers")
		const TMap<FName, FS_StatusEffectModifierData>& GetModifiers() const;

	/* Add a modifier, which is an additive effect applied to a stat over a duration. If the ID is invalid, one will be generated. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects|Modifiers")
		void AddModifiers(const TMap<FName, FS_StatusEffectModifierData>& InStatModifiers);

	/* Remove the first modifier that matches the ID. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects|Modifiers")
		void RemoveModifier(const FName& InModifierId);

};
