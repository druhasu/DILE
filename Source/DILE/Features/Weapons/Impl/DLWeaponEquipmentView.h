// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Utils/DLWorldObject.h"
#include "Features/Inventory/DLItemFragment.h"

#include "DLWeaponEquipmentView.generated.h"

class IDLEquipmentService;
class IDLWeaponSlotsService;
class UDLItemInstance_Equipment;
class UDLItemInstance_Weapon;
class UDLEquipmentSlot;

/*
 * Visualizes currently equiped Weapon
 */
UCLASS()
class DILE_API UDLWeaponEquipmentView : public UDLWorldObject
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLEquipmentService> InEquipmentService,
        TScriptInterface<IDLWeaponSlotsService> InWeaponSlotsService
    );

protected:
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Weapon Changed"))
    void K2_OnWeaponChanged(UDLItemInstance_Weapon* NewWeapon);

private:
    void OnItemEquipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot);
    void OnItemUnequipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot);

    void OnSelectedSlotChanged(int32 NewSlotIndex);
    void OnWeaponChanged(UDLItemInstance_Weapon* NewWeapon);

private:
    UPROPERTY()
    TScriptInterface<IDLEquipmentService> EquipmentService;

    UPROPERTY()
    TScriptInterface<IDLWeaponSlotsService> WeaponSlotsService;
};
