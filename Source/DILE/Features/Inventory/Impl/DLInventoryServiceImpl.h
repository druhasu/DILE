// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/ServiceReplication/DLReplicatedService.h"
#include "Features/Inventory/IDLInventoryService.h"
#include "DLItemCountsCollection.h"
#include "DLItemInstancesCollection.h"

#include "DLInventoryServiceImpl.generated.h"

class IInjector;
class IDLSubObjectReplicationService;
struct FDLItemFragment_Countable;

UCLASS()
class DILE_API UDLInventoryServiceImpl : public UDLReplicatedServiceBase, public IDLInventoryService
{
    GENERATED_BODY()
    friend struct FDLItemInstancesCollection;
    friend struct FDLItemCountsCollection;

public:
    void InitDependencies(
        TScriptInterface<IInjector> InInjector,
        TScriptInterface<IDLSubObjectReplicationService> InSubObjectReplicationService
    );

    // Begin IDLInventoryService
    TConstStridedView<TObjectPtr<UDLItemInstance>> GetAllItems() const override;
    UDLItemInstance* CreateItem(const UDLItemType* ItemType) override;
    void AddItem(UDLItemInstance* Item) override;
    void RemoveItem(UDLItemInstance* Item) override;
    FItemInstanceSignature& OnItemAdded() override { return ItemAddedDelegate; }
    FItemInstanceSignature& OnItemRemoved() override { return ItemRemovedDelegate; }
    int32 GetItemCount(const UDLItemType* ItemType) const override;
    int32 IncreaseItemCount(const UDLItemType* ItemType, int32 Amount) override;
    int32 DecreaseItemCount(const UDLItemType* ItemType, int32 Amount) override;
    FItemCountSignature& OnItemCountChanged() override { return ItemCountChangedDelegate; }
    // End IDLInventoryService

private:
    void AddItemInternal(UDLItemInstance* Item);
    void RemoveItemInternal(int32 Index);
    int32 ModifyCountInternal(const UDLItemType* ItemType, const FDLItemFragment_Countable* CountableFragment, int32 Delta);

private:
    UPROPERTY()
    TScriptInterface<IInjector> Injector;

    UPROPERTY()
    TScriptInterface<IDLSubObjectReplicationService> SubObjectReplicationService;

    UPROPERTY(Replicated)
    FDLItemInstancesCollection ItemInstances;

    UPROPERTY(Replicated)
    FDLItemCountsCollection ItemCounts;

    FItemInstanceSignature ItemAddedDelegate;
    FItemInstanceSignature ItemRemovedDelegate;
    FItemCountSignature ItemCountChangedDelegate;
};
