// Copyright Andrei Sudarikov. All Rights Reserved.

#include "NamedStructRegistry.h"

#include "Modules/ModuleManager.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/AssetManager.h"
#include "INamedStructProvider.h"
#include "NamedStructProvider_DataTable.h"
#include "NamedStructProvider_DataAsset.h"

FNamedStructRegistry& FNamedStructRegistry::Get()
{
    static TSharedRef<FNamedStructRegistry> Singleton = []()
    {
        TSharedRef<FNamedStructRegistry> Registry = MakeShared<FNamedStructRegistry>();
        Registry->Startup();
        return Registry;
    }();

    return *Singleton;
}

uint8 FNamedStructRegistry::RegisterNamedStruct(FStructGetter NameStructGetter, FStructGetter DataStructGetter)
{
    TArray<FUnprocessedEntry>& UnprocessedEntries = GetUnprocessedEntries();
    UnprocessedEntries.Add({ NameStructGetter, DataStructGetter });

    return 1;
}

TArray<FName> FNamedStructRegistry::GetPossibleNames(UScriptStruct* NameStruct)
{
    EnsureLoaded();

    TArray<FName> Result;

    const FStructEntry* StructEntry = StructData.FindByPredicate([&](const FStructEntry& Entry)
    {
        return Entry.NameStruct == NameStruct;
    });

    if (StructEntry != nullptr)
    {
        Result.Reserve(StructEntry->Values.Num());
        Algo::Transform(StructEntry->Values, Result, [](const FValueEntry& ValueEntry) { return ValueEntry.Name; });
    }

    return Result;
}

TArray<UScriptStruct*> FNamedStructRegistry::GetAllNameStructs()
{
    TArray<UScriptStruct*> Result;
    Algo::Transform(StructData, Result, [](const FStructEntry& Entry) { return Entry.NameStruct; });

    return Result;
}

void FNamedStructRegistry::GetPreloadDependencies(UScriptStruct* NameStruct, FName Name, TArray<UObject*>& OutDeps)
{
    EnsureLoaded();

    const FStructEntry* StructEntry = StructData.FindByPredicate([&](const FStructEntry& Entry)
    {
        return Entry.NameStruct == NameStruct;
    });

    if (ensure(StructEntry != nullptr))
    {
        const FValueEntry* ValueEntry = StructEntry->Values.FindByPredicate([&](const FValueEntry& Entry)
        {
            return Entry.Name == Name;
        });

        if (ensure(ValueEntry != nullptr))
        {
            OutDeps.Add(ValueEntry->ContainingAsset);
        }
    }
}

bool FNamedStructRegistry::IsNameStruct(UScriptStruct* NameStruct)
{
    EnsureLoaded();

    const FStructEntry* StructEntry = StructData.FindByPredicate([&](const FStructEntry& Entry)
    {
        return Entry.NameStruct == NameStruct;
    });

    return StructEntry != nullptr;
}

void FNamedStructRegistry::Startup()
{
    Providers.Add(MakeShared<FNamedStructProvider_DataAsset>());
    Providers.Add(MakeShared<FNamedStructProvider_DataTable>());

    for (TSharedRef<INamedStructProvider>& Provider : Providers)
    {
        Provider->OnReloadRequested.AddSP(this, &ThisClass::ReloadAssets);
    }

    FModuleManager::Get().OnModulesChanged().AddSP(this, &ThisClass::OnModulesChanged);
    ProcessPendingRegistrations();

    if (IAssetRegistry::Get()->IsLoadingAssets())
    {
        IAssetRegistry::Get()->OnFilesLoaded().AddSP(this, &ThisClass::StartAssetsLoading);
    }
    else
    {
        StartAssetsLoading();
    }

    IAssetRegistry::Get()->OnAssetAdded().AddSP(this, &ThisClass::OnAssetAdded);
    IAssetRegistry::Get()->OnAssetRemoved().AddSP(this, &ThisClass::OnAssetRemoved);
}

TArray<FNamedStructRegistry::FUnprocessedEntry>& FNamedStructRegistry::GetUnprocessedEntries()
{
    static TArray<FUnprocessedEntry> Entries;
    return Entries;
}

