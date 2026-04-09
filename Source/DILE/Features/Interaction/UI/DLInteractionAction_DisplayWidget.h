// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Interaction/DLInteractionAction.h"

#include "GameplayTagContainer.h"

#include "DLInteractionAction_DisplayWidget.generated.h"

class IDLWidgetDisplayService;
class UDLUserWidget;

/*
 * Interaction action to display Widget
 */
UCLASS(Abstract, Blueprintable)
class UDLInteractionAction_DisplayWidget : public UDLInteractionAction
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IDLWidgetDisplayService> InWidgetDisplayService);

protected:
    FDLTask ExecuteAsync(TScriptInterface<IDLInteractable> Interactable) const override;

protected:
    /* Slot to display widget into */
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "WidgetSlot"))
    FGameplayTag WidgetSlot;

    /* Class of a Widget to display */
    UPROPERTY(EditDefaultsOnly)
    TSoftClassPtr<UDLUserWidget> WidgetClass;

private:
    UPROPERTY()
    TScriptInterface<IDLWidgetDisplayService> WidgetDisplayService;
};
