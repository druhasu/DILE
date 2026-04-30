// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Abilities/DLGameplayAbility.h"

#include "DLGameplayAbility_Shoot.generated.h"

struct FGameplayAbilityTargetDataHandle;

UCLASS()
class UDLGameplayAbility_Shoot : public UDLGameplayAbility
{
    GENERATED_BODY()

public:
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
    UFUNCTION(BlueprintCallable)
    void SubmitTargetData(const FGameplayAbilityTargetDataHandle& Handle);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Target Data Received"))
    void K2_TargetDataReceived(const FGameplayAbilityTargetDataHandle& Handle);

private:
    void OnTargetDataSet(const FGameplayAbilityTargetDataHandle& TargetData, FGameplayTag);
};
