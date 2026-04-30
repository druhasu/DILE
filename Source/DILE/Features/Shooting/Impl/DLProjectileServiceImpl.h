// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Shooting/IDLProjectileService.h"

#include "DLProjectileServiceImpl.generated.h"

class IDLNetModeService;

UCLASS()
class DILE_API UDLProjectileServiceImpl : public UObject, public IDLProjectileService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLNetModeService> InNetModeService
    );

    // Begin IDLProjectileService
    ADLProjectile* SpawnProjectile(TSubclassOf<ADLProjectile> ProjectileClass, FTransform SpawnTransform, APawn* Instigator) override;
    // End IDLProjectileService

private:
    UPROPERTY()
    TScriptInterface<IDLNetModeService> NetModeService;
};
