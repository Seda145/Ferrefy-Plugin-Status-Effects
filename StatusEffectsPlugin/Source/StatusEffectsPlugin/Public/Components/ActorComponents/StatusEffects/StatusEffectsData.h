/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatusEffectData.h"
#include "StatusEffectModifierData.h"

#include "StatusEffectsData.generated.h"


USTRUCT(BlueprintType)
struct STATUSEFFECTSPLUGIN_API FS_StatusEffectsData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffect", EditAnywhere)
		TMap<FName, FS_StatusEffectData> StatusEffectData;

	UPROPERTY(BlueprintReadWrite, Category = "StatusEffectModifier", EditAnywhere)
		TMap<FName, FS_StatusEffectModifierData> StatusEffectModifiers;

	// Initialize
	FS_StatusEffectsData() 
		: StatusEffectData (TMap<FName, FS_StatusEffectData>())
		, StatusEffectModifiers (TMap<FName, FS_StatusEffectModifierData>())
	{}
	FS_StatusEffectsData(TMap<FName, FS_StatusEffectData> InStatusEffectData, TMap<FName, FS_StatusEffectModifierData> InStatusEffectModifiers)
		: StatusEffectData (InStatusEffectData)
		, StatusEffectModifiers (InStatusEffectModifiers)
	{}

	// Operators
	bool operator==(const FS_StatusEffectsData& Other) const {
		return (
			StatusEffectData.OrderIndependentCompareEqual(Other.StatusEffectData)
			&& StatusEffectModifiers.OrderIndependentCompareEqual(Other.StatusEffectModifiers)
		);
	}
};