// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLProjectileServiceImpl.h"
#include "DLProjectile.h"
#include "Systems/NetMode/IDLNetModeService.h"

void UDLProjectileServiceImpl::InitDependencies(
    TScriptInterface<IDLNetModeService> InNetModeService
)
{
    NetModeService = InNetModeService;
}

ADLProjectile* UDLProjectileServiceImpl::SpawnProjectile(TSubclassOf<ADLProjectile> ProjectileClass, FTransform SpawnTransform, APawn* Instigator)
{
    // for now, just return when called on client. later we may add predictive spawning and reconcilation
    if (NetModeService->IsClient())
        return nullptr;

    FActorSpawnParameters Parameters;
    Parameters.bDeferConstruction = true;
    Parameters.Instigator = Instigator;
    Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // force default scale
    SpawnTransform.SetScale3D(FVector::OneVector);

    // here we can add pooling when needed
    ADLProjectile* Result = GetWorld()->SpawnActor<ADLProjectile>(ProjectileClass, SpawnTransform, Parameters);

    // ... additional initialization goes here

    Result->FinishSpawning(SpawnTransform);

    return Result;
}
