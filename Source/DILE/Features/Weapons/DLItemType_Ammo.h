// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemType.h"
#include "DLItemFragment_Ammo.h"

#include "DLItemType_Ammo.generated.h"

UCLASS()
class UDLItemType_Ammo : public UDLItemType
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Ammo", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Ammo Ammo;

    UPROPERTY(EditDefaultsOnly, Category = "Ammo", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Countable Countable;
};
