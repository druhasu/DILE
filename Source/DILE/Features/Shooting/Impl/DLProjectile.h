// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"

#include "DLProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class ADLProjectile : public AActor
{
    GENERATED_BODY()

public:
    ADLProjectile();

    void PostInitializeComponents() override;
    void TearOff() override;

protected:
    /* Provides collision detection for the projectile */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USphereComponent* CollisionComponent;

    /* Handles movement for the projectile */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement;
};
