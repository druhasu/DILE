// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLItemCountsCollection.h"
#include "DLInventoryServiceImpl.h"
#include "Features/Inventory/DLItemType.h"

FString FDLItemCountsCollectionEntry::GetDebugString()
{
    return FString::Printf(TEXT("%s: %d"), ItemType != nullptr ? *ItemType->GetClass()->GetName() : TEXT("<None>"), Count);
}

void FDLItemCountsCollection::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    for (int32 Index : AddedIndices)
    {
        FDLItemCountsCollectionEntry& Entry = Entries[Index];
        Inventory->ItemCountChangedDelegate.Broadcast(Entry.ItemType, 0, Entry.Count);
        Entry.PrevCount = Entry.Count;
    }
}

void FDLItemCountsCollection::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
    for (int32 Index : ChangedIndices)
    {
        FDLItemCountsCollectionEntry& Entry = Entries[Index];
        if (Entry.Count != Entry.PrevCount)
        {
            Inventory->ItemCountChangedDelegate.Broadcast(Entry.ItemType, Entry.PrevCount, Entry.Count);
            Entry.PrevCount = Entry.Count;
        }
    }
}
