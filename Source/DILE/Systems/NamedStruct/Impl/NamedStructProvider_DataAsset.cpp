// Copyright Andrei Sudarikov. All Rights Reserved.

#include "NamedStructProvider_DataAsset.h"

#include "Systems/NamedStruct/NamedStructDataAsset.h"

void FNamedStructProvider_DataAsset::GetAssetsBaseClassPaths(TArray<FTopLevelAssetPath>& OutClassPaths) const
{
    OutClassPaths.Emplace(UNamedStructDataAsset::StaticClass());
}

FTopLevelAssetPath FNamedStructProvider_DataAsset::GetDataStructType(const FAssetData& AssetData) const
{
    FString DataStructName = AssetData.GetTagValueRef<FString>(UNamedStructDataAsset::DataStructureTag);

    // FTopLevelAssetPath will ensure if path is not properly formatted. Check if it is actually good
    if (!DataStructName.IsEmpty() && (DataStructName.Contains(TEXT("/"), 1) || DataStructName.Contains(TEXT("\'"), 1)))
    {
        return FTopLevelAssetPath(DataStructName);
    }

    // UNamedStructDataAssets don't report their Asset Registry tags in packaged Game after being loaded
    if (AssetData.IsAssetLoaded())
    {
        // if it is already loaded we can just ask UNamedStructDataAsset directly
        auto* DataAsset = Cast<UNamedStructDataAsset>(AssetData.GetAsset());
        if (DataAsset != nullptr)
        {
            // Make sure UNamedStructDataAsset object is fully loaded, because this call may be made during loading
            if (DataAsset->HasAnyFlags(EObjectFlags::RF_NeedLoad))
            {
                auto* Linker = DataAsset->GetLinker();
                Linker->Preload(DataAsset);
            }

            return FTopLevelAssetPath(DataAsset->GetDataStructType());
        }
    }

    return {};
}

void FNamedStructProvider_DataAsset::GetValuesFromAsset(UObject* Asset, TFunctionRef<void(UScriptStruct*, FName, const void*)> ResultCallback) const
{
    if (UNamedStructDataAsset* DataAsset = Cast<UNamedStructDataAsset>(Asset))
    {
        ResultCallback(DataAsset->GetDataStructType(), DataAsset->GetStructName(), DataAsset->GetValuePtr());
    }
}
