// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "NamedStructDataAsset.generated.h"

/*
 * Base class for NamedStruct assets
 */
UCLASS(Abstract)
class UNamedStructDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    /* Name of contained NamedStruct */
    FName GetStructName() const;

    /* Type of DataStruct */
    UScriptStruct* GetDataStructType() const;

    /* Value of DataStruct */
    const void* GetValuePtr() const;

#if WITH_EDITORONLY_DATA
    virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
#endif
    static inline const FName DataStructureTag = "DataStructure";

private:
    FStructProperty* GetDataProperty() const;

    mutable FName CachedName;
};
