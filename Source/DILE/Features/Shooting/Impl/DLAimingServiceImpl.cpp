// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLAimingServiceImpl.h"
#include "Systems/PlayerContext/IDLPlayerPawnService.h"

void UDLAimingServiceImpl::InitDependencies(TScriptInterface<IDLPlayerPawnService> InPawnService)
{
    PawnService = InPawnService;
}

FVector UDLAimingServiceImpl::GetAimPoint() const
{
    if (!CachedAimPoint.IsSet())
        UpdateAimInfo();

    return CachedAimPoint.GetValue();
}

FVector UDLAimingServiceImpl::GetAimDirection() const
{
    if (!CachedAimDirection.IsSet())
        UpdateAimInfo();

    return CachedAimDirection.GetValue();
}

void UDLAimingServiceImpl::UpdateAimInfo() const
{
    APawn* Pawn = PawnService->GetPawn();
    if (Pawn == nullptr)
    {
        CachedAimPoint = FVector::ZeroVector;
        CachedAimDirection = FVector::ForwardVector;
        return;
    }

    FMinimalViewInfo ViewInfo;
    Pawn->CalcCamera(0.0f, ViewInfo);

    CachedAimDirection = ViewInfo.Rotation.RotateVector(FVector::ForwardVector);

    FVector TraceEnd = ViewInfo.Location + CachedAimDirection.GetValue() * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Pawn);
    Params.bDebugQuery = true;
    GetWorld()->LineTraceSingleByChannel(Hit, ViewInfo.Location, TraceEnd, TraceChannel, Params);

    FVector Result = Hit.bBlockingHit ? Hit.ImpactPoint : TraceEnd;
    CachedAimPoint = Result;
}
