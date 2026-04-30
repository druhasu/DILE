// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"

#include "AbilitySystemInterface.h"

#include "DLPawn.generated.h"

class UObjectContainer;
class FObjectContainerBuilder;
class UDLContainerComponent;
class UAbilitySystemComponent;

/*
 * Generic Pawn class
 * The idea behind it is that it should not contain any game specific logic itself
 * We intend to "enrich" it by adding components and services to its container
 */
UCLASS()
class DILE_API ADLPawn : public APawn, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ADLPawn();

    void PossessedBy(AController* NewController) override;

    // Begin IAbilitySystemInterface
    UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    // End IAbilitySystemInterface

private:
    UObjectContainer* GetParentContainer();
    void ConfigureContainer(FObjectContainerBuilder& Builder);

private:
    UPROPERTY(VisibleInstanceOnly)
    TObjectPtr<UDLContainerComponent> ContainerComponent;
};
