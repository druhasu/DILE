// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/ServiceReplication/DLReplicatedService.h"
#include "Features/Weapons/IDLWeaponSlotsService.h"

#include "DLWeaponSlotsServiceImpl.generated.h"

class IDLEquipmentService;
class UDLItemInstance_Equipment;
class UDLEquipmentSlot;

UCLASS()
class DILE_API UDLWeaponSlotsServiceImpl : public UDLReplicatedServiceBase, public IDLWeaponSlotsService
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IDLEquipmentService> InEquipmentService);

    // Begin IDLWeaponSlotsService
    TConstArrayView<const UDLEquipmentSlot*> GetSlots() const override { return MakeConstArrayView(ObjectPtrDecay(Slots)); }
    int32 GetSelectedSlotIndex() const override { return SelectedSlotIndex; }
    void SetSelectedSlotIndex(int32 SlotIndex) override;
    FChangedSignature& OnSelectedSlotChanged() override { return SelectedSlotChangedDelegate; }
    // End IDLWeaponSlotsService

private:
    UFUNCTION(Server, Reliable)
    void Server_SetSelectedSlot(int32 InSlotIndex);

    UFUNCTION()
    void OnRep_SelectedSlotIndex(int32 PrevIndex);

private:
    UPROPERTY()
    TScriptInterface<IDLEquipmentService> EquipmentService;

    UPROPERTY()
    TArray<TObjectPtr<const UDLEquipmentSlot>> Slots;

    UPROPERTY(ReplicatedUsing = OnRep_SelectedSlotIndex)
    int32 SelectedSlotIndex;

    FChangedSignature SelectedSlotChangedDelegate;
};
