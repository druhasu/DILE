// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLTargetingServiceImpl.h"
#include "Features/Shooting/IDLAimingService.h"
#include "Features/WeaponView/IDLWeaponSocketsService.h"
#include "Systems/PlayerContext/IDLPlayerPawnService.h"

void UDLTargetingServiceImpl::InitDependencies(
    TScriptInterface<IDLAimingService> InAimingService,
    TScriptInterface<IDLWeaponSocketsService> InWeaponSocketsService,
    TScriptInterface<IDLPlayerPawnService> InPawnService
)
{
    AimingService = InAimingService;
    WeaponSocketsService = InWeaponSocketsService;
    PawnService = InPawnService;
}

FGameplayAbilityTargetDataHandle UDLTargetingServiceImpl::MakeHitscanTargetData() const
{
    const FVector AimPoint = AimingService->GetAimPoint();
    const FVector TraceStartPoint = WeaponSocketsService->GetMuzzleSocket().GetTransform().GetLocation();
    const FVector AimDirection = AimPoint - TraceStartPoint;
    const double Length = AimDirection.Size();
    const FVector TraceEndPoint = Length > 0.1 ? TraceStartPoint + AimDirection * (TraceDistance / Length) : AimPoint;

    FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
    FCollisionQueryParams Params(SCENE_QUERY_STAT(TargetingService));
    Params.AddIgnoredActor(PawnService->GetPawn());

    GetWorld()->LineTraceSingleByChannel(TargetData->HitResult, TraceStartPoint, TraceEndPoint, TraceChannel, Params);

    return { TargetData };
}

FGameplayAbilityTargetDataHandle UDLTargetingServiceImpl::MakeProjectileTargetData() const
{
    FGameplayAbilityTargetData_Projectile* TargetData = new FGameplayAbilityTargetData_Projectile();
    TargetData->SourceLocation.LiteralTransform = WeaponSocketsService->GetMuzzleSocket().GetTransform();
    TargetData->SourceLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;

    return { TargetData };
}

bool FGameplayAbilityTargetData_Projectile::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
    SourceLocation.NetSerialize(Ar, Map, bOutSuccess);

    bOutSuccess = true;
    return true;
}
