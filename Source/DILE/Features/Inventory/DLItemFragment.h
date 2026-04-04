// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLItemFragment.generated.h"

class UDLItemInstance;

/*
 * Base struct for item type fragments
 */
USTRUCT(BlueprintType)
struct FDLItemFragment
{
    GENERATED_BODY()
};

/*
 * Common fragment for all item types
 */
USTRUCT()
struct FDLItemFragment_Common : public FDLItemFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    FText Name;
};

/*
 * Fragment for items that can be instanced and have its own state
 */
USTRUCT()
struct FDLItemFragment_Instanced : public FDLItemFragment
{
    GENERATED_BODY()

public:
    /* Class of the instance to use */
    UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Gameplay"))
    TSoftClassPtr<UDLItemInstance> InstanceClass;
};

/*
 * Fragment for items that can be counted in the inventory
 */
USTRUCT()
struct FDLItemFragment_Countable : public FDLItemFragment
{
    GENERATED_BODY()

public:
    /* Maximum count of items that can be in the inventory. Use -1 for unlimited */
    UPROPERTY(EditDefaultsOnly)
    int32 MaxCount = -1;
};
