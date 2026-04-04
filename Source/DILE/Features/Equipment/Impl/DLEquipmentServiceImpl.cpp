// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLEquipmentServiceImpl.h"
#include "Features/Inventory/IDLInventoryService.h"
#include "Features/Equipment/DLItemInstance_Equipment.h"
#include "Utils/DLEnsure.h"

#include "Net/UnrealNetwork.h"

void UDLEquipmentServiceImpl::InitDependencies(TScriptInterface<IDLInventoryService> InInventoryService)
{
    InventoryService = InInventoryService;

    Equipment.EquipmentService = this;

    Equipment.Entries.Reserve(Slots.Num());
    for (const UDLEquipmentSlot* Slot : Slots)
    {
        Equipment.Entries.Emplace(Slot);
        Equipment.MarkItemDirty(Equipment.Entries.Last());
    }
}

void UDLEquipmentServiceImpl::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, Equipment);
}

UDLItemInstance_Equipment* UDLEquipmentServiceImpl::GetEquippedItem(const UDLEquipmentSlot* Slot) const
{
    DL_ENSURE_RETURN(Slot != nullptr, nullptr);

    const FDLEquipmentCollectionEntry* FoundEntry = Equipment.Entries.FindByPredicate([&](const FDLEquipmentCollectionEntry& Entry)
    {
        return Entry.Slot == Slot;
    });

    return FoundEntry != nullptr ? FoundEntry->ItemInstance.Get() : nullptr;
}

void UDLEquipmentServiceImpl::EquipItem(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)
{
    DL_ENSURE_RETURN(Item != nullptr);
    DL_ENSURE_RETURN(Slot != nullptr);
    DL_ENSURE_RETURN(Item->GetInventory() == InventoryService);

    if (!Item->CanEquipInto(Slot))
        return;

    Server_EquipItem(Item, Slot);
}

void UDLEquipmentServiceImpl::UnequipItem(UDLItemInstance_Equipment* Item)
{
    DL_ENSURE_RETURN(Item != nullptr);
    DL_ENSURE_RETURN(Item->GetInventory() == InventoryService);

    Server_UnequipItem(Item);
}

void UDLEquipmentServiceImpl::UnequipItem(const UDLEquipmentSlot* Slot)
{
    DL_ENSURE_RETURN(Slot != nullptr);

    Server_UnequipItemFromSlot(Slot);
}

void UDLEquipmentServiceImpl::Server_EquipItem_Implementation(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)
{
    DL_ENSURE_RETURN(Item != nullptr);
    DL_ENSURE_RETURN(Slot != nullptr);
    DL_ENSURE_RETURN(Item->GetInventory() == InventoryService);

    FDLEquipmentCollectionEntry* FoundEntry = Equipment.Entries.FindByPredicate([&](const FDLEquipmentCollectionEntry& Entry)
    {
        return Entry.Slot == Slot;
    });

    if (FoundEntry != nullptr)
        EquipInternal(Item, *FoundEntry);
}

void UDLEquipmentServiceImpl::Server_UnequipItem_Implementation(UDLItemInstance_Equipment* Item)
{
    DL_ENSURE_RETURN(Item != nullptr);
    DL_ENSURE_RETURN(Item->GetInventory() == InventoryService);

    FDLEquipmentCollectionEntry* FoundEntry = Equipment.Entries.FindByPredicate([&](const FDLEquipmentCollectionEntry& Entry)
    {
        return Entry.ItemInstance == Item;
    });

    if (FoundEntry != nullptr)
        UnequipInternal(*FoundEntry);
}

void UDLEquipmentServiceImpl::Server_UnequipItemFromSlot_Implementation(const UDLEquipmentSlot* Slot)
{
    DL_ENSURE_RETURN(Slot != nullptr);

    FDLEquipmentCollectionEntry* FoundEntry = Equipment.Entries.FindByPredicate([&](const FDLEquipmentCollectionEntry& Entry)
    {
        return Entry.Slot == Slot;
    });

    if (FoundEntry != nullptr)
        UnequipInternal(*FoundEntry);
}

void UDLEquipmentServiceImpl::EquipInternal(UDLItemInstance_Equipment* Item, FDLEquipmentCollectionEntry& Entry)
{
    DL_ENSURE_RETURN(HasAuthority());

    if (Entry.ItemInstance == Item)
        return;

    if (Entry.ItemInstance != nullptr)
        UnequipInternal(Entry);

    Item->NotifyEquipped();

    Entry.ItemInstance = Item;
    Equipment.MarkItemDirty(Entry);

    ItemEquippedDelegate.Broadcast(Item, Entry.Slot);
}

void UDLEquipmentServiceImpl::UnequipInternal(FDLEquipmentCollectionEntry& Entry)
{
    DL_ENSURE_RETURN(HasAuthority());

    if (Entry.ItemInstance == nullptr)
        return;

    UDLItemInstance_Equipment* Item = Entry.ItemInstance;
    Item->NotifyUnequipped();

    Entry.ItemInstance = nullptr;
    Equipment.MarkItemDirty(Entry);

    ItemUnequippedDelegate.Broadcast(Item, Entry.Slot);
}
