// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLAbilitySystemComponent.h"

#include "DI/IInjector.h"

void UDLAbilitySystemComponent::InitDependencies(
    TScriptInterface<IInjector> InInjector
)
{
    Injector = InInjector;
}

void UDLAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability != nullptr)
    {
        Super::OnGiveAbility(AbilitySpec);
        GiveAbilityDelegate.Broadcast(AbilitySpec);
    }
}

void UDLAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
    if (AbilitySpec.Ability != nullptr)
    {
        Super::OnRemoveAbility(AbilitySpec);
        RemoveAbilityDelegate.Broadcast(AbilitySpec);
    }
}

UGameplayAbility* UDLAbilitySystemComponent::CreateNewInstanceOfAbility(FGameplayAbilitySpec& Spec, const UGameplayAbility* Ability)
{
    UGameplayAbility* Result = Super::CreateNewInstanceOfAbility(Spec, Ability);
    if (Result != nullptr)
    {
        Injector->Inject(Result);
    }

    return Result;
}
