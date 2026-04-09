// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemType.h"
#include "DLItemFragment_Equipment.h"

#include "DLItemType_Equipment.generated.h"

/*
 * Equipment Item type
 */
UCLASS(Abstract)
class UDLItemType_Equipment : public UDLItemType
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Equipment", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Equipment Equipment;
};
