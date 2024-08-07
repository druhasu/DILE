// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/NameTypes.h"

class INamedStructProvider;
class UScriptStruct;
struct FStreamableHandle;

class DILE_API FNamedStructRegistry : public TSharedFromThis<FNamedStructRegistry>
{
public:
    using ThisClass = FNamedStructRegistry;
    using FStructGetter = UScriptStruct * (*)();
    using FPossibleNamesGetter = const TArray<FName>& (*)();

    static FNamedStructRegistry& Get();

    /* Registers Named struct */
    template<typename TNameStruct, typename TDataStruct>
    uint8 RegisterNamedStruct();

    /* Registers Named struct required functions */
    uint8 RegisterNamedStruct(FStructGetter NameStructGetter, FStructGetter DataStructGetter);

    /* Returns list of possible values of a Named Struct by its struct object. Returns empty list if struct is not registered and ensures */
    TArray<FName> GetPossibleNames(UScriptStruct* NameStruct);

    /* Returns list of all registered Name structs */
    TArray<UScriptStruct*> GetAllNameStructs();

    /* Returns asset dependencies of passed in NameStruct and Name */
    void GetPreloadDependencies(UScriptStruct* NameStruct, FName Name, TArray<UObject*>& OutDeps);

    /* Checks whether given struct is a Name struct */
    bool IsNameStruct(UScriptStruct* NameStruct);

private:
    /* Contains raw registration data that is not processed */
    struct FUnprocessedEntry
    {
        FStructGetter NameStructGetter;
        FStructGetter DataStructGetter;
    };

    struct FValueEntry
    {
        FName Name;
        const void* ValuePtr;
        TObjectPtr<UObject> ContainingAsset;
    };

    struct FStructEntry
    {
        UScriptStruct* NameStruct;
        UScriptStruct* DataStruct;
        TArray<FValueEntry> Values;
    };

private:
    /* Called at Startup. Initializes the system */
    void Startup();

    TArray<FUnprocessedEntry>& GetUnprocessedEntries();

    void ProcessPendingRegistrations();
    void StartAssetsLoading();
    void ReloadAssets();
    void EnsureLoaded();

    void OnModulesChanged(FName InModule, EModuleChangeReason InReason);
    void OnAssetsLoaded();

    void OnAssetAdded(const FAssetData& AssetData);
    void OnAssetRemoved(const FAssetData& AssetData);

    bool IsRelevantAsset(const FAssetData& AssetData) const;

private:
    TArray<TSharedRef<INamedStructProvider>> Providers;
    TSharedPtr<FStreamableHandle> LoadHandle;

    TArray<FStructEntry> StructData;
};

template<typename TNameStruct, typename TDataStruct>
inline uint8 FNamedStructRegistry::RegisterNamedStruct()
{
    return RegisterNamedStruct(&TNameStruct::StaticStruct, TDataStruct::StaticStruct);
}
