// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Features/Inventory/DLItemFragment.h"

#include "GameplayTagContainer.h"

#include "DLWeaponViewComponent.generated.h"

class IDLEquipmentService;
class IDLWeaponSlotsService;
class UDLItemInstance_Equipment;
class UDLItemInstance_Weapon;
class UDLEquipmentSlot;
class USkeletalMeshComponent;
class UAnimInstance;

/*
 * Contains settings for visualizing equipped weapon
 */
USTRUCT(BlueprintType)
struct FDLItemFragment_WeaponEquipmentView : public FDLItemFragment
{
    GENERATED_BODY()

public:
    /* Class of the Actor to spawn */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Gameplay"))
    TSoftClassPtr<AActor> ViewActor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "WeaponGrip"))
    FGameplayTag WeaponGrip;
};

/*
 * Contains animation layers for single type of Weapon Grip
 */
USTRUCT(BlueprintType)
struct FDLWeaponGripAnimSet
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UAnimInstance> ThirdPersonLayers;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UAnimInstance> FirstPersonLayers;
};

/*
 * Visualizes currently equiped Weapon
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class DILE_API UDLWeaponViewComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLEquipmentService> InEquipmentService,
        TScriptInterface<IDLWeaponSlotsService> InWeaponSlotsService
    );

    void EndPlay(EEndPlayReason::Type InReason) override;

protected:
    UPROPERTY(EditDefaultsOnly)
    FName FirstPersonMeshRef;

    UPROPERTY(EditDefaultsOnly)
    FName ThirdPersonMeshRef;

    UPROPERTY(EditDefaultsOnly)
    FName WeaponSocket;

    UPROPERTY(EditDefaultsOnly, meta = (Categories = "WeaponGrip"))
    TMap<FGameplayTag, FDLWeaponGripAnimSet> WeaponGripLayers;

private:
    void OnItemEquipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot);
    void OnItemUnequipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot);

    void OnSelectedSlotChanged(int32 NewSlotIndex);
    void OnWeaponChanged(UDLItemInstance_Weapon* NewWeapon);

    void AttachWeapon(UDLItemInstance_Weapon* NewWeapon);
    void DetachWeapon();

    USkeletalMeshComponent* SelectAttachComponent();

private:
    UPROPERTY()
    TScriptInterface<IDLEquipmentService> EquipmentService;

    UPROPERTY()
    TScriptInterface<IDLWeaponSlotsService> WeaponSlotsService;

    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;

    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> ThirdPersonMesh;

    TWeakObjectPtr<AActor> WeaponActor;
    FDLWeaponGripAnimSet CurrentAnimLayers;
};
