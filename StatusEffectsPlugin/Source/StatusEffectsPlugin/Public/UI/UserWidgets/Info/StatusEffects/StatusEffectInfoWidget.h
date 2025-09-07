/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "StatusEffectInfoWidget.generated.h"


class UStatusEffect;
class UStatusEffectsComponent;


UCLASS(Abstract, BlueprintType, Blueprintable, meta = (DontUseGenericSpawnObject = "True"))
class STATUSEFFECTSPLUGIN_API UStatusEffectInfoWidget : public UUserWidget {
    GENERATED_BODY()

private:

	// Setup

	UPROPERTY(Transient)
		UStatusEffectsComponent* StatusEffectsComponent = nullptr;

	/* Identifier of the status effect to track. */
	UPROPERTY(EditAnywhere, Category = "StatusEffect")
		FName StatusEffectId = NAME_None;

	UPROPERTY(Transient)
		UStatusEffect* StatusEffect = nullptr;

protected:
	
public:

private:

protected:

    // Setup

    virtual void NativeOnInitialized() override;

	// Appearance

	/**
	* Widget desigers can override this method to implement a visual response to data from the status effect. 
	* Called automatically in response to GetStatusEffect()->OnStatusDataChanged. 
	* Also called when the status effect is inserted or removed.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Appearance")
		void UpdateStatusEffectAppearance(UStatusEffect* InStatusEffect);

    // Delegates

	/* Called when any status effect is inserted into the status effects component. */
    UFUNCTION()
		void ActOnInsertedStatusEffect(const FName& InStatusEffectId);

	/* Called when any status effect is removed from the status effects component. */
    UFUNCTION()
		void ActOnRemovedStatusEffect(const FName& InStatusEffectId);

	/* Called data of the status effect we are tracking changes. */
    UFUNCTION()
		void ActOnStatusDataChanged(UStatusEffect* InStatusEffect);

public:

	// Status Effect

	/**
	* This method must be called to bind the widget to the correct status effect component, and a single status effect identified by InStatusEffectId.
	* Automatically calls UnBindStatusEffect before proceeding.
	* The UStatusEffectsComponent is often placed on a Pawn (tracking health points), or a different actor for a different purpose.
	*/
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
		void BindStatusEffect(UStatusEffectsComponent* InComponent, const FName& InStatusEffectId);
	
	/** 
	* Removes bindings to the status effect component, status effect
	* , then proceeds to clear state relevant to them (effect id, pointers to component and effect).
	* Automatically called by BindStatusEffect before it proceeds binding / rebinding. 
	*/
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
		void UnBindStatusEffect();

	/* Returns the status effects component we want to track. This component can hold multiple status effects. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatusEffect")
		UStatusEffectsComponent* GetStatusEffectsComponent() const;

	/* Returns the identifier used to track a single status effect on the status effects component. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatusEffect")
		const FName& GetStatusEffectId() const;

	/* Returns the status effect tracked by GetStatusEffectId(). */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "StatusEffect")
		UStatusEffect* GetStatusEffect() const;
	
    // Appearance

	/** 
	* Utility. Calls SetVisibility(ESlateVisibility::HitTestInvisible). 
	* Visibility is auto managed in response to presence of the status effect on the status effects component. 
	*/
    UFUNCTION(BlueprintCallable, Category = "Appearance")
		void Show();

	/** 
	* Utility. Calls SetVisibility(ESlateVisibility::Collapsed). 
	* Visibility is auto managed in response to presence of the status effect on the status effects component. 
	*/
    UFUNCTION(BlueprintCallable, Category = "Appearance")
		void Hide();
	
};