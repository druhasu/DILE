// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLWidgetSlotsServiceImpl.h"
#include "DLSingleWidgetSlot.h"
#include "Systems/ObjectTracking/IDLObjectTrackerService.h"
#include "Systems/WidgetSlots/IDLWidgetSlot.h"
#include "Systems/WidgetDisplay/IDLViewportWidgetService.h"
#include "Utils/DLEnsure.h"

#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(Tag_RootWidgetSlot, "WidgetSlot.Root");

void UDLWidgetSlotsServiceImpl::InitDependencies(
    TScriptInterface<IDLObjectTrackerService> InObjectTrackerService,
    TScriptInterface<IDLViewportWidgetService> InViewportWidgetService
)
{
    ObjectTrackerService = InObjectTrackerService;
    ViewportWidgetService = InViewportWidgetService;

    RootSlot = NewObject<UDLSingleWidgetSlot>(GetWorld());
    RootSlot->SetSlotTag(Tag_RootWidgetSlot);
    ViewportWidgetService->AddViewportWidgetContent(RootSlot->TakeWidget());

    ObjectTrackerService->OnRegister<IDLWidgetSlot>(this, &ThisClass::OnSlotAdded);
    ObjectTrackerService->OnUnregister<IDLWidgetSlot>(this, &ThisClass::OnSlotRemoved);
}

TScriptInterface<IDLWidgetSlot> UDLWidgetSlotsServiceImpl::GetSlot(FGameplayTag SlotTag) const
{
    return Slots.FindRef(SlotTag);
}

void UDLWidgetSlotsServiceImpl::OnShutdown()
{
    RootSlot->ReleaseSlateResources(true);
    RootSlot = nullptr;

    Slots.Reset();

    for (auto& Pair : PendingSlots)
    {
        Pair.Value->Trigger();
    }
}

TDLTask<TScriptInterface<IDLWidgetSlot>> UDLWidgetSlotsServiceImpl::WaitForSlotImpl(FGameplayTag SlotTag)
{
    using namespace UE5Coro;

    TScriptInterface<IDLWidgetSlot>* FoundSlot = Slots.Find(SlotTag);
    if (FoundSlot != nullptr)
    {
        co_return *FoundSlot;
    }

    TUniquePtr<FAwaitableEvent>* FoundEvent = PendingSlots.Find(SlotTag);
    if (FoundEvent == nullptr)
    {
        FoundEvent = &PendingSlots.Emplace(SlotTag, MakeUnique<FAwaitableEvent>(EEventMode::ManualReset));
    }

    co_await **FoundEvent;

    FoundSlot = Slots.Find(SlotTag);
    if (FoundSlot == nullptr)
    {
        // if slot is not found, then we are in the middle of destruction - cancel ourselves
        co_await FSelfCancellation();
    }

    co_return *FoundSlot;
}

void UDLWidgetSlotsServiceImpl::OnSlotAdded(TScriptInterface<IDLWidgetSlot> Slot)
{
    using namespace UE5Coro;

    FGameplayTag SlotTag = Slot->GetSlotTag();
    DL_ENSURE_RETURN(!Slots.Contains(SlotTag));

    Slots.Emplace(SlotTag, Slot);
    TUniquePtr<FAwaitableEvent>* FoundEvent = PendingSlots.Find(SlotTag);
    if (FoundEvent != nullptr)
    {
        (*FoundEvent)->Trigger();
        PendingSlots.Remove(SlotTag);
    }
}

void UDLWidgetSlotsServiceImpl::OnSlotRemoved(TScriptInterface<IDLWidgetSlot> Slot)
{
    FGameplayTag SlotTag = Slot->GetSlotTag();
    DL_ENSURE_RETURN(Slots.Remove(SlotTag) == 1);
}
