// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLEquipmentAbilityServiceImpl.h"
#include "Features/Abilities/IDLAbilityService.h"
#include "Features/Abilities/DLGameplayAbility.h"
#include "Features/Abilities/DLItemFragment_Ability.h"
#include "Features/Equipment/IDLEquipmentService.h"
#include "Features/Equipment/DLItemInstance_Equipment.h"

#include "GameplayAbilitySpec.h"

void UDLEquipmentAbilityServiceImpl::InitDependencies(
    TScriptInterface<IDLEquipmentService> InEquipmentService,
    TScriptInterface<IDLAbilityService> InAbilityService
)
{
    EquipmentService = InEquipmentService;
    AbilityService = InAbilityService;

    EquipmentService->OnItemEquipped().AddUObject(this, &ThisClass::OnItemEquipped);
    EquipmentService->OnItemUnequipped().AddUObject(this, &ThisClass::OnItemUnequipped);
}

void UDLEquipmentAbilityServiceImpl::OnItemEquipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)
{
    if (const FDLItemFragment_Ability* Fragment = Item->FindFragment<FDLItemFragment_Ability>())
    {
        AbilityService->GiveAbility(FGameplayAbilitySpec(Fragment->Ability, 1, -1, Item));
    }
}

void UDLEquipmentAbilityServiceImpl::OnItemUnequipped(UDLItemInstance_Equipment* Item, const UDLEquipmentSlot* Slot)
{
    if (const FDLItemFragment_Ability* Fragment = Item->FindFragment<FDLItemFragment_Ability>())
    {
        if (const FGameplayAbilitySpec* Spec = AbilityService->FindActivatableAbility(Fragment->Ability))
        {
            AbilityService->RemoveAbility(Spec->Handle);
        }
    }
}
