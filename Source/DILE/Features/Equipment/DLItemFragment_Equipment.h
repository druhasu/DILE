// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"
#include "GameplayTagContainer.h"

#include "DLItemFragment_Equipment.generated.h"

class UDLEquipmentSlot;

USTRUCT()
struct FDLItemFragment_Equipment : public FDLItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, meta = (Categories = "EquipmentSlot"))
    FGameplayTag EquipmentSlotTag;
};
