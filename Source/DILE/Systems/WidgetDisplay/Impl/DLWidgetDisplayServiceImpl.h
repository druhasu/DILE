// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/WidgetDisplay/IDLWidgetDisplayService.h"

#include "DLWidgetDisplayServiceImpl.generated.h"

class IDLStreamableService;
class IDLViewFactory;
class IDLWidgetSlotsService;
class IDLPlayerControllerService;
class IDLWidgetSlot;
class UUserWidget;

UCLASS()
class DILE_API UDLWidgetDisplayServiceImpl : public UObject, public IDLWidgetDisplayService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLStreamableService> InStreamableService,
        TScriptInterface<IDLViewFactory> InViewFactory,
        TScriptInterface<IDLWidgetSlotsService> InWidgetSlotsService,
        TScriptInterface<IDLPlayerControllerService> InPlayerControllerService
    );

    // Begin IDLWidgetDisplayService
    TDLTask<UDLUserWidget*> DisplayWidget(FGameplayTag SlotTag, const TSoftClassPtr<UDLUserWidget>& WidgetClass, FDLViewContent Content, bool bWaitForAnimation) override
    {
        return std::move(DisplayWidgetImpl(SlotTag, WidgetClass, MoveTemp(Content), bWaitForAnimation));
    }

    FDLTask CloseWidget(FGameplayTag SlotTag, bool bWaitForAnimation) override
    {
        return CloseWidgetImpl(SlotTag, bWaitForAnimation);
    }
    // End IDLWidgetDisplayService

    static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

private:
    TDLTask<UDLUserWidget*> DisplayWidgetImpl(FGameplayTag SlotTag, const TSoftClassPtr<UDLUserWidget>& WidgetClass, FDLViewContent Content, bool bWaitForAnimation, FForceLatentCoroutine = {});
    FDLTask CloseWidgetImpl(FGameplayTag SlotTag, bool bWaitForAnimation, FForceLatentCoroutine = {});

    void OnWidgetDestructed(UUserWidget* Widget);

    void EnableMouseInput();
    void DisableMouseInput();

private:
    UPROPERTY()
    TScriptInterface<IDLStreamableService> StreamableService;

    UPROPERTY()
    TScriptInterface<IDLViewFactory> ViewFactory;

    UPROPERTY()
    TScriptInterface<IDLWidgetSlotsService> WidgetSlotsService;

    UPROPERTY()
    TScriptInterface<IDLPlayerControllerService> PlayerControllerService;

    TArray<TWeakObjectPtr<UDLUserWidget>> WidgetsWithMouse;

    TArray<FDLViewContent*> ContentToKeep;
};
