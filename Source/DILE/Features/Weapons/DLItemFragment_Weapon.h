// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"

#include "DLItemFragment_Weapon.generated.h"

class UDLItemType_Ammo;

USTRUCT()
struct FDLItemFragment_Weapon : public FDLItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "UI"))
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UDLItemType_Ammo> AmmoType;
};
