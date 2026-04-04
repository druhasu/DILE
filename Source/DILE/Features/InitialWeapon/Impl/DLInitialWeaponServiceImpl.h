// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/InitialWeapon/IDLInitialWeaponService.h"
#include "Systems/ServiceReplication/DLReplicatedService.h"
#include "Utils/DLTask.h"

#include "DLInitialWeaponServiceImpl.generated.h"

class IDLInventoryService;
class IDLEquipmentService;
class IDLWeaponSlotsService;
class UDLItemInstance_Weapon;

UCLASS(Blueprintable)
class DILE_API UDLInitialWeaponServiceImpl : public UDLReplicatedServiceBase, public IDLInitialWeaponService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLInventoryService> InInventoryService,
        TScriptInterface<IDLEquipmentService> InEquipmentService,
        TScriptInterface<IDLWeaponSlotsService> InWeaponSlotsService
    );

    // Begin IDLInitialWeaponService
    TDLAssetId<UDLItemType_Weapon> GetInitialWeapon() const override { return InitialWeapon; }
    void SetInitialWeapon(const TDLAssetId<UDLItemType_Weapon>& InWeapon) override;
    FChangedSignature& OnInitialWeaponChanged() override { return InitialWeaponChangedDelegate; }
    // End IDLInitialWeaponService

private:
    UFUNCTION(Server, Reliable)
    void Server_SetInitialWeapon(const TDLAssetId<UDLItemType_Weapon>& InWeapon);

    UFUNCTION()
    void OnRep_InitialWeapon(const TDLAssetId<UDLItemType_Weapon>& PrevWeapon);

    FDLTask InstantiateAndEquipNewWeapon(FForceLatentCoroutine = {});

private:
    UPROPERTY(VisibleInstanceOnly)
    TScriptInterface<IDLInventoryService> InventoryService;

    UPROPERTY(VisibleInstanceOnly)
    TScriptInterface<IDLEquipmentService> EquipmentService;

    UPROPERTY(VisibleInstanceOnly)
    TScriptInterface<IDLWeaponSlotsService> WeaponSlotsService;

    UPROPERTY(ReplicatedUsing = OnRep_InitialWeapon)
    TDLAssetId<UDLItemType_Weapon> InitialWeapon;

    UPROPERTY()
    TObjectPtr<UDLItemInstance_Weapon> WeaponInstance;

    FChangedSignature InitialWeaponChangedDelegate;
    FDLTask InstancingTask = FDLTask::CompletedCoroutine;
};
