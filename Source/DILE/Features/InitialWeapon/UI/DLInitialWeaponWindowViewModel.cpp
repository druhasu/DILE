// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInitialWeaponWindowViewModel.h"
#include "Features/InitialWeapon/IDLInitialWeaponService.h"
#include "Features/Weapons/IDLWeaponCatalogService.h"

#include "Mvvm/MvvmUtils.h"

void UDLInitialWeaponWindowViewModel::InitDependencies(
    TScriptInterface<IDLWeaponCatalogService> InWeaponCatalog,
    TScriptInterface<IDLInitialWeaponService> InInitialWeaponService
)
{
    WeaponCatalog = InWeaponCatalog;
    InitialWeaponService = InInitialWeaponService;

    InitWeapons();
}

void UDLInitialWeaponWindowViewModel::ApplySelection()
{
    if (SelectedWeaponField != nullptr)
    {
        InitialWeaponService->SetInitialWeapon(SelectedWeaponField->GetWeaponId());
    }
}

FDLTask UDLInitialWeaponWindowViewModel::InitWeapons()
{
    SetLoading(true);

    TArray<TDLTask<UDLItemType_Weapon*>> AssetTasks;
    Algo::Transform(WeaponCatalog->GetWeapons(), AssetTasks, [&](const TDLAssetId<UDLItemType_Weapon>& ID)
    {
        return WeaponCatalog->GetWeaponData(ID, { "UI" });
    });

    TArray<UDLItemType_Weapon*> Assets;
    Assets.Reserve(AssetTasks.Num());

    for (auto& AssetTask : AssetTasks)
    {
        if (UDLItemType_Weapon* WeaponData = co_await AssetTask)
        {
            Assets.Emplace(WeaponData);
        }
    }

    MvvmUtils::SyncViewModelCollection(WeaponsField, Assets);
    RaiseChanged(WeaponsProperty());

    TDLAssetId<UDLItemType_Weapon> SelectedID = InitialWeaponService->GetInitialWeapon();
    TObjectPtr<UDLInitialWeaponEntryViewModel>* NewSelectedViewModel = WeaponsField.FindByPredicate([&](UDLInitialWeaponEntryViewModel* ViewModel)
    {
        return ViewModel->GetWeaponId() == SelectedID;
    });

    if (NewSelectedViewModel != nullptr)
    {
        SetSelectedWeapon(*NewSelectedViewModel);
    }
    else if (WeaponsField.Num() > 0)
    {
        SetSelectedWeapon(WeaponsField[0]);
    }
    else
    {
        SetSelectedWeapon(nullptr);
    }

    SetLoading(false);
}
