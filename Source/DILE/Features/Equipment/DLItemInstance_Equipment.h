// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemInstance.h"

#include "DLItemInstance_Equipment.generated.h"

class UDLEquipmentSlot;

UCLASS(Abstract)
class UDLItemInstance_Equipment : public UDLItemInstance
{
    GENERATED_BODY()

public:
    bool CanEquipInto(const UDLEquipmentSlot* Slot) const;

    virtual void NotifyEquipped() {}
    virtual void NotifyUnequipped() {}
};
