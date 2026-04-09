// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Equipment/DLItemInstance_Equipment.h"

#include "DLItemInstance_Weapon.generated.h"

class UDLItemType_Ammo;

/*
 * Instance of a Weapon item
 */
UCLASS()
class UDLItemInstance_Weapon : public UDLItemInstance_Equipment
{
    GENERATED_BODY()

public:
    /* Type of Ammo used by this weapon */
    const UDLItemType_Ammo* GetAmmoType() const;
};
