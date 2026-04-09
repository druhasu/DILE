// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLItemType.h"

#include "DLItemInstance.generated.h"

class IDLInventoryService;

/*
 * Instance of an Item of some ItemType
 */
UCLASS(Abstract, Blueprintable)
class UDLItemInstance : public UObject
{
    GENERATED_BODY()
    friend class UDLItemStatics;

public:
    /* Creates new ItemInstance object of given ItemType */
    template <typename T = UDLItemInstance UE_REQUIRES(TIsDerivedFrom<T, UDLItemInstance>::Value)>
    static T* Create(UObject* Outer, TSubclassOf<UDLItemInstance> InstanceClass, const UDLItemType* ItemType)
    {
        T* Result = NewObject<T>(Outer, InstanceClass);
        Result->ItemType = ItemType;
        return Result;
    }

    virtual bool IsSupportedForNetworking() const override { return true; }

    /* Type of the ItemInstance */
    UFUNCTION(BlueprintCallable)
    const UDLItemType* GetItemType() const { return ItemType; }

    /* Inventory that holds the ItemInstance. May be nullptr if it is not inside any inventory */
    TScriptInterface<IDLInventoryService> GetInventory() const { return Inventory; }

    /* Finds fragment of given type inside the instance or inside its ItemType */
    template <typename T UE_REQUIRES(TIsDerivedFrom<T, FDLItemFragment>::Value)>
    const T* FindFragment() const { return ensure(ItemType != nullptr) ? ItemType->FindFragment<T>() : nullptr; }

    /* Called when ItemInstance is added to an Inventory */
    virtual void NotifyAddedToInventory(TScriptInterface<IDLInventoryService> InInventory) { Inventory = InInventory; }

    /* Called when ItemInstance is removed from an Inventory */
    virtual void NotifyRemovedFromInventory(TScriptInterface<IDLInventoryService> InInventory) { Inventory = nullptr; }

private:
    UPROPERTY(Replicated)
    TObjectPtr<const UDLItemType> ItemType;

    UPROPERTY()
    TScriptInterface<IDLInventoryService> Inventory;
};
