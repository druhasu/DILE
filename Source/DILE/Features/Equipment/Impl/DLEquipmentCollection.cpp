// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLEquipmentCollection.h"
#include "DLEquipmentServiceImpl.h"
#include "Features/Equipment/DLItemInstance_Equipment.h"
#include "Features/Equipment/DLEquipmentSlot.h"
#include "Utils/DLLog.h"

FString FDLEquipmentCollectionEntry::GetDebugString()
{
    return ItemInstance != nullptr ? ItemInstance->GetClass()->GetName() : TEXT("<None>");
}

void FDLEquipmentCollection::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    for (int32 Index : AddedIndices)
    {
        FDLEquipmentCollectionEntry& Entry = Entries[Index];
        if (Entry.ItemInstance != nullptr)
        {
            EquipmentService->ItemEquippedDelegate.Broadcast(Entry.ItemInstance, Entry.Slot);
        }
        Entry.PrevItemInstance = Entry.ItemInstance;
    }
}

void FDLEquipmentCollection::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
    for (int32 Index : ChangedIndices)
    {
        FDLEquipmentCollectionEntry& Entry = Entries[Index];
        if (Entry.ItemInstance != nullptr)
        {
            EquipmentService->ItemEquippedDelegate.Broadcast(Entry.ItemInstance, Entry.Slot);
            DL_LOG(Verbose, TEXT("Item '{%s}' equipped in slot '{%s}'"), *Entry.ItemInstance->GetName(), *Entry.Slot->GetName());
        }
        else if(Entry.PrevItemInstance != nullptr)
        {
            EquipmentService->ItemUnequippedDelegate.Broadcast(Entry.PrevItemInstance, Entry.Slot);
            DL_LOG(Verbose, TEXT("Item '{%s}' equipped in slot '{%s}'"), *Entry.ItemInstance->GetName(), *Entry.Slot->GetName());
        }

        Entry.PrevItemInstance = Entry.ItemInstance;
    }
}
