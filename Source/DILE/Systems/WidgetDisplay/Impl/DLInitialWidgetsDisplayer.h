// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/Lifecycle/IDLStartupListener.h"

#include "GameplayTagContainer.h"

#include "DLInitialWidgetsDisplayer.generated.h"

class UDLUserWidget;
class IDLWidgetDisplayService;

USTRUCT()
struct FDLInitialHUDWidget
{
    GENERATED_BODY()

public:
    /* Tag of the Slot to add Widget to */
    UPROPERTY(EditAnywhere, meta = (Categories = "WidgetSlot"))
    FGameplayTag Slot;

    /* Class of the Widget to create and add */
    UPROPERTY(EditAnywhere)
    TSoftClassPtr<UDLUserWidget> Class;
};

/*
 * Allows configuring which widgets should be added to slots at startup
 */
UCLASS(Blueprintable)
class DILE_API UDLInitialWidgetDisplayer : public UObject, public IDLStartupListener
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLWidgetDisplayService> InWidgetDisplayService
    );

    void OnStartup() override;

protected:
    UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "Slot"))
    TArray<FDLInitialHUDWidget> InitialWidgets;

private:
    UPROPERTY()
    TScriptInterface<IDLWidgetDisplayService> WidgetDisplayService;
};
