// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/WidgetSlots/IDLWidgetSlotsService.h"
#include "Systems/Lifecycle/IDLShutdownListener.h"

#include "DLWidgetSlotsServiceImpl.generated.h"

class IDLObjectTrackerService;
class IDLViewportWidgetService;
class UDLSingleWidgetSlot;

UCLASS()
class DILE_API UDLWidgetSlotsServiceImpl : public UObject, public IDLWidgetSlotsService, public IDLShutdownListener
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLObjectTrackerService> InObjectTrackerService,
        TScriptInterface<IDLViewportWidgetService> InViewportWidgetService
    );

    // Begin IDLWidgetSlotsService
    TScriptInterface<IDLWidgetSlot> GetSlot(FGameplayTag SlotTag) const override;
    TDLTask<TScriptInterface<IDLWidgetSlot>> WaitForSlot(FGameplayTag SlotTag) override { return WaitForSlotImpl(SlotTag); }
    // End IDLWidgetSlotsService

    // Begin IDLShutdownListener
    void OnShutdown() override;
    // End IDLShutdownListener

private:
    TDLTask<TScriptInterface<IDLWidgetSlot>> WaitForSlotImpl(FGameplayTag SlotTag);

    void OnSlotAdded(TScriptInterface<IDLWidgetSlot> Slot);
    void OnSlotRemoved(TScriptInterface<IDLWidgetSlot> Slot);

private:
    UPROPERTY()
    TScriptInterface<IDLObjectTrackerService> ObjectTrackerService;

    UPROPERTY()
    TScriptInterface<IDLViewportWidgetService> ViewportWidgetService;

    UPROPERTY()
    TObjectPtr<UDLSingleWidgetSlot> RootSlot;

    UPROPERTY()
    TMap<FGameplayTag, TScriptInterface<IDLWidgetSlot>> Slots;
    TMap<FGameplayTag, TUniquePtr<UE5Coro::FAwaitableEvent>> PendingSlots;
};
