// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLAmmoServiceImpl.h"
#include "Features/Inventory/IDLInventoryService.h"
#include "Features/Weapons/DLItemType_Ammo.h"
#include "Utils/DLEnsure.h"

void UDLAmmoServiceImpl::InitDependencies(TScriptInterface<IDLInventoryService> InInventory)
{
    Inventory = InInventory;

    Inventory->OnItemCountChanged().AddUObject(this, &ThisClass::OnItemCountChanged);
}

int32 UDLAmmoServiceImpl::GetAmmoCount(const UDLItemType_Ammo* AmmoType) const
{
    DL_ENSURE_RETURN(AmmoType != nullptr, 0);

    return Inventory->GetItemCount(AmmoType);
}

void UDLAmmoServiceImpl::OnItemCountChanged(const UDLItemType* ItemType, int32 OldCount, int32 NewCount)
{
    if (const UDLItemType_Ammo* CastedItemType = Cast<UDLItemType_Ammo>(ItemType))
    {
        AmmoCountChangedDelegate.Broadcast(CastedItemType, OldCount, NewCount);
        AmmoCountChangedDynamicDelegate.Broadcast(CastedItemType, OldCount, NewCount);
    }
}
