// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"

#include "DLAbilitySystemComponent.generated.h"

class IInjector;

UCLASS(Blueprintable)
class UDLAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    using FAbilityChangeSignature = TMulticastDelegate<void(const FGameplayAbilitySpec& AbilitySpec)>;

    void InitDependencies(
        TScriptInterface<IInjector> InInjector
    );

    FAbilityChangeSignature& OnGiveAbility() { return GiveAbilityDelegate; }
    FAbilityChangeSignature& OnRemoveAbility() { return RemoveAbilityDelegate; }

protected:
    // Begin UAbilitySystemComponent overrides
    void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
    void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
    UGameplayAbility* CreateNewInstanceOfAbility(FGameplayAbilitySpec& Spec, const UGameplayAbility* Ability) override;
    //~End UAbilitySystemComponent overrides

private:
    UPROPERTY()
    TScriptInterface<IInjector> Injector;

    FAbilityChangeSignature GiveAbilityDelegate;
    FAbilityChangeSignature RemoveAbilityDelegate;
};