void FNamedStructRegistry::ProcessPendingRegistrations()
{
    if (GIsInitialLoad)
    {
        // wait until UObject subsystem is loaded
        return;
    }

    TArray<FUnprocessedEntry>& Entries = GetUnprocessedEntries();
    if (Entries.Num() > 0)
    {
        for (const FUnprocessedEntry& Entry : Entries)
        {
            StructData.Add(FStructEntry{ Entry.NameStructGetter(), Entry.DataStructGetter(), {} });
        }

        Entries.Reset();
    }
}

void FNamedStructRegistry::StartAssetsLoading()
{
    FARFilter Filter;
    Filter.bRecursiveClasses = true;

    for (const auto& Provider : Providers)
    {
        Provider->GetAssetsBaseClassPaths(Filter.ClassPaths);
    }

    TArray<FAssetData> Assets;
    TArray<FSoftObjectPath> AssetsToLoad;

    IAssetRegistry::Get()->GetAssets(Filter, Assets);
    for (const FAssetData& AssetData : Assets)
    {
        for (const auto& Provider : Providers)
        {
            FTopLevelAssetPath DataStructType = Provider->GetDataStructType(AssetData);

            const FStructEntry* StructEntry = StructData.FindByPredicate([&](const FStructEntry& Entry)
            {
                return DataStructType == FTopLevelAssetPath(Entry.DataStruct);
            });

            if (StructEntry != nullptr)
            {
                // we know this struct, load asset
                AssetsToLoad.Add(AssetData.ToSoftObjectPath());

                // assume each asset can be handled only by one provider
                break;
            }
        }
    }

    LoadHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AssetsToLoad, FStreamableDelegate::CreateSP(this, &FNamedStructRegistry::OnAssetsLoaded));
}

void FNamedStructRegistry::ReloadAssets()
{
    if (!IAssetRegistry::Get()->IsLoadingAssets())
    {
        StartAssetsLoading();
    }
}

void FNamedStructRegistry::EnsureLoaded()
{
    if (ensure(LoadHandle.IsValid()))
    {
        LoadHandle->WaitUntilComplete();
    }
}

void FNamedStructRegistry::OnModulesChanged(FName InModule, EModuleChangeReason InReason)
{
    if (InReason == EModuleChangeReason::ModuleLoaded)
    {
        ProcessPendingRegistrations();
    }
}

void FNamedStructRegistry::OnAssetsLoaded()
{
    for (FStructEntry& StructEntry : StructData)
    {
        StructEntry.Values.Reset();
    }

    LoadHandle->ForEachLoadedAsset([this](UObject* Asset)
    {
        for (const auto& Provider : Providers)
        {
            Provider->GetValuesFromAsset(Asset, [&](UScriptStruct* DataStruct, FName Name, const void* ValuePtr)
            {
                FStructEntry* StructEntry = StructData.FindByPredicate([&](const FStructEntry& Entry)
                {
                    return DataStruct == Entry.DataStruct;
                });

                if (ensure(StructEntry != nullptr))
                {
                    StructEntry->Values.Add({ Name, ValuePtr, Asset });
                }
            });
        }
    });
}

void FNamedStructRegistry::OnAssetAdded(const FAssetData& AssetData)
{
    if (IsRelevantAsset(AssetData))
    {
        ReloadAssets();
    }
}

void FNamedStructRegistry::OnAssetRemoved(const FAssetData& AssetData)
{
    if (IsRelevantAsset(AssetData))
    {
        ReloadAssets();
    }
}

bool FNamedStructRegistry::IsRelevantAsset(const FAssetData& AssetData) const
{
    for (const auto& Provider : Providers)
    {
        FTopLevelAssetPath DataStructType = Provider->GetDataStructType(AssetData);

        const FStructEntry* StructEntry = StructData.FindByPredicate([&](const FStructEntry& Entry)
        {
            return DataStructType == FTopLevelAssetPath(Entry.DataStruct);
        });

        if (StructEntry != nullptr)
        {
            // assume each asset can be handled only by one provider
            return true;
        }
    }

    return false;
}
