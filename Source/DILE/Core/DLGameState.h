// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/GameStateBase.h"

#include "DLGameState.generated.h"

class UObjectContainer;
class FObjectContainerBuilder;
class UDLContainerComponent;

/*
  GameState responsible for creating DI container
 */
UCLASS()
class ADLGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ADLGameState();

    void ReceivedGameModeClass() override;
    void EndPlay(EEndPlayReason::Type InReason) override;

    UObjectContainer* GetContainer() const;

protected:
    void OnRep_ReplicatedHasBegunPlay() override;

private:
    UObjectContainer* GetParentContainer();
    void ConfigureContainer(FObjectContainerBuilder& Builder);
    void OnContainerCreated(UObjectContainer* Container);

    void TryDispatchBeginPlay();
    bool IsReadyForBeginPlay() const;

private:
    UPROPERTY()
    TObjectPtr<UDLContainerComponent> ContainerComponent;

    FTimerHandle BeginPlayTimer;
};
