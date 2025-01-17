// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "INamedStructProvider.h"

class FNamedStructProvider_DataAsset : public INamedStructProvider
{
public:
    void GetAssetsBaseClassPaths(TArray<FTopLevelAssetPath>& OutClassPaths) const override;
    FTopLevelAssetPath GetDataStructType(const FAssetData& AssetData) const override;
    void GetValuesFromAsset(UObject* Asset, TFunctionRef<void(UScriptStruct*, FName, const void*)> ResultCallback) const override;
};
