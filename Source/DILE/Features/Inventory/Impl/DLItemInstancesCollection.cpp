// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLItemInstancesCollection.h"
#include "DLInventoryServiceImpl.h"
#include "Features/Inventory/DLItemInstance.h"
#include "Utils/DLLog.h"

FString FDLItemInstancesCollectionEntry::GetDebugString()
{
    return ItemInstance != nullptr ? ItemInstance->GetClass()->GetName() : TEXT("<None>");
}

bool FDLItemInstancesCollection::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
    return FastArrayDeltaSerialize<FDLItemInstancesCollectionEntry, FDLItemInstancesCollection>(Entries, DeltaParms, *this);
}

void FDLItemInstancesCollection::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
    for (int32 Index : RemovedIndices)
    {
        UDLItemInstance* Instance = Entries[Index].ItemInstance;

        if (Instance != nullptr)
        {
            Inventory->ItemRemovedDelegate.Broadcast(Instance);
            DL_LOG(Verbose, TEXT("ItemInstance '%s' removed"), *Instance->GetName());
        }
    }
}

void FDLItemInstancesCollection::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    for (int32 Index : AddedIndices)
    {
        UDLItemInstance* Instance = Entries[Index].ItemInstance;

        if (Instance != nullptr)
        {
            Inventory->ItemAddedDelegate.Broadcast(Instance);
            DL_LOG(Verbose, TEXT("ItemInstance '%s' added"), *Instance->GetName());
        }
    }
}

void FDLItemInstancesCollection::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
    for (int32 Index : ChangedIndices)
    {
        UDLItemInstance* Instance = Entries[Index].ItemInstance;
        TObjectPtr<UDLItemInstance>& PrevInstance = Entries[Index].PrevItemInstance;

        if (PrevInstance != nullptr)
        {
            Inventory->ItemRemovedDelegate.Broadcast(PrevInstance);
            DL_LOG(Verbose, TEXT("ItemInstance '%s' removed"), *PrevInstance->GetName());
        }

        if (Instance != nullptr)
        {
            Inventory->ItemAddedDelegate.Broadcast(Instance);
            DL_LOG(Verbose, TEXT("ItemInstance '%s' added"), *Instance->GetName());
        }

        PrevInstance = Instance;
    }
}
