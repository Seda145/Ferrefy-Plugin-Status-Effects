/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#pragma once

#include "CoreMinimal.h"

#include "StatusEffect.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectChanged, UStatusEffect*, InStatusEffect);


UCLASS(BlueprintType, Blueprintable)
class STATUSEFFECTSPLUGIN_API UStatusEffect : public UObject {
	GENERATED_BODY()

private:

	// Stat

	UPROPERTY(Transient)
		float Amount = 0.f;

	UPROPERTY(Transient)
		float MaxAmount = 0.f;

protected:

public:

	UPROPERTY(BlueprintAssignable, Category = "StatusEffect|Delegates")
		FOnStatusEffectChanged OnStatusDataChanged;

	UPROPERTY(BlueprintAssignable, Category = "StatusEffect|Delegates")
		FOnStatusEffectChanged OnStatusAmountIsZero;

	UPROPERTY(BlueprintAssignable, Category = "StatusEffect|Delegates")
		FOnStatusEffectChanged OnStatusAmountIsMax;

private:

protected:

public:

	// Stat

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatusEffect")
		float GetAmount() const;

	/* Set the Amount. The Amount property clamps between 0 and MaxAmount. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
		void SetAmount(float InAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatusEffect")
		float GetMaxAmount() const;

	/* Set the MaxAmount. The value will be clamped to a minimum of 0. The Amount property clamps between 0 and MaxAmount. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
		void SetMaxAmount(float InMaxAmount);

	/* Damages Amount by the InAmount. Value must be greater than 0. The Amount property clamps between 0 and MaxAmount. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
		void DecreaseAmount(float InAmount);

	/* Restores Amount by the InAmount. Value must be greater than 0. The Amount property clamps between 0 and MaxAmount. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
		void IncreaseAmount(float InAmount);

	/* This method contains the logic for DamageAmount and RestoreAmount. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
		void ModifyAmount(float InAmount);

};
