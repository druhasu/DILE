// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLAbilityServiceImpl.h"
#include "DLAbilitySystemComponent.h"
#include "Systems/PlayerContext/IDLPlayerPawnService.h"
#include "Systems/NetMode/IDLNetModeService.h"
#include "Utils/DLEnsure.h"
#include "Utils/DLStaticEmpty.h"

void UDLAbilityServiceImpl::InitDependencies(
    UDLAbilitySystemComponent* InAbilitySystemComponent,
    TScriptInterface<IDLPlayerPawnService> InPawnService,
    TScriptInterface<IDLNetModeService> InNetModeService
)
{
    AbilitySystemComponent = InAbilitySystemComponent;
    PawnService = InPawnService;
    NetModeService = InNetModeService;

    PawnService->OnPawnChanged().AddUObject(this, &ThisClass::OnPawnChanged);
    OnPawnChanged(PawnService->GetPawn());

    AbilitySystemComponent->OnGiveAbility().AddUObject(this, &ThisClass::OnGiveAbility);
    AbilitySystemComponent->OnRemoveAbility().AddUObject(this, &ThisClass::OnRemoveAbility);
    AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &ThisClass::OnAbilityEndedInternal);
    AbilitySystemComponent->AbilityCommittedCallbacks.AddUObject(this, &ThisClass::OnAbilityCommitedInternal);
    AbilitySystemComponent->AbilityActivatedCallbacks.AddUObject(this, &ThisClass::OnAbilityActivatedInternal);
}

const TArray<FGameplayAbilitySpec>& UDLAbilityServiceImpl::GetActivatableAbilities() const
{
    return AbilitySystemComponent->GetActivatableAbilities();
}

const FGameplayAbilityActorInfo& UDLAbilityServiceImpl::GetActorInfo() const
{
    FGameplayAbilityActorInfo* Info = AbilitySystemComponent->AbilityActorInfo.Get();
    return Info ? *Info : TDLStaticEmpty<FGameplayAbilityActorInfo>::Value;
}

bool UDLAbilityServiceImpl::HasAuthority() const
{
    return !NetModeService->IsNetMode(ENetMode::NM_Client);
}

FGameplayAbilitySpecHandle UDLAbilityServiceImpl::GiveAbility(const FGameplayAbilitySpec& AbilitySpec)
{
    if (HasAuthority())
    {
        return AbilitySystemComponent->GiveAbility(AbilitySpec);
    }

    return {};
}

void UDLAbilityServiceImpl::RemoveAbility(const FGameplayAbilitySpecHandle& Handle)
{
    if (HasAuthority())
    {
        return AbilitySystemComponent->ClearAbility(Handle);
    }
}

bool UDLAbilityServiceImpl::TryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation)
{
    return AbilitySystemComponent->TryActivateAbility(AbilityToActivate, bAllowRemoteActivation);
}

bool UDLAbilityServiceImpl::TryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, const FGameplayTag& EventTag, const FGameplayEventData& Payload)
{
    DL_ENSURE_RETURN(EventTag.IsValid(), false);

    return AbilitySystemComponent->TriggerAbilityFromGameplayEvent(AbilityToActivate, AbilitySystemComponent->AbilityActorInfo.Get(), EventTag, &Payload, *AbilitySystemComponent.Get());
}

void UDLAbilityServiceImpl::CancelAbility(FGameplayAbilitySpecHandle AbilityToCancel)
{
    AbilitySystemComponent->CancelAbilityHandle(AbilityToCancel);
}

void UDLAbilityServiceImpl::OnPawnChanged(APawn* NewPawn)
{
    if (NewPawn != nullptr)
    {
        AbilitySystemComponent->InitAbilityActorInfo(AbilitySystemComponent->GetOwner(), NewPawn);
    }
}

void UDLAbilityServiceImpl::OnGiveAbility(const FGameplayAbilitySpec& Spec)
{
    ActivatableAbilitiesChangedDelegate.Broadcast();
}

void UDLAbilityServiceImpl::OnRemoveAbility(const FGameplayAbilitySpec& Spec)
{
    ActivatableAbilitiesChangedDelegate.Broadcast();
}

void UDLAbilityServiceImpl::OnAbilityEndedInternal(const FAbilityEndedData& EndData)
{
    AbilityEndedDelegate.Broadcast(EndData);
}

void UDLAbilityServiceImpl::OnAbilityCommitedInternal(UGameplayAbility* ActivatedAbility)
{
    AbilityCommitedDelegate.Broadcast(ActivatedAbility);
}

void UDLAbilityServiceImpl::OnAbilityActivatedInternal(UGameplayAbility* ActivatedAbility)
{
    AbilityActivatedDelegate.Broadcast(ActivatedAbility);
}
