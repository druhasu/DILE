// Copyright Andrei Sudarikov. All Rights Reserved.

#include "IDLAbilityService.h"

#include "Abilities/GameplayAbility.h"
#include "GameplayAbilitySpec.h"

const FGameplayAbilitySpec* IDLAbilityService::FindActivatableAbility(TSubclassOf<UGameplayAbility> AbilityClass) const
{
    const FGameplayAbilitySpec* FoundSpec = GetActivatableAbilities().FindByPredicate([&](const FGameplayAbilitySpec& Spec)
    {
        return Spec.Ability == AbilityClass.GetDefaultObject();
    });

    return FoundSpec;
}

const FGameplayAbilitySpec* IDLAbilityService::FindActivatableAbility(FGameplayTag AbilityTag) const
{
    const FGameplayAbilitySpec* FoundSpec = GetActivatableAbilities().FindByPredicate([&](const FGameplayAbilitySpec& Spec)
    {
        return Spec.Ability->AbilityTags.HasTag(AbilityTag);
    });

    return FoundSpec;
}

bool IDLAbilityService::HasActivatableAbility(TSubclassOf<UGameplayAbility> AbilityClass) const
{
    return FindActivatableAbility(AbilityClass) != nullptr;
}

bool IDLAbilityService::HasActivatableAbility(FGameplayTag AbilityTag) const
{
    return FindActivatableAbility(AbilityTag) != nullptr;
}
