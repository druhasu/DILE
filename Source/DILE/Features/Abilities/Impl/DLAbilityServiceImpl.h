// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Abilities/IDLAbilityService.h"

#include "DLAbilityServiceImpl.generated.h"

class IDLPlayerPawnService;
class IDLNetModeService;
class UDLAbilitySystemComponent;
class APawn;

UCLASS()
class DILE_API UDLAbilityServiceImpl : public UObject, public IDLAbilityService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        UDLAbilitySystemComponent* InAbilitySystemComponent,
        TScriptInterface<IDLPlayerPawnService> InPawnService,
        TScriptInterface<IDLNetModeService> InNetModeService
    );

    // Begin IDLAbilityService
    const TArray<FGameplayAbilitySpec>& GetActivatableAbilities() const override;
    const FGameplayAbilityActorInfo& GetActorInfo() const override;
    bool HasAuthority() const override;
    FWAbilitiesChangedSignature& OnActivatableAbilitiesChanged() override { return ActivatableAbilitiesChangedDelegate; }
    FGameplayAbilitySpecHandle GiveAbility(const FGameplayAbilitySpec& AbilitySpec) override;
    void RemoveAbility(const FGameplayAbilitySpecHandle& Handle) override;
    bool TryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation = true) override;
    bool TryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, const FGameplayTag& EventTag, const FGameplayEventData& Payload) override;
    void CancelAbility(FGameplayAbilitySpecHandle AbilityToCancel) override;
    FWAbilityEndedSignature& OnAbilityEnded() override { return AbilityEndedDelegate; }
    FWGenericAbilitySignature& OnAbilityCommited() override { return AbilityCommitedDelegate; }
    FWGenericAbilitySignature& OnAbilityActivated() override { return AbilityActivatedDelegate; }
    // End IDLAbilityService

private:
    void OnPawnChanged(APawn* NewPawn);

    void OnGiveAbility(const FGameplayAbilitySpec& Spec);
    void OnRemoveAbility(const FGameplayAbilitySpec& Spec);
    void OnAbilityEndedInternal(const FAbilityEndedData& EndData);
    void OnAbilityCommitedInternal(UGameplayAbility* ActivatedAbility);
    void OnAbilityActivatedInternal(UGameplayAbility* ActivatedAbility);

private:
    UPROPERTY()
    TObjectPtr<UDLAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TScriptInterface<IDLPlayerPawnService> PawnService;

    UPROPERTY()
    TScriptInterface<IDLNetModeService> NetModeService;

    FWAbilitiesChangedSignature ActivatableAbilitiesChangedDelegate;
    FWAbilityEndedSignature AbilityEndedDelegate;
    FWGenericAbilitySignature AbilityCommitedDelegate;
    FWGenericAbilitySignature AbilityActivatedDelegate;
};
