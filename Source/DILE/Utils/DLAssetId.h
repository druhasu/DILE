// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "UObject/PrimaryAssetId.h"

/*
 * Strongly typed wrapper around FPrimaryAssetId
 */
template <typename TItemType>
struct [[nodiscard]] TDLAssetId
{
public:
    TDLAssetId() = default;

    explicit TDLAssetId(const TItemType* ItemType)
        : TDLAssetId(((const UPrimaryDataAsset*)ItemType)->GetPrimaryAssetId())
    {
    }

    explicit TDLAssetId(const FPrimaryAssetId& InAssetId)
        : AssetId(InAssetId)
    {
    }

    explicit TDLAssetId(FPrimaryAssetId&& InAssetId)
        : AssetId(MoveTemp(InAssetId))
    {
    }

    static FPrimaryAssetType GetAssetType()
    {
        return FPrimaryAssetType(TItemType::StaticClass()->GetFName());
    }

    const FPrimaryAssetId& GetAssetId() const
    {
        return AssetId;
    }

    bool IsValid() const
    {
        return AssetId.IsValid();
    }

    bool operator== (const TDLAssetId<TItemType>& Other) const
    {
        return Other.AssetId == AssetId;
    }

    bool operator!= (const TDLAssetId<TItemType>& Other) const
    {
        return Other.AssetId != AssetId;
    }

private:
    FPrimaryAssetId AssetId;
};
