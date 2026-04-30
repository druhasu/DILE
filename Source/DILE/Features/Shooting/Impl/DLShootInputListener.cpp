// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLShootInputListener.h"
#include "Features/Abilities/IDLAbilityService.h"
#include "Systems/Input/IDLInputBindingService.h"

#include "GameplayAbilitySpec.h"
#include "InputTriggers.h"

void UDLShootInputListener::InitDependencies(
    TScriptInterface<IDLAbilityService> InAbilityService,
    TScriptInterface<IDLInputBindingService> InInputService
)
{
    AbilityService = InAbilityService;
    InputService = InInputService;

    InputService->BindInput(ShootAction, ETriggerEvent::Triggered, IDLInputBindingService::FActionEventSignature::CreateUObject(this, &ThisClass::OnShootTriggered));
}

void UDLShootInputListener::OnShootTriggered(const FInputActionInstance& ActionInstance)
{
    if (const FGameplayAbilitySpec* Spec = AbilityService->FindActivatableAbility(ShootAbilityTag))
    {
        AbilityService->TryActivateAbility(Spec->Handle);
    }
}
