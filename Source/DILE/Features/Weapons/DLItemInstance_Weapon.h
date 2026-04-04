// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Equipment/DLItemInstance_Equipment.h"

#include "DLItemInstance_Weapon.generated.h"

class UDLItemType_Ammo;

UCLASS()
class UDLItemInstance_Weapon : public UDLItemInstance_Equipment
{
    GENERATED_BODY()

public:
    const UDLItemType_Ammo* GetAmmoType() const;
};
