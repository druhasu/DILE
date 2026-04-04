// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Inventory/DLItemFragment.h"

#include "DLItemFragment_Ammo.generated.h"

USTRUCT()
struct FDLItemFragment_Ammo : public FDLItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "UI"))
    TSoftObjectPtr<UTexture2D> Icon;
};
