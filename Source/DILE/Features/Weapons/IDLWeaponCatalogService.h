// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLTask.h"
#include "Utils/DLAssetId.h"

#include "IDLWeaponCatalogService.generated.h"

class UDLItemType_Weapon;

UINTERFACE(MinimalApi)
class UDLWeaponCatalogService : public UInterface { GENERATED_BODY() };

/*
 * Provides info about all existing weapons
 */
class DILE_API IDLWeaponCatalogService
{
    GENERATED_BODY()

public:
    /* List of all existing weapons */
    virtual const TArray<TDLAssetId<UDLItemType_Weapon>>& GetWeapons() const = 0;

    /* Loads Weapon data with given WeaponId */
    virtual TDLTask<UDLItemType_Weapon*> GetWeaponData(const TDLAssetId<UDLItemType_Weapon>& WeaponId, const TArray<FName>& LoadBundles = {}) const = 0;
};
