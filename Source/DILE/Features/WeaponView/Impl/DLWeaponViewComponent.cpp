// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLWeaponViewComponent.h"
#include "Features/Equipment/IDLEquipmentService.h"
#include "Features/Weapons/IDLWeaponSlotsService.h"
#include "Features/Weapons/DLItemType_Weapon.h"
#include "Utils/DLEnsure.h"

void UDLWeaponViewComponent::InitDependencies(
    TScriptInterface<IDLEquipmentService> InEquipmentService,
    TScriptInterface<IDLWeaponSlotsService> InWeaponSlotsService
)
{
    EquipmentService = InEquipmentService;
    WeaponSlotsService = InWeaponSlotsService;

    for (UActorComponent* Component : GetOwner()->GetComponents())
    {
        if (Component->GetFName() == FirstPersonMeshRef)
            FirstPersonMesh = Cast<USkeletalMeshComponent>(Component);
        else if (Component->GetFName() == ThirdPersonMeshRef)
            ThirdPersonMesh = Cast<USkeletalMeshComponent>(Component);
    }

    DL_ENSURE_RETURN(FirstPersonMesh != nullptr);
    DL_ENSURE_RETURN(ThirdPersonMesh != nullptr);

    EquipmentService->OnItemEquipped().AddUObject(this, &ThisClass::OnItemEquipped);
    EquipmentService->OnItemUnequipped().AddUObject(this, &ThisClass::OnItemUnequipped);
    WeaponSlotsService->OnSelectedSlotChanged().AddUObject(this, &ThisClass::OnSelectedSlotChanged);

    OnSelectedSlotChanged(WeaponSlotsService->GetSelectedSlotIndex());
}

void UDLWeaponViewComponent::EndPlay(EEndPlayReason::Type InReason)
{
    Super::EndPlay(InReason);
    DetachWeapon();
}

void UDLWeaponViewComponent::OnItemEquipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)
{
    DL_ENSURE_RETURN(Item != nullptr);
    DL_ENSURE_RETURN(Slot != nullptr);

    if (WeaponSlotsService->GetSelectedSlot() == Slot)
    {
        OnWeaponChanged(Cast<UDLItemInstance_Weapon>(Item));
    }
}

void UDLWeaponViewComponent::OnItemUnequipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)
{
    DL_ENSURE_RETURN(Item != nullptr);
    DL_ENSURE_RETURN(Slot != nullptr);

    if (WeaponSlotsService->GetSelectedSlot() == Slot)
    {
        OnWeaponChanged(nullptr);
    }
}

void UDLWeaponViewComponent::OnSelectedSlotChanged(int32 NewSlotIndex)
{
    const UDLEquipmentSlot* Slot = WeaponSlotsService->GetSelectedSlot();

    if (Slot == nullptr)
        return;

    OnWeaponChanged(Cast<UDLItemInstance_Weapon>(EquipmentService->GetEquippedItem(Slot)));
}

void UDLWeaponViewComponent::OnWeaponChanged(UDLItemInstance_Weapon* NewWeapon)
{
    if (NewWeapon != nullptr)
        AttachWeapon(NewWeapon);
    else
        DetachWeapon();
}

void UDLWeaponViewComponent::AttachWeapon(UDLItemInstance_Weapon* NewWeapon)
{
    DetachWeapon();

    const FDLItemFragment_WeaponEquipmentView* ViewFragment = NewWeapon->FindFragment<FDLItemFragment_WeaponEquipmentView>();
    DL_ENSURE_RETURN(ViewFragment);

    UClass* ActorClass = ViewFragment->ViewActor.Get();
    DL_ENSURE_RETURN(ActorClass != nullptr);

    AActor* NewWeaponActor = GetWorld()->SpawnActor(ActorClass);
    DL_ENSURE_RETURN(NewWeaponActor != nullptr);

    USkeletalMeshComponent* AttachComponent = SelectAttachComponent();

    NewWeaponActor->AttachToComponent(AttachComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), WeaponSocket);
    NewWeaponActor->ForEachComponent<UPrimitiveComponent>(false, [&](UPrimitiveComponent* Component)
    {
        Component->SetFirstPersonPrimitiveType(AttachComponent->FirstPersonPrimitiveType);
    });

    CurrentAnimLayers = WeaponGripLayers.FindRef(ViewFragment->WeaponGrip);
    ThirdPersonMesh->LinkAnimClassLayers(CurrentAnimLayers.ThirdPersonLayers);
    if (AttachComponent == FirstPersonMesh)
        FirstPersonMesh->LinkAnimClassLayers(CurrentAnimLayers.FirstPersonLayers);

    WeaponActor = NewWeaponActor;
    MuzzleSocketName = ViewFragment->MuzzleSocketName;
}

void UDLWeaponViewComponent::DetachWeapon()
{
    AActor* ExistingActor = WeaponActor.Get();
    if (ExistingActor != nullptr)
        ExistingActor->Destroy();

    if (FirstPersonMesh != nullptr)
        FirstPersonMesh->UnlinkAnimClassLayers(CurrentAnimLayers.FirstPersonLayers);

    if (ThirdPersonMesh != nullptr)
        ThirdPersonMesh->UnlinkAnimClassLayers(CurrentAnimLayers.ThirdPersonLayers);

    WeaponActor = nullptr;
    CurrentAnimLayers = {};
    MuzzleSocketName = {};
}

USkeletalMeshComponent* UDLWeaponViewComponent::SelectAttachComponent()
{
    APawn* Pawn = GetOwner<APawn>();
    return Pawn->IsLocallyControlled() ? FirstPersonMesh : ThirdPersonMesh;
}
