// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLInventoryService.generated.h"

class UDLItemType;
class UDLItemInstance;

UINTERFACE(MinimalApi)
class UDLInventoryService : public UInterface { GENERATED_BODY() };

/*
 * Provides access to player ingame inventory
 */
class DILE_API IDLInventoryService
{
    GENERATED_BODY()

public:
    using FItemInstanceSignature = TMulticastDelegate<void(UDLItemInstance* Item)>;
    using FItemCountSignature = TMulticastDelegate<void(const UDLItemType* ItemType, int32 OldCount, int32 NewCount)>;

    /* All item instances */
    virtual TConstStridedView<TObjectPtr<UDLItemInstance>> GetAllItems() const = 0;

    /* Creates and adds new item instance */
    virtual UDLItemInstance* CreateItem(const UDLItemType* ItemType) = 0;

    /* Adds existing item instance to the Inventory. This instance must not be a part of any other inventory. Remove it first */
    virtual void AddItem(UDLItemInstance* Item) = 0;

    /* Removes existing item instance from the Inventory */
    virtual void RemoveItem(UDLItemInstance* Item) = 0;

    /* Called when new item instance is added */
    virtual FItemInstanceSignature& OnItemAdded() = 0;

    /* Called when item instance is removed */
    virtual FItemInstanceSignature& OnItemRemoved() = 0;

    /* Returns count of non instanced item */
    virtual int32 GetItemCount(const UDLItemType* ItemType) const = 0;

    /* Increases count of non instanced item. Amount must be greater than 0 */
    virtual int32 IncreaseItemCount(const UDLItemType* ItemType, int32 Amount) = 0;

    /* Decreases count of non instanced item. Amount must be greater than 0 */
    virtual int32 DecreaseItemCount(const UDLItemType* ItemType, int32 Amount) = 0;

    /* Called when count of non instanced item changes */
    virtual FItemCountSignature& OnItemCountChanged() = 0;

    /* Creates and adds new item instance */
    template <typename TItemType UE_REQUIRES(TIsDerivedFrom<TItemType, UDLItemType>::Value)>
    TItemType::FInstanceType* CreateItem(const TItemType* ItemType)
    {
        return Cast<typename TItemType::FInstanceType>(CreateItem((const UDLItemType*)ItemType));
    }
};

