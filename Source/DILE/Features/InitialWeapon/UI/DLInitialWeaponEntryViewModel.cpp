// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInitialWeaponEntryViewModel.h"
#include "Features/Weapons/DLItemType_Weapon.h"
#include "Utils/DLEnsure.h"

void UDLInitialWeaponEntryViewModel::SetModel(UDLItemType_Weapon* InWeaponAsset)
{
    DL_ENSURE_RETURN(InWeaponAsset != nullptr);

    WeaponAsset = InWeaponAsset;

    SetWeaponId(TDLAssetId<UDLItemType_Weapon>(WeaponAsset));
    SetWeaponName(WeaponAsset->Common.Name);
    SetIcon(WeaponAsset->Weapon.Icon.Get());
}
