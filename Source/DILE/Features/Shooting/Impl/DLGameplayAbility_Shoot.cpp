// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLGameplayAbility_Shoot.h"
#include "Utils/DLEnsure.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

void UDLGameplayAbility_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    DL_ENSURE_RETURN(ActorInfo != nullptr);

    ActorInfo->AbilitySystemComponent->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnTargetDataSet);

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDLGameplayAbility_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    ActorInfo->AbilitySystemComponent->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).RemoveAll(this);

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDLGameplayAbility_Shoot::SubmitTargetData(const FGameplayAbilityTargetDataHandle& Handle)
{
    DL_ENSURE_RETURN(CurrentActorInfo != nullptr);

    // this function should only be called from client side
    if (!HasAuthority(&CurrentActivationInfo))
    {
        FScopedPredictionWindow ScopedPredictionWindow(CurrentActorInfo->AbilitySystemComponent.Get());

        CurrentActorInfo->AbilitySystemComponent->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), Handle, {}, CurrentActorInfo->AbilitySystemComponent->ScopedPredictionKey);

        OnTargetDataSet(Handle, {});
    }
}

void UDLGameplayAbility_Shoot::OnTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData, FGameplayTag)
{
    DL_ENSURE_RETURN(CurrentActorInfo != nullptr);

    if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
    {
        // Let the blueprint do stuff like apply effects to the targets
        K2_TargetDataReceived(TargetData);
    }
    else
    {
        K2_EndAbility();
    }

    CurrentActorInfo->AbilitySystemComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
}
