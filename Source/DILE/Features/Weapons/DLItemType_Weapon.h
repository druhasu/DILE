// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Equipment/DLItemType_Equipment.h"
#include "DLItemInstance_Weapon.h"
#include "DLItemFragment_Weapon.h"

#include "DLItemType_Weapon.generated.h"

UCLASS()
class UDLItemType_Weapon : public UDLItemType_Equipment
{
    GENERATED_BODY()

public:
    using FInstanceType = UDLItemInstance_Weapon;

    UDLItemType_Weapon()
    {
        Instanced.InstanceClass = FInstanceType::StaticClass();
    }

public:
    UPROPERTY(EditAnywhere, Category = "Weapon", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Weapon Weapon;

    UPROPERTY(EditAnywhere, Category = "Weapon", meta = (ShowOnlyInnerProperties))
    FDLItemFragment_Instanced Instanced;
};
