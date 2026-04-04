// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Mvvm/BaseViewModel.h"
#include "DLInitialWeaponEntryViewModel.h"
#include "Utils/DLTask.h"

#include "DLInitialWeaponWindowViewModel.generated.h"

class IDLWeaponCatalogService;
class IDLInitialWeaponService;

UCLASS()
class UDLInitialWeaponWindowViewModel : public UBaseViewModel
{
    GENERATED_BODY()

    VM_PROP_AG_AS(bool, Loading);
    VM_PROP_AG_AS(const TArray<TObjectPtr<UDLInitialWeaponEntryViewModel>>&, Weapons);
    VM_PROP_AG_AS(TObjectPtr<UDLInitialWeaponEntryViewModel>, SelectedWeapon, public);

public:
    void InitDependencies(
        TScriptInterface<IDLWeaponCatalogService> InWeaponCatalog,
        TScriptInterface<IDLInitialWeaponService> InInitialWeaponService
    );

    UFUNCTION(BlueprintCallable)
    void ApplySelection();

private:
    FDLTask InitWeapons();

private:
    UPROPERTY()
    TScriptInterface<IDLWeaponCatalogService> WeaponCatalog;

    UPROPERTY()
    TScriptInterface<IDLInitialWeaponService> InitialWeaponService;
};
