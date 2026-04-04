// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLItemInstance_Equipment.h"
#include "DLEquipmentSlot.h"
#include "DLItemFragment_Equipment.h"
#include "Utils/DLEnsure.h"

bool UDLItemInstance_Equipment::CanEquipInto(const UDLEquipmentSlot* Slot) const
{
    DL_ENSURE_RETURN(Slot != nullptr, false);

    const FDLItemFragment_Equipment* Fragment = FindFragment<FDLItemFragment_Equipment>();
    DL_ENSURE_RETURN(Fragment != nullptr, false);

    return Slot->EquipmentSlotTag.MatchesTag(Fragment->EquipmentSlotTag);
}
