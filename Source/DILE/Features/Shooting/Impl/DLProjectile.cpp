// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADLProjectile::ADLProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true; // required for replicating spawn to clients

    // create the collision component and assign it as the root
    RootComponent = CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));

    CollisionComponent->SetSphereRadius(16.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

    // create the projectile movement component. No need to attach it because it's not a Scene Component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
}

void ADLProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (AActor* MyInstigator = GetInstigator())
    {
        CollisionComponent->IgnoreActorWhenMoving(MyInstigator, true);
    }
}

void ADLProjectile::TearOff()
{
    Super::TearOff();
    bNetTemporary = true;
}
