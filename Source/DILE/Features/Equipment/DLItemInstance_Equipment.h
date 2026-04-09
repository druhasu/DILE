// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemInstance.h"

#include "DLItemInstance_Equipment.generated.h"

class UDLEquipmentSlot;

/*
 * Instance of Equipment item
 */
UCLASS(Abstract)
class UDLItemInstance_Equipment : public UDLItemInstance
{
    GENERATED_BODY()

public:
    /* Whether this instance can be equipped into given slot */
    bool CanEquipInto(const UDLEquipmentSlot* Slot) const;

    /* Called when instance is equipped */
    virtual void NotifyEquipped() {}

    /* Called when instance is unequipped */
    virtual void NotifyUnequipped() {}
};
