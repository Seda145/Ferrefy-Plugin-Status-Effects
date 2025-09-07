/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Misc/Guid.h"
#include "LogStatusEffectsPlugin.h"

#include "StatusEffectModifierData.generated.h"


USTRUCT(BlueprintType)
struct STATUSEFFECTSPLUGIN_API FS_StatusEffectModifierData : public FTableRowBase {
	GENERATED_BODY()

	/* Identifier of the status effect to modify */
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffectModifier", EditAnywhere)
		FName StatusEffectId;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffectModifier", EditAnywhere)
		float Amount;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffectModifier", EditAnywhere, meta = (ClampMin = "-100", UIMin = "-100", ClampMax = "100", UIMax = "100"))
		float AmountInPercentage;

	/* Duration in seconds over which the total amount of the modifier is applied. */
	UPROPERTY(BlueprintReadWrite, Category = "StatusEffectModifier", EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
		float OverDurationSeconds;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffectModifier", EditAnywhere)
		bool bLoopInfinitely;

	// Initialize
	FS_StatusEffectModifierData()
		: StatusEffectId(NAME_None)
		, Amount (0.f)
		, AmountInPercentage (0.f)
		, OverDurationSeconds (0.f)
		, bLoopInfinitely (false)
	{}
	FS_StatusEffectModifierData(const FName& InStatusEffectId, float InAmount, float InAmountInPercentage, float InOverDurationSeconds, bool bInLoopInfinitely)
		: StatusEffectId(InStatusEffectId)
		, Amount (InAmount)
		, AmountInPercentage (FMath::Clamp(InAmountInPercentage, -100.f, 100.f))
		, OverDurationSeconds (InOverDurationSeconds)
		, bLoopInfinitely (bInLoopInfinitely)
	{}

	// Operators
	bool operator==(const FS_StatusEffectModifierData& Other) const {
		return (
			StatusEffectId == Other.StatusEffectId
			&& Amount == Other.Amount
			&& AmountInPercentage == Other.AmountInPercentage
			&& OverDurationSeconds == Other.OverDurationSeconds
			&& bLoopInfinitely == Other.bLoopInfinitely
		);
	}
};

