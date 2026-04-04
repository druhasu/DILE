// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInitialWeaponServiceImpl.h"
#include "Features/Equipment/IDLEquipmentService.h"
#include "Features/Equipment/DLEquipmentSlot.h"
#include "Features/Inventory/IDLInventoryService.h"
#include "Features/Weapons/IDLWeaponSlotsService.h"
#include "Features/Weapons/DLItemInstance_Weapon.h"
#include "Features/Weapons/DLItemType_Weapon.h"
#include "Utils/DLEnsure.h"

#include "Net/UnrealNetwork.h"

void UDLInitialWeaponServiceImpl::InitDependencies(
    TScriptInterface<IDLInventoryService> InInventoryService,
    TScriptInterface<IDLEquipmentService> InEquipmentService,
    TScriptInterface<IDLWeaponSlotsService> InWeaponSlotsService
)
{
    InventoryService = InInventoryService;
    EquipmentService = InEquipmentService;
    WeaponSlotsService = InWeaponSlotsService;
}

void UDLInitialWeaponServiceImpl::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, InitialWeapon);
}

void UDLInitialWeaponServiceImpl::SetInitialWeapon(const TDLAssetId<UDLItemType_Weapon>& InWeapon)
{
    DL_ENSURE_RETURN(InWeapon.IsValid());

    if (InitialWeapon == InWeapon)
        return;

    Server_SetInitialWeapon(InWeapon);
}

void UDLInitialWeaponServiceImpl::Server_SetInitialWeapon_Implementation(const TDLAssetId<UDLItemType_Weapon>& InWeapon)
{
    DL_ENSURE_RETURN(InWeapon.IsValid());

    if (InitialWeapon == InWeapon)
        return;

    TDLAssetId<UDLItemType_Weapon> PrevWeapon = InitialWeapon;
    InitialWeapon = InWeapon;
    OnRep_InitialWeapon(PrevWeapon);

    InstancingTask.Cancel();
    InstancingTask = InstantiateAndEquipNewWeapon();
}

void UDLInitialWeaponServiceImpl::OnRep_InitialWeapon(const TDLAssetId<UDLItemType_Weapon>& PrevWeapon)
{
    InitialWeaponChangedDelegate.Broadcast(InitialWeapon);
}

FDLTask UDLInitialWeaponServiceImpl::InstantiateAndEquipNewWeapon(FForceLatentCoroutine)
{
    DL_ENSURE_CORETURN(HasAuthority());

    const UDLItemType_Weapon* ItemType = co_await UE5Coro::Latent::AsyncLoadPrimaryAsset<UDLItemType_Weapon>(InitialWeapon.GetAssetId(), { TEXT("Gameplay") });

    if (WeaponInstance != nullptr)
    {
        InventoryService->RemoveItem(WeaponInstance);
        WeaponInstance = nullptr;
    }

    WeaponInstance = InventoryService->CreateItem(ItemType);
    DL_ENSURE_CORETURN(WeaponInstance != nullptr);

    const UDLEquipmentSlot* TargetSlot = WeaponSlotsService->GetSelectedSlot();
    DL_ENSURE_CORETURN(TargetSlot != nullptr);

    EquipmentService->EquipItem(WeaponInstance, TargetSlot);
}
