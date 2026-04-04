// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "DI/IInstanceFactory.h"

#include "DLGameState.generated.h"

class UObjectContainer;
class FObjectContainerBuilder;
class UDLContainerComponent;

/*
  GameState responsible for creating DI container
 */
UCLASS()
class ADLGameState : public AGameStateBase, public IInstanceFactory
{
    GENERATED_BODY()

public:
    ADLGameState();

    void ReceivedGameModeClass() override;
    void EndPlay(EEndPlayReason::Type InReason) override;

    UObjectContainer* GetContainer() const;

protected:
    void OnRep_ReplicatedHasBegunPlay() override;

    // Begin IInstanceFactory
    bool IsClassSupported(UClass* EffectiveClass) const override;
    UObject* Create(UObject* Outer, UClass* EffectiveClass) const override;
    void FinalizeCreation(UObject* Object) const override;
    // End IInstanceFactory

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
