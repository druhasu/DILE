// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "AssetRegistry/AssetData.h"
#include "UObject/TopLevelAssetPath.h"

class INamedStructProvider
{
public:
    virtual ~INamedStructProvider() = default;

    /* Returns which classes of assets this provider handles */
    virtual void GetAssetsBaseClassPaths(TArray<FTopLevelAssetPath>& OutClassPaths) const = 0;

    /* Returns StructData type that is contained in passed in Asset. Does not load the Asset*/
    virtual FTopLevelAssetPath GetDataStructType(const FAssetData& AssetData) const = 0;

    /* Returns Named Struct data from passed in asset */
    virtual void GetValuesFromAsset(UObject* Asset, TFunctionRef<void(UScriptStruct*, FName, const void*)> ResultCallback) const = 0;

    /* Called when Provider requests reload of assets */
    FSimpleMulticastDelegate OnReloadRequested;
};
