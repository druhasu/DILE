// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Weapons/IDLWeaponCatalogService.h"

#include "DLWeaponCatalogServiceImpl.generated.h"

UCLASS()
class DILE_API UDLWeaponCatalogServiceImpl : public UObject, public IDLWeaponCatalogService
{
    GENERATED_BODY()

public:
    void InitDependencies();

    // Begin IDLWeaponCatalogService
    const TArray<TDLAssetId<UDLItemType_Weapon>>& GetWeapons() const override { return Weapons; }
    TDLTask<UDLItemType_Weapon*> GetWeaponData(const TDLAssetId<UDLItemType_Weapon>& WeaponId, const TArray<FName>& LoadBundles = {}) const override;
    // End IDLWeaponCatalogService

private:
    UPROPERTY(VisibleInstanceOnly)
    TArray<TDLAssetId<UDLItemType_Weapon>> Weapons;
};
