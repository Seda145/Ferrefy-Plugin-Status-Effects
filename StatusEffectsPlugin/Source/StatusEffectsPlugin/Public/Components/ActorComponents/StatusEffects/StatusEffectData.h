/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "StatusEffectData.generated.h"


/* Struct used to set up a status effect from a datatable. */
USTRUCT(BlueprintType)
struct STATUSEFFECTSPLUGIN_API FS_StatusEffectData : public FTableRowBase {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "StatusEffect", EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
        float Amount;

    UPROPERTY(BlueprintReadWrite, Category = "StatusEffect", EditAnywhere, meta = (ClampMin = "0", UIMin = "0"))
        float MaxAmount;

    // Initialize
    FS_StatusEffectData()
        : Amount (100.f)
        , MaxAmount (100.f)
    {}
    FS_StatusEffectData(float InAmount, float InMaxAmount)
        : Amount (InAmount)
        , MaxAmount (InMaxAmount)
    {}

    // Operators
    bool operator==(const FS_StatusEffectData& Other) const {
        return (
            Amount == Other.Amount
            && MaxAmount == Other.MaxAmount
        );
    }
};