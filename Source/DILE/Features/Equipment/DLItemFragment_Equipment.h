// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"
#include "GameplayTagContainer.h"

#include "DLItemFragment_Equipment.generated.h"

class UDLEquipmentSlot;

/*
 * Data of Equipment item
 */
USTRUCT()
struct FDLItemFragment_Equipment : public FDLItemFragment
{
    GENERATED_BODY()

public:
    /* Tag of Equipment slot that may contain this equipment item */
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "EquipmentSlot"))
    FGameplayTag EquipmentSlotTag;
};
