// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLTargetingService.generated.h"

struct FGameplayAbilityTargetDataHandle;

UINTERFACE(MinimalApi, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UDLTargetingService : public UInterface { GENERATED_BODY() };

/*
 * Provides helper functions for building TargetData for shooting
 */
class DILE_API IDLTargetingService
{
    GENERATED_BODY()

public:
    /* Makes TargetData for hitscan weapons */
    UFUNCTION(BlueprintPure = false)
    virtual FGameplayAbilityTargetDataHandle MakeHitscanTargetData() const = 0;

    /* Makes TargetData for projectile weapons */
    UFUNCTION(BlueprintPure = false)
    virtual FGameplayAbilityTargetDataHandle MakeProjectileTargetData() const = 0;
};

