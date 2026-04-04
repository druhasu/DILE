// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLWeaponEquipmentView.h"
#include "Features/Equipment/IDLEquipmentService.h"
#include "Features/Weapons/IDLWeaponSlotsService.h"
#include "Features/Weapons/DLItemType_Weapon.h"
#include "Utils/DLEnsure.h"

void UDLWeaponEquipmentView::InitDependencies(
    TScriptInterface<IDLEquipmentService> InEquipmentService,
    TScriptInterface<IDLWeaponSlotsService> InWeaponSlotsService
)
{
    EquipmentService = InEquipmentService;
    WeaponSlotsService = InWeaponSlotsService;

    EquipmentService->OnItemEquipped().AddUObject(this, &ThisClass::OnItemEquipped);
    EquipmentService->OnItemUnequipped().AddUObject(this, &ThisClass::OnItemUnequipped);
    WeaponSlotsService->OnSelectedSlotChanged().AddUObject(this, &ThisClass::OnSelectedSlotChanged);

    OnSelectedSlotChanged(WeaponSlotsService->GetSelectedSlotIndex());
}

void UDLWeaponEquipmentView::OnItemEquipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)
{
    DL_ENSURE_RETURN(Item != nullptr);
    DL_ENSURE_RETURN(Slot != nullptr);

    if (WeaponSlotsService->GetSelectedSlot() == Slot)
    {
        OnWeaponChanged(Cast<UDLItemInstance_Weapon>(Item));
    }
}

void UDLWeaponEquipmentView::OnItemUnequipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)
{
    DL_ENSURE_RETURN(Item != nullptr);
    DL_ENSURE_RETURN(Slot != nullptr);

    if (WeaponSlotsService->GetSelectedSlot() == Slot)
    {
        OnWeaponChanged(nullptr);
    }
}

void UDLWeaponEquipmentView::OnSelectedSlotChanged(int32 NewSlotIndex)
{
    const UDLEquipmentSlot* Slot = WeaponSlotsService->GetSelectedSlot();

    if (Slot == nullptr)
        return;
    
    OnWeaponChanged(Cast<UDLItemInstance_Weapon>(EquipmentService->GetEquippedItem(Slot)));
}

void UDLWeaponEquipmentView::OnWeaponChanged(UDLItemInstance_Weapon* InWeapon)
{
    K2_OnWeaponChanged(InWeapon);
}
