// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Weapons/IDLAmmoService.h"

#include "DLAmmoServiceImpl.generated.h"

class IDLInventoryService;
class UDLItemType;

UCLASS()
class DILE_API UDLAmmoServiceImpl : public UObject, public IDLAmmoService
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IDLInventoryService> InInventory);

    // Begin IDLAmmoService
    int32 GetAmmoCount(const UDLItemType_Ammo* AmmoType) const override;
    FChangedSignature& OnAmmoCountChanged() override { return AmmoCountChangedDelegate; }
    FDLAmmoCountChangedSignature& OnAmmoCountChangedDynamic() override { return AmmoCountChangedDynamicDelegate; }
    // End IDLAmmoService

private:
    void OnItemCountChanged(const UDLItemType* ItemType, int32 OldCount, int32 NewCount);

private:
    UPROPERTY()
    TScriptInterface<IDLInventoryService> Inventory;

    FChangedSignature AmmoCountChangedDelegate;
    FDLAmmoCountChangedSignature AmmoCountChangedDynamicDelegate;
};
