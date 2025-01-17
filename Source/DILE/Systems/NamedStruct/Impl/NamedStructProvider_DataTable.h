// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "INamedStructProvider.h"

class FNamedStructProvider_DataTable : public INamedStructProvider, public TSharedFromThis<FNamedStructProvider_DataTable>
{
public:
    void GetAssetsBaseClassPaths(TArray<FTopLevelAssetPath>& OutClassPaths) const override;
    FTopLevelAssetPath GetDataStructType(const FAssetData& AssetData) const override;
    void GetValuesFromAsset(UObject* Asset, TFunctionRef<void(UScriptStruct* DataStruct, FName Name, const void* ValuePtr)> ResultCallback) const override;

private:
    void RequestReload();
};
