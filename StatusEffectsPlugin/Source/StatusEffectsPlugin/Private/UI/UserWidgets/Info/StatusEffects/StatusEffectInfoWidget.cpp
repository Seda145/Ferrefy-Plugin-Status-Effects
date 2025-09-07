/**Copyright 2025: Roy Wierer (Ferrefy). All Rights Reserved.**/
#include "StatusEffectInfoWidget.h"
#include "LogStatusEffectsPlugin.h"
#include "StatusEffect.h"
#include "StatusEffectsComponent.h"


// Setup

void UStatusEffectInfoWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	// Widget hides by default.
	// Visibility is controlled by the presence of the status effect this widget visualizes.
	// This allows us to place the widget where we want it from the editor panel, but only show it automatically when it's required.
	Hide();		
}

// StatusEffects

void UStatusEffectInfoWidget::UnBindStatusEffect() {
	// Remove up old status effects component bindings if any.
	if (IsValid(GetStatusEffectsComponent())) {
		GetStatusEffectsComponent()->OnInsertedStatusEffect.RemoveDynamic(this, &UStatusEffectInfoWidget::ActOnInsertedStatusEffect);
		GetStatusEffectsComponent()->OnRemovedStatusEffect.RemoveDynamic(this, &UStatusEffectInfoWidget::ActOnRemovedStatusEffect);
	}
	// Clean up through the method we'd also call when the status effect component removes the effect.
	ActOnRemovedStatusEffect(GetStatusEffectId());
	// Clear any state left to clear referring to the component or status effect.
	StatusEffectsComponent = nullptr;
	StatusEffectId = "";
}

void UStatusEffectInfoWidget::BindStatusEffect(UStatusEffectsComponent* InComponent, const FName& InStatusEffectId) {
	UnBindStatusEffect();

	if (!IsValid(InComponent)) {
		UE_LOG(LogStatusEffectsPlugin, Error, TEXT("InComponent invalid. can't bind to new status effect."));
		return;
	}

	// Set new status effects component and new bindings for it.
	// The status effect component can add or remove the status effect at any time. 
	// This widget should respond to the correct status effect, matching GetStatusEffectId().
	StatusEffectsComponent = InComponent;
	StatusEffectId = InStatusEffectId;
	GetStatusEffectsComponent()->OnInsertedStatusEffect.AddDynamic(this, &UStatusEffectInfoWidget::ActOnInsertedStatusEffect);
	GetStatusEffectsComponent()->OnRemovedStatusEffect.AddDynamic(this, &UStatusEffectInfoWidget::ActOnRemovedStatusEffect);

	// If a relevant stat has been inserted already, set up this widget for it and set up bindings to the stat.
	if (GetStatusEffectsComponent()->GetStatusEffects().Contains(GetStatusEffectId())) {
		ActOnInsertedStatusEffect(GetStatusEffectId());
	}
}

UStatusEffectsComponent* UStatusEffectInfoWidget::GetStatusEffectsComponent() const {
	return StatusEffectsComponent;
}

const FName& UStatusEffectInfoWidget::GetStatusEffectId() const {
	return StatusEffectId;
}

UStatusEffect* UStatusEffectInfoWidget::GetStatusEffect() const {
	return StatusEffect;
}

// Appearance

void UStatusEffectInfoWidget::Show() {
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UStatusEffectInfoWidget::Hide() {
	SetVisibility(ESlateVisibility::Collapsed);
}

// Delegates

void UStatusEffectInfoWidget::ActOnInsertedStatusEffect(const FName& InStatusEffectId) {
	if (GetStatusEffectId() != InStatusEffectId) {
		// Irrelevant event. 
		// This method was likely called for another effect present on the status effects component.
		return;		
	}
	if (!IsValid(GetStatusEffectsComponent())) {
		UE_LOG(LogStatusEffectsPlugin, Error, TEXT("GetStatusEffectsComponent() invalid. this is not expected. Can't bind to status effect which was just inserted into that component."));
		return;
	}
	if (IsValid(GetStatusEffect())) {
		UE_LOG(LogStatusEffectsPlugin, Error, TEXT("GetStatusEffect is already set. The status effects component is expected to properly remove an effect before inserting one."));
		return;
	}
	StatusEffect = GetStatusEffectsComponent()->GetStatusEffect(GetStatusEffectId());
	if (!IsValid(GetStatusEffect())) {
		UE_LOG(LogStatusEffectsPlugin, Error, TEXT("The status effect reported as inserted, is invalid. This is unexpected."));
		return;
	}	

	GetStatusEffect()->OnStatusDataChanged.AddDynamic(this, &UStatusEffectInfoWidget::ActOnStatusDataChanged);
	
	// Immediately update widget with status effect data, and show it.
	ActOnStatusDataChanged(GetStatusEffect());
	Show();
}

void UStatusEffectInfoWidget::ActOnRemovedStatusEffect(const FName& InStatusEffectId) {
	if (GetStatusEffectId() != InStatusEffectId) {
		// Irrelevant event.
		return;		
	}
	
	if (IsValid(GetStatusEffect())) {
		GetStatusEffect()->OnStatusDataChanged.RemoveDynamic(this, &UStatusEffectInfoWidget::ActOnStatusDataChanged);
	}
	
	// Immediately update widget with nullptr (no status effect data), and hide it.
	StatusEffect = nullptr;
	ActOnStatusDataChanged(GetStatusEffect());
	Hide();		
}

void UStatusEffectInfoWidget::ActOnStatusDataChanged(UStatusEffect* InStatusEffect) {
	UpdateStatusEffectAppearance(InStatusEffect);
}