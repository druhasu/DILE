// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLAssetId.h"

#include "IDLInitialWeaponService.generated.h"

class UDLItemType_Weapon;

UINTERFACE(MinimalApi)
class UDLInitialWeaponService : public UInterface { GENERATED_BODY() };

/*
 * Allows to select initial Weapon to be used in the mission
 */
class DILE_API IDLInitialWeaponService
{
    GENERATED_BODY()

public:
    using FChangedSignature = TMulticastDelegate<void(const TDLAssetId<UDLItemType_Weapon>& NewWeapon)>;

    /* Currently selected initial Weapon */
    virtual TDLAssetId<UDLItemType_Weapon> GetInitialWeapon() const = 0;

    /* Set new Weapon as initial */
    virtual void SetInitialWeapon(const TDLAssetId<UDLItemType_Weapon>& InWeapon) = 0;

    /* Called when initial weapon Changes */
    virtual FChangedSignature& OnInitialWeaponChanged() = 0;
};
