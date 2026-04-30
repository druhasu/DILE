// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Shooting/IDLAimingService.h"

#include "DLAimingServiceImpl.generated.h"

class IDLPlayerPawnService;

UCLASS(Blueprintable)
class DILE_API UDLAimingServiceImpl : public UObject, public IDLAimingService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLPlayerPawnService> InPawnService
    );

    // Begin IDLAimingService
    FVector GetAimPoint() const override;
    FVector GetAimDirection() const override;
    // End IDLAimingService

protected:
    UPROPERTY(EditDefaultsOnly, meta = (Units = "cm"))
    double TraceDistance = 2000;

    UPROPERTY(EditDefaultsOnly)
    TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_GameTraceChannel1;

private:
    void UpdateAimInfo() const;

private:
    UPROPERTY()
    TScriptInterface<IDLPlayerPawnService> PawnService;

    mutable TFrameValue<FVector> CachedAimPoint;
    mutable TFrameValue<FVector> CachedAimDirection;
    mutable TFrameValue<FVector> CachedMuzzlePoint;
};
