// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLAimingService.generated.h"

UINTERFACE(MinimalApi, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UDLAimingService : public UInterface { GENERATED_BODY() };

/*
 * Provides aiming data
 */
class DILE_API IDLAimingService
{
    GENERATED_BODY()

public:
    /* World space location of a point under the reticle */
    UFUNCTION(BlueprintCallable)
    virtual FVector GetAimPoint() const = 0;
};
