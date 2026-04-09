// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLEquipmentService.generated.h"

class UDLItemInstance_Equipment;
class UDLEquipmentSlot;

UINTERFACE(MinimalApi)
class UDLEquipmentService : public UInterface { GENERATED_BODY() };

/*
 * Allows to equip and unequip items
 */
class DILE_API IDLEquipmentService
{
    GENERATED_BODY()

public:
    using FChangedSignature = TMulticastDelegate<void(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)>;

    /* All equipment slots */
    virtual TArrayView<TObjectPtr<const UDLEquipmentSlot>> GetSlots() const = 0;

    /* Returns instance of Equipment in a given slot */
    virtual UDLItemInstance_Equipment* GetEquippedItem(const UDLEquipmentSlot* Slot) const = 0;

    /* Equips item in given slot. If there is an item in the slot already, it will be unequipped */
    virtual void EquipItem(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot) = 0;

    /* Unequips an item */
    virtual void UnequipItem(UDLItemInstance_Equipment* Item) = 0;

    /* Unequips an item from a given slot */
    virtual void UnequipItem(const UDLEquipmentSlot* Slot) = 0;

    /* Called when item is equipped */
    virtual FChangedSignature& OnItemEquipped() = 0;

    /* Called when item is unequipped */
    virtual FChangedSignature& OnItemUnequipped() = 0;
};
