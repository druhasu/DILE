// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

#include "DLShootInputListener.generated.h"

class IDLAbilityService;
class IDLInputBindingService;
class UInputAction;
struct FInputActionInstance;

UCLASS(Abstract, Blueprintable)
class UDLShootInputListener : public UObject
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLAbilityService> InAbilityService,
        TScriptInterface<IDLInputBindingService> InInputService
    );

protected:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UInputAction> ShootAction;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag ShootAbilityTag;

private:
    void OnShootTriggered(const FInputActionInstance& ActionInstance);

private:
    UPROPERTY()
    TScriptInterface<IDLAbilityService> AbilityService;

    UPROPERTY()
    TScriptInterface<IDLInputBindingService> InputService;
};
