// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLAimingServiceImpl.h"
#include "Systems/PlayerContext/IDLPlayerPawnService.h"

void UDLAimingServiceImpl::InitDependencies(TScriptInterface<IDLPlayerPawnService> InPawnService)
{
    PawnService = InPawnService;
}

FVector UDLAimingServiceImpl::GetAimPoint() const
{
    if (CachedAimPoint.IsSet())
        return CachedAimPoint.GetValue();

    APawn* Pawn = PawnService->GetPawn();
    if (Pawn == nullptr)
        return FVector::ZeroVector;

    FMinimalViewInfo ViewInfo;
    Pawn->CalcCamera(0.0f, ViewInfo);

    FVector TraceEnd = ViewInfo.Location + ViewInfo.Rotation.RotateVector(FVector::ForwardVector * TraceDistance);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Pawn);
    Params.bDebugQuery = true;
    GetWorld()->LineTraceSingleByChannel(Hit, ViewInfo.Location, TraceEnd, TraceChannel, Params);

    FVector Result = Hit.bBlockingHit ? Hit.ImpactPoint : TraceEnd;
    CachedAimPoint = Result;

    return Result;
}
