// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameplayAbilitySpecHandle.h"
#include "UObject/Interface.h"

#include "IDLAbilityService.generated.h"

class UGameplayAbility;
struct FGameplayTag;
struct FGameplayTagContainer;
struct FGameplayAbilitySpec;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;
struct FAbilityEndedData;

UINTERFACE(MinimalApi)
class UDLAbilityService : public UInterface
{
    GENERATED_BODY()
};

/*
 * Allows to control abilities
 */
class DILE_API IDLAbilityService
{
    GENERATED_BODY()

public:
    using FWAbilitiesChangedSignature = TMulticastDelegate<void()>;
    using FWAbilityEndedSignature = TMulticastDelegate<void(const FAbilityEndedData& EndData)>;
    using FWGenericAbilitySignature = TMulticastDelegate<void(UGameplayAbility* ActivatedAbility)>;

    /* List of all activatable abilities */
    virtual const TArray<FGameplayAbilitySpec>& GetActivatableAbilities() const = 0;

    /* Actor info used by the service */
    virtual const FGameplayAbilityActorInfo& GetActorInfo() const = 0;

    /* Whether this service represents Authority */
    virtual bool HasAuthority() const = 0;

    /* Called when list of activatable abilities changes */
    virtual FWAbilitiesChangedSignature& OnActivatableAbilitiesChanged() = 0;

    /* Adds given ability. Does nothing if not authority */
    virtual FGameplayAbilitySpecHandle GiveAbility(const FGameplayAbilitySpec& AbilitySpec) = 0;

    /* Removes given ability. Does nothing if not authority */
    virtual void RemoveAbility(const FGameplayAbilitySpecHandle& Handle) = 0;

    /*
     * Attempts to activate the given ability.
     * Will check costs and requirements before doing so.
     * If bAllowRemoteActivation is true, it will remotely activate local/server abilities, if false it will only try to locally activate the ability.
     * Returns true if it thinks it activated, but it may return false positives due to failure later in activation.
     */
    virtual bool TryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation = true) = 0;

    /*
     * Attempts to activate the given ability from a gameplay event.
     * Will check costs and requirements before doing so.
     * Will only trigger on local/server depending on execution flags.
     * Returns true if it thinks it activated, but it may return false positives due to failure later in activation.
     */
    virtual bool TryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, const FGameplayTag& EventTag, const FGameplayEventData& Payload) = 0;

    /** Cancels the ability indicated by passed in spec handle. If handle is not found among reactivated abilities nothing happens. */
    virtual void CancelAbility(FGameplayAbilitySpecHandle AbilityToCancel) = 0;

    /* Called when any Ability ends */
    virtual FWAbilityEndedSignature& OnAbilityEnded() = 0;

    /* Called when any Ability is commited */
    virtual FWGenericAbilitySignature& OnAbilityCommited() = 0;

    /* Called when any Ability is activated */
    virtual FWGenericAbilitySignature& OnAbilityActivated() = 0;

    /* Spec of activatable ability with given class. Returns nullptr if not found */
    const FGameplayAbilitySpec* FindActivatableAbility(TSubclassOf<UGameplayAbility> AbilityClass) const;

    /* Spec of activatable ability with given Tag. Returns nullptr if not found */
    const FGameplayAbilitySpec* FindActivatableAbility(FGameplayTag AbilityTag) const;

    /* Whether we have activatable ability with given class */
    bool HasActivatableAbility(TSubclassOf<UGameplayAbility> AbilityClass) const;

    /* Whether we have activatable ability with given Tag */
    bool HasActivatableAbility(FGameplayTag AbilityTag) const;
};
