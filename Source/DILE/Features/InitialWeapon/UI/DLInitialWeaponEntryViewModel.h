// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Mvvm/BaseViewModel.h"
#include "Utils/DLAssetId.h"

#include "DLInitialWeaponEntryViewModel.generated.h"

class UDLItemType_Weapon;

UCLASS()
class UDLInitialWeaponEntryViewModel : public UBaseViewModel
{
    GENERATED_BODY()

    VM_PROP_AG_AS(TDLAssetId<UDLItemType_Weapon>, WeaponId);
    VM_PROP_AG_AS(FText, WeaponName);
    VM_PROP_AG_AS(TObjectPtr<UTexture2D>, Icon);

public:
    void SetModel(UDLItemType_Weapon* InWeaponAsset);

private:
    UPROPERTY()
    TObjectPtr<UDLItemType_Weapon> WeaponAsset;
};
