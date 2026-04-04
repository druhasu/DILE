// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLAssetId.h"

#include "IDLAmmoService.generated.h"

class UDLItemType_Ammo;

UINTERFACE(MinimalApi, NotBlueprintable)
class UDLAmmoService : public UInterface { GENERATED_BODY() };

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDLAmmoCountChangedSignature, const UDLItemType_Ammo*, AmmoType, int32, OldCount, int32, NewCount);

/*
 * Provides access to Ammo counts
 */
class DILE_API IDLAmmoService
{
    GENERATED_BODY()

public:
    using FChangedSignature = TMulticastDelegate<void(const UDLItemType_Ammo* AmmoType, int32 OldCount, int32 NewCount)>;

    /* Counts Ammo of specified type */
    virtual int32 GetAmmoCount(const UDLItemType_Ammo* AmmoType) const = 0;

    /* Called when Ammo count changes */
    virtual FChangedSignature& OnAmmoCountChanged() = 0;

    /* Called when Ammo count changes */
    virtual FDLAmmoCountChangedSignature& OnAmmoCountChangedDynamic() = 0;

    UFUNCTION(BlueprintCallable)
    virtual void TestAmmoMethod(int32 SomeValue) {}
};
