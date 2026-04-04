// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLItemInstance_Weapon.h"
#include "DLItemFragment_Weapon.h"
#include "Utils/DLEnsure.h"

const UDLItemType_Ammo* UDLItemInstance_Weapon::GetAmmoType() const
{
    const FDLItemFragment_Weapon* WeaponFragment = FindFragment<FDLItemFragment_Weapon>();
    DL_ENSURE_RETURN(WeaponFragment != nullptr, nullptr);

    return WeaponFragment->AmmoType;
}
