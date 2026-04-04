// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Weapons/IDLAmmoService.h"

#include "DLAmmoServiceImpl.generated.h"

UCLASS()
class DILE_API UDLAmmoServiceImpl : public UObject, public IDLAmmoService
{
    GENERATED_BODY()

public:
    void InitDependencies();

    // Begin IDLAmmoService
    int32 GetAmmoCount(const UDLItemType_Ammo* AmmoType) const override { return 0; }
    FChangedSignature& OnAmmoCountChanged() override { return AmmoCountChangedDelegate; }
    FDLAmmoCountChangedSignature& OnAmmoCountChangedDynamic() override { return AmmoCountChangedDynamicDelegate; }
    // End IDLAmmoService

private:
    FChangedSignature AmmoCountChangedDelegate;
    FDLAmmoCountChangedSignature AmmoCountChangedDynamicDelegate;
};
