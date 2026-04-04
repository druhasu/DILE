// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLItemInstancesCollection.h"
#include "DLInventoryServiceImpl.h"
#include "Features/Inventory/DLItemInstance.h"

FString FDLItemInstancesCollectionEntry::GetDebugString()
{
    return ItemInstance != nullptr ? ItemInstance->GetClass()->GetName() : TEXT("<None>");
}

void FDLItemInstancesCollection::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
    for (int32 Index : RemovedIndices)
    {
        UDLItemInstance* Instance = Entries[Index].ItemInstance;

        if (Instance != nullptr)
            Inventory->ItemRemovedDelegate.Broadcast(Instance);
    }
}

void FDLItemInstancesCollection::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    for (int32 Index : AddedIndices)
    {
        UDLItemInstance* Instance = Entries[Index].ItemInstance;

        if (Instance != nullptr)
            Inventory->ItemAddedDelegate.Broadcast(Instance);
    }
}

void FDLItemInstancesCollection::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
    for (int32 Index : ChangedIndices)
    {
        UDLItemInstance* Instance = Entries[Index].ItemInstance;
        TObjectPtr<UDLItemInstance>& PrevInstance = Entries[Index].PrevItemInstance;

        if (PrevInstance != nullptr)
            Inventory->ItemRemovedDelegate.Broadcast(PrevInstance);

        if (Instance != nullptr)
            Inventory->ItemAddedDelegate.Broadcast(Instance);

        PrevInstance = Instance;
    }
}
