// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLItemType.h"

#include "DLItemInstance.generated.h"

class IDLInventoryService;

UCLASS(Abstract, Blueprintable)
class UDLItemInstance : public UObject
{
    GENERATED_BODY()
    friend class UDLItemStatics;

public:
    template <typename T = UDLItemInstance UE_REQUIRES(TIsDerivedFrom<T, UDLItemInstance>::Value)>
    static T* Create(UObject* Outer, TSubclassOf<UDLItemInstance> InstanceClass, const UDLItemType* ItemType)
    {
        T* Result = NewObject<T>(Outer, InstanceClass);
        Result->ItemType = ItemType;
        return Result;
    }

    virtual bool IsSupportedForNetworking() const override { return true; }

    UFUNCTION(BlueprintCallable)
    const UDLItemType* GetItemType() const { return ItemType; }

    TScriptInterface<IDLInventoryService> GetInventory() const { return Inventory; }

    template <typename T UE_REQUIRES(TIsDerivedFrom<T, FDLItemFragment>::Value)>
    const T* FindFragment() const { return ensure(ItemType != nullptr) ? ItemType->FindFragment<T>() : nullptr; }

    virtual void NotifyAddedToInventory(TScriptInterface<IDLInventoryService> InInventory) { Inventory = InInventory; }
    virtual void NotifyRemovedFromInventory(TScriptInterface<IDLInventoryService> InInventory) { Inventory = nullptr; }

private:
    UPROPERTY(Replicated)
    TObjectPtr<const UDLItemType> ItemType;

    UPROPERTY()
    TScriptInterface<IDLInventoryService> Inventory;
};
