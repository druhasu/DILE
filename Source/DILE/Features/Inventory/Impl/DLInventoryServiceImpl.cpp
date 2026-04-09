// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInventoryServiceImpl.h"
#include "Features/Inventory/DLItemInstance.h"
#include "Systems/ServiceReplication/IDLSubObjectReplicationService.h"
#include "Utils/DLEnsure.h"

#include "DI/IInjector.h"
#include "Net/UnrealNetwork.h"

void UDLInventoryServiceImpl::InitDependencies(
    TScriptInterface<IInjector> InInjector,
    TScriptInterface<IDLSubObjectReplicationService> InSubObjectReplicationService
)
{
    Injector = InInjector;
    SubObjectReplicationService = InSubObjectReplicationService;

    ItemInstances.Inventory = this;
    ItemCounts.Inventory = this;
}

void UDLInventoryServiceImpl::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ThisClass, ItemInstances, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(ThisClass, ItemCounts, COND_OwnerOnly);
}

TConstStridedView<TObjectPtr<UDLItemInstance>> UDLInventoryServiceImpl::GetAllItems() const
{
    return MakeConstStridedView(ItemInstances.Entries, &FDLItemInstancesCollectionEntry::ItemInstance);
}

UDLItemInstance* UDLInventoryServiceImpl::CreateItem(const UDLItemType* ItemType)
{
    DL_ENSURE_RETURN(HasAuthority(), nullptr);
    DL_ENSURE_RETURN(ItemType != nullptr, nullptr);

    // must be instanced
    const FDLItemFragment_Instanced* Instanced = ItemType->FindFragment<FDLItemFragment_Instanced>();
    DL_ENSURE_RETURN(Instanced != nullptr, nullptr);
    DL_ENSURE_RETURN(!Instanced->InstanceClass.IsNull(), nullptr);
    DL_ENSURE_RETURN(Instanced->InstanceClass.Get() != nullptr, nullptr); // here class must already be loaded by caller

    UDLItemInstance* Result = UDLItemInstance::Create(SubObjectReplicationService->GetOuterForSubObjects(), Instanced->InstanceClass.Get(), ItemType);
    Injector->Inject(Result);
    
    AddItemInternal(Result);

    return Result;
}

void UDLInventoryServiceImpl::AddItem(UDLItemInstance* Item)
{
    DL_ENSURE_RETURN(HasAuthority());
    DL_ENSURE_RETURN(Item != nullptr);

    TScriptInterface<IDLInventoryService> ItemInventory = Item->GetInventory();
    if (ItemInventory == this)
        return;

    // this means Item is inside some other inventory
    // it must be removed from it before adding
    DL_ENSURE_RETURN(ItemInventory == nullptr);

    UObject* RequiredOuter = SubObjectReplicationService->GetOuterForSubObjects();
    if (Item->GetOuter() != RequiredOuter)
    {
        Item->Rename(nullptr, RequiredOuter);
        Injector->Inject(Item);
    }

    AddItemInternal(Item);
}

void UDLInventoryServiceImpl::RemoveItem(UDLItemInstance* Item)
{
    DL_ENSURE_RETURN(HasAuthority());
    DL_ENSURE_RETURN(Item != nullptr);

    TScriptInterface<IDLInventoryService> ItemInventory = Item->GetInventory();
    if (ItemInventory != this)
        return;

    const int32 Index = ItemInstances.Entries.IndexOfByPredicate([&](const FDLItemInstancesCollectionEntry& Entry)
    {
        return Entry.ItemInstance == Item;
    });

    DL_ENSURE_RETURN(Index != INDEX_NONE);

    RemoveItemInternal(Index);
}

int32 UDLInventoryServiceImpl::GetItemCount(const UDLItemType* ItemType) const
{
    DL_ENSURE_RETURN(ItemType != nullptr, 0);

    const FDLItemCountsCollectionEntry* FoundEntry = ItemCounts.Entries.FindByPredicate([&](const FDLItemCountsCollectionEntry& Entry)
    {
        return Entry.ItemType == ItemType;
    });

    return FoundEntry != nullptr ? FoundEntry->Count : 0;
}

int32 UDLInventoryServiceImpl::IncreaseItemCount(const UDLItemType* ItemType, int32 Amount)
{
    DL_ENSURE_RETURN(HasAuthority(), 0);
    DL_ENSURE_RETURN(ItemType != nullptr, 0);
    DL_ENSURE_RETURN(Amount > 0, GetItemCount(ItemType));

    // must be countable
    const FDLItemFragment_Countable* CountableFragment = ItemType->FindFragment<FDLItemFragment_Countable>();
    DL_ENSURE_RETURN(CountableFragment != nullptr, 0); 

    return ModifyCountInternal(ItemType, CountableFragment, Amount);
}

int32 UDLInventoryServiceImpl::DecreaseItemCount(const UDLItemType* ItemType, int32 Amount)
{
    DL_ENSURE_RETURN(HasAuthority(), 0);
    DL_ENSURE_RETURN(ItemType != nullptr, 0);
    DL_ENSURE_RETURN(Amount > 0, GetItemCount(ItemType));

    // must be countable
    const FDLItemFragment_Countable* CountableFragment = ItemType->FindFragment<FDLItemFragment_Countable>();
    DL_ENSURE_RETURN(CountableFragment != nullptr, 0);

    return ModifyCountInternal(ItemType, CountableFragment, -Amount);
}

void UDLInventoryServiceImpl::AddItemInternal(UDLItemInstance* Item)
{
    DL_ENSURE_RETURN(HasAuthority());

    Item->NotifyAddedToInventory(this);

    SubObjectReplicationService->AddSubObject(Item, COND_OwnerOnly);

    FDLItemInstancesCollectionEntry& NewEntry = ItemInstances.Entries.Emplace_GetRef(Item);
    ItemInstances.MarkItemDirty(NewEntry);

    ItemAddedDelegate.Broadcast(Item);
}

void UDLInventoryServiceImpl::RemoveItemInternal(int32 Index)
{
    DL_ENSURE_RETURN(HasAuthority());

    UDLItemInstance* Item = ItemInstances.Entries[Index].ItemInstance;
    Item->NotifyRemovedFromInventory(this);

    SubObjectReplicationService->RemoveSubObject(Item);

    ItemInstances.Entries.RemoveAt(Index);
    ItemInstances.MarkArrayDirty();

    ItemRemovedDelegate.Broadcast(Item);
}

int32 UDLInventoryServiceImpl::ModifyCountInternal(const UDLItemType* ItemType, const FDLItemFragment_Countable* CountableFragment, int32 Delta)
{
    DL_ENSURE_RETURN(HasAuthority(), 0);

    FDLItemCountsCollectionEntry* FoundEntry = ItemCounts.Entries.FindByPredicate([&](const FDLItemCountsCollectionEntry& Entry)
    {
        return Entry.ItemType == ItemType;
    });

    if (FoundEntry == nullptr)
    {
        FoundEntry = &ItemCounts.Entries.Emplace_GetRef(ItemType);
    }

    const int32 OldCount = FoundEntry->Count;
    const int32 NewCount = FMath::Max(0, OldCount + Delta);
    FoundEntry->Count = NewCount;

    ItemCounts.MarkItemDirty(*FoundEntry);

    ItemCountChangedDelegate.Broadcast(ItemType, OldCount, NewCount);

    return NewCount;
}
