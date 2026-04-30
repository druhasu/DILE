// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Net/Serialization/FastArraySerializer.h"

#include "DLItemInstancesCollection.generated.h"

class UDLItemInstance;
class UDLInventoryServiceImpl;

USTRUCT()
struct FDLItemInstancesCollectionEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

public:
    FDLItemInstancesCollectionEntry() = default;
    FDLItemInstancesCollectionEntry(UDLItemInstance* InInstance) : ItemInstance(InInstance) {}

    FString GetDebugString();

public:
    UPROPERTY()
    TObjectPtr<UDLItemInstance> ItemInstance;

    UPROPERTY(NotReplicated)
    TObjectPtr<UDLItemInstance> PrevItemInstance;
};

USTRUCT()
struct FDLItemInstancesCollection : public FFastArraySerializer
{
    GENERATED_BODY()

public:
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);

    void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
    void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
    void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

public:
    UPROPERTY()
    TArray<FDLItemInstancesCollectionEntry> Entries;

    UPROPERTY(NotReplicated)
    TObjectPtr<UDLInventoryServiceImpl> Inventory;
};

template<>
struct TStructOpsTypeTraits<FDLItemInstancesCollection> : public TStructOpsTypeTraitsBase2<FDLItemInstancesCollection>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};
