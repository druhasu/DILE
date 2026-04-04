// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/Pawn.h"

#include "DLPawn.generated.h"

class UObjectContainer;
class FObjectContainerBuilder;
class UDLContainerComponent;

UCLASS()
class DILE_API ADLPawn : public APawn
{
    GENERATED_BODY()

public:
    ADLPawn();

    void PossessedBy(AController* NewController) override;

private:
    UObjectContainer* GetParentContainer();
    void ConfigureContainer(FObjectContainerBuilder& Builder);

private:
    UPROPERTY()
    TObjectPtr<UDLContainerComponent> ContainerComponent;
};
