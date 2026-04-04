// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Net/Serialization/FastArraySerializer.h"

#include "DLEquipmentCollection.generated.h"

class UDLItemInstance_Equipment;
class UDLEquipmentSlot;
class UDLEquipmentServiceImpl;

USTRUCT()
struct FDLEquipmentCollectionEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()

public:
    FDLEquipmentCollectionEntry() = default;
    FDLEquipmentCollectionEntry(const UDLEquipmentSlot* InSlot) : Slot(InSlot) {}

    FString GetDebugString();

public:
    UPROPERTY()
    TObjectPtr<const UDLEquipmentSlot> Slot;

    UPROPERTY()
    TObjectPtr<UDLItemInstance_Equipment> ItemInstance;

    UPROPERTY(NotReplicated)
    TObjectPtr<UDLItemInstance_Equipment> PrevItemInstance;
};

USTRUCT()
struct FDLEquipmentCollection : public FFastArraySerializer
{
    GENERATED_BODY()

public:
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        return FastArrayDeltaSerialize<FDLEquipmentCollectionEntry, FDLEquipmentCollection>(Entries, DeltaParms, *this);
    }

    void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
    void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

public:
    UPROPERTY()
    TArray<FDLEquipmentCollectionEntry> Entries;

    UPROPERTY(NotReplicated)
    TObjectPtr<UDLEquipmentServiceImpl> EquipmentService;
};

template<>
struct TStructOpsTypeTraits<FDLEquipmentCollection> : public TStructOpsTypeTraitsBase2<FDLEquipmentCollection>
{
    enum
    {
        WithNetDeltaSerializer = true,
    };
};
