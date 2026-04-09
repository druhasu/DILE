// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"

#include "DLItemFragment_Weapon.generated.h"

class UDLItemType_Ammo;

/*
 * Data of a Weapon item
 */
USTRUCT()
struct FDLItemFragment_Weapon : public FDLItemFragment
{
    GENERATED_BODY()

public:
    /* Icon to show in UI*/
    UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "UI"))
    TSoftObjectPtr<UTexture2D> Icon;

    /* Type of Ammo used by this weapon */
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UDLItemType_Ammo> AmmoType;
};
