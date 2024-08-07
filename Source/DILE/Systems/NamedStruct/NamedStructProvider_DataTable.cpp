// Copyright Andrei Sudarikov. All Rights Reserved.

#include "NamedStructProvider_DataTable.h"
#include "Engine/DataTable.h"

void FNamedStructProvider_DataTable::GetAssetsBaseClassPaths(TArray<FTopLevelAssetPath>& OutClassPaths) const
{
    OutClassPaths.Emplace(UDataTable::StaticClass());
}

FTopLevelAssetPath FNamedStructProvider_DataTable::GetDataStructType(const FAssetData& AssetData) const
{
    static const FName RowStructureTag = "RowStructure";

    if (AssetData.AssetClassPath != FTopLevelAssetPath(UDataTable::StaticClass()))
    {
        // ignore everything, except DataTables
        return {};
    }

    FString RowStructName = AssetData.GetTagValueRef<FString>(RowStructureTag);

    // FTopLevelAssetPath will ensure if path is not properly formatted. Check if it is actually good
    if (!RowStructName.IsEmpty() && (RowStructName.Contains(TEXT("/"), 1) || RowStructName.Contains(TEXT("\'"), 1)))
    {
        return FTopLevelAssetPath(RowStructName);
    }

    // DataTables don't report their Asset Registry tags in packaged Game after being loaded
    if (AssetData.IsAssetLoaded())
    {
        // if it is already loaded we can just ask UDataTable directly
        auto* DataTable = Cast<UDataTable>(AssetData.GetAsset());
        if (ensureMsgf(DataTable != nullptr, TEXT("Cannot get DataTable from Asset '%s'"), *AssetData.ToSoftObjectPath().ToString()))
        {
            // Make sure UDataTable object is fully loaded, because this call may be made during loading
            if (DataTable->HasAnyFlags(EObjectFlags::RF_NeedLoad))
            {
                auto* Linker = DataTable->GetLinker();
                Linker->Preload(DataTable);
            }

            return FTopLevelAssetPath(DataTable->GetRowStruct());
        }
    }

    return {};
}

void FNamedStructProvider_DataTable::GetValuesFromAsset(UObject* Asset, TFunctionRef<void(UScriptStruct*, FName, const void*)> ResultCallback) const
{
    if (UDataTable* DataTable = Cast<UDataTable>(Asset))
    {
        for (auto [Key, Value] : DataTable->GetRowMap())
        {
            ResultCallback(const_cast<UScriptStruct*>(DataTable->GetRowStruct()), Key, Value);
        }

        if (!DataTable->OnDataTableChanged().IsBoundToObject(this))
        {
            DataTable->OnDataTableChanged().AddSP(const_cast<FNamedStructProvider_DataTable*>(this), &FNamedStructProvider_DataTable::RequestReload);
        }
    }
}

void FNamedStructProvider_DataTable::RequestReload()
{
    OnReloadRequested.Broadcast();
}
