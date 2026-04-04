// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Net/Serialization/FastArraySerializer.h"

#include "DLItemCountsCollection.generated.h"

class UDLItemType;
class UDLInventoryServiceImpl;

USTRUCT()
struct FDLItemCountsCollectionEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

public:
    FDLItemCountsCollectionEntry() = default;
    FDLItemCountsCollectionEntry(const UDLItemType* InItemType) : ItemType(InItemType), Count(0) {}

    FString GetDebugString();

public:
    UPROPERTY()
    TObjectPtr<const UDLItemType> ItemType;

    UPROPERTY()
    int32 Count = 0;

    UPROPERTY(NotReplicated)
    int32 PrevCount = 0;
};

USTRUCT()
struct FDLItemCountsCollection : public FFastArraySerializer
{
    GENERATED_BODY()

public:
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FastArrayDeltaSerialize<FDLItemCountsCollectionEntry, FDLItemCountsCollection>(Entries, DeltaParms, *this);
    }

    void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
    void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

public:
    UPROPERTY()
    TArray<FDLItemCountsCollectionEntry> Entries;

    UPROPERTY(NotReplicated)
    TObjectPtr<UDLInventoryServiceImpl> Inventory;
};

template<>
struct TStructOpsTypeTraits<FDLItemCountsCollection> : public TStructOpsTypeTraitsBase2<FDLItemCountsCollection>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};
