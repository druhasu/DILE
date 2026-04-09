// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"

#include "DLItemFragment_Ammo.generated.h"

/*
 * Data of an Ammo item
 */
USTRUCT()
struct FDLItemFragment_Ammo : public FDLItemFragment
{
    GENERATED_BODY()

public:
    /* Icon to show in UI */
    UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "UI"))
    TSoftObjectPtr<UTexture2D> Icon;
};
