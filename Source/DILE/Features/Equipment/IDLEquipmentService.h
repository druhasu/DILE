// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLEquipmentService.generated.h"

class UDLItemInstance_Equipment;
class UDLEquipmentSlot;

UINTERFACE(MinimalApi)
class UDLEquipmentService : public UInterface { GENERATED_BODY() };

/*
 * Allows to equip items
 */
class DILE_API IDLEquipmentService
{
    GENERATED_BODY()

public:
    using FChangedSignature = TMulticastDelegate<void(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)>;

    virtual TArrayView<TObjectPtr<const UDLEquipmentSlot>> GetSlots() const = 0;

    virtual UDLItemInstance_Equipment* GetEquippedItem(const UDLEquipmentSlot* Slot) const = 0;

    virtual void EquipItem(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot) = 0;

    virtual void UnequipItem(UDLItemInstance_Equipment* Item) = 0;

    virtual void UnequipItem(const UDLEquipmentSlot* Slot) = 0;

    virtual FChangedSignature& OnItemEquipped() = 0;

    virtual FChangedSignature& OnItemUnequipped() = 0;
};
