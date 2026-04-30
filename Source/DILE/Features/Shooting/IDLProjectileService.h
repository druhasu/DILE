// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLProjectileService.generated.h"

class ADLProjectile;
class APawn;

UINTERFACE(MinimalApi, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UDLProjectileService : public UInterface { GENERATED_BODY() };

/*
 * Allows spawning and launching of projectiles
 */
class DILE_API IDLProjectileService
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    virtual ADLProjectile* SpawnProjectile(TSubclassOf<ADLProjectile> ProjectileClass, FTransform SpawnTransform, APawn* Instigator) = 0;
};
