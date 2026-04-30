// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLEquipmentAbilityServiceImpl.generated.h"

class IDLEquipmentService;
class IDLAbilityService;
class UDLItemInstance_Equipment;
class UDLEquipmentSlot;

UCLASS()
class DILE_API UDLEquipmentAbilityServiceImpl : public UObject
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLEquipmentService> InEquipmentService,
        TScriptInterface<IDLAbilityService> InAbilityService
    );

private:
    void OnItemEquipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot);
    void OnItemUnequipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot);

private:
    UPROPERTY()
    TScriptInterface<IDLEquipmentService> EquipmentService;

    UPROPERTY()
    TScriptInterface<IDLAbilityService> AbilityService;
};
