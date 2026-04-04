// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/ServiceReplication/DLReplicatedService.h"
#include "Features/Equipment/IDLEquipmentService.h"
#include "DLEquipmentCollection.h"

#include "DLEquipmentServiceImpl.generated.h"

class IDLInventoryService;

UCLASS(Blueprintable)
class DILE_API UDLEquipmentServiceImpl : public UDLReplicatedServiceBase, public IDLEquipmentService
{
    GENERATED_BODY()
    friend struct FDLEquipmentCollection;

public:
    void InitDependencies(TScriptInterface<IDLInventoryService> InInventoryService);

    // Begin IDLEquipmentService
    TArrayView<TObjectPtr<const UDLEquipmentSlot>> GetSlots() const override { return MakeArrayView(const_cast<TObjectPtr<const UDLEquipmentSlot>*>(Slots.GetData()), Slots.Num()); }
    UDLItemInstance_Equipment* GetEquippedItem(const UDLEquipmentSlot* Slot) const override;
    void EquipItem(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot) override;
    void UnequipItem(UDLItemInstance_Equipment* Item) override;
    void UnequipItem(const UDLEquipmentSlot* Slot) override;
    FChangedSignature& OnItemEquipped() override { return ItemEquippedDelegate; }
    FChangedSignature& OnItemUnequipped() override { return ItemUnequippedDelegate; }
    // End IDLEquipmentService

protected:
    UPROPERTY(EditDefaultsOnly)
    TArray<TObjectPtr<const UDLEquipmentSlot>> Slots;

private:
    UFUNCTION(Server, Reliable)
    void Server_EquipItem(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot);

    UFUNCTION(Server, Reliable)
    void Server_UnequipItem(UDLItemInstance_Equipment* Item);

    UFUNCTION(Server, Reliable)
    void Server_UnequipItemFromSlot(const UDLEquipmentSlot* Slot);

    void EquipInternal(UDLItemInstance_Equipment* Item, FDLEquipmentCollectionEntry& Entry);
    void UnequipInternal(FDLEquipmentCollectionEntry& Entry);

private:
    UPROPERTY()
    TScriptInterface<IDLInventoryService> InventoryService;

    UPROPERTY(Replicated)
    FDLEquipmentCollection Equipment;

    FChangedSignature ItemEquippedDelegate;
    FChangedSignature ItemUnequippedDelegate;
};
