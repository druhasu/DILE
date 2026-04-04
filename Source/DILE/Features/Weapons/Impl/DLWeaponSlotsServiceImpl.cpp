// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLWeaponSlotsServiceImpl.h"
#include "Features/Equipment/IDLEquipmentService.h"
#include "Features/Equipment/DLEquipmentSlot.h"
#include "Utils/DLEnsure.h"

#include "Net/UnrealNetwork.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tag_EquipmentSlot_Weapon, "EquipmentSlot.Weapon", "Equipment slot for Weapons");

void UDLWeaponSlotsServiceImpl::InitDependencies(TScriptInterface<IDLEquipmentService> InEquipmentService)
{
    EquipmentService = InEquipmentService;

    for (const UDLEquipmentSlot* Slot : EquipmentService->GetSlots())
    {
        if (Slot->EquipmentSlotTag.MatchesTag(Tag_EquipmentSlot_Weapon))
        {
            Slots.Add(Slot);
        }
    }
}

void UDLWeaponSlotsServiceImpl::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, SelectedSlotIndex);
}

void UDLWeaponSlotsServiceImpl::SetSelectedSlotIndex(int32 SlotIndex)
{
    if (SlotIndex == SelectedSlotIndex)
        return;

    Server_SetSelectedSlot(SlotIndex);
}

void UDLWeaponSlotsServiceImpl::Server_SetSelectedSlot_Implementation(int32 InSlotIndex)
{
    DL_ENSURE_RETURN(HasAuthority());
    DL_ENSURE_RETURN(Slots.IsValidIndex(InSlotIndex));

    int32 PrevIndex = SelectedSlotIndex;
    SelectedSlotIndex = InSlotIndex;
    OnRep_SelectedSlotIndex(PrevIndex);
}

void UDLWeaponSlotsServiceImpl::OnRep_SelectedSlotIndex(int32 PrevIndex)
{
    SelectedSlotChangedDelegate.Broadcast(SelectedSlotIndex);
}
