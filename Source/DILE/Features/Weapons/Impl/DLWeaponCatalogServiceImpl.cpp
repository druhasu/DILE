// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLWeaponCatalogServiceImpl.h"
#include "Features/Weapons/DLItemType_Weapon.h"
#include "Utils/DLEnsure.h"

#include "Engine/AssetManager.h"

void UDLWeaponCatalogServiceImpl::InitDependencies()
{
    TArray<FPrimaryAssetId> AssetIDs;
    UAssetManager::Get().GetPrimaryAssetIdList(TDLAssetId<UDLItemType_Weapon>::GetAssetType(), AssetIDs);

    Algo::Transform(AssetIDs, Weapons, [](auto& Id) { return TDLAssetId<UDLItemType_Weapon>(Id); });
}

TDLTask<UDLItemType_Weapon*> UDLWeaponCatalogServiceImpl::GetWeaponData(const TDLAssetId<UDLItemType_Weapon>& WeaponId, const TArray<FName>& LoadBundles) const
{
    DL_ENSURE_CORETURN(WeaponId.IsValid(), nullptr);

    UDLItemType_Weapon* Result = co_await UE5Coro::Latent::AsyncLoadPrimaryAsset<UDLItemType_Weapon>(WeaponId.GetAssetId(), LoadBundles);
    co_return Result;
}
