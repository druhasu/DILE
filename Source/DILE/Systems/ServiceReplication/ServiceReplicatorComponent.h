// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "DI/ObjectsCollection.h"
#include "ServiceReplicatorComponent.generated.h"

class UCounterService;
class IReplicatedService;

UCLASS()
class UServiceReplicatorComponent: public UActorComponent
{
    GENERATED_BODY()

public:
    UServiceReplicatorComponent();

    void InitDependencies(TOptional<TObjectsCollection<IReplicatedService>> InReplicatedServices);

    void EndPlay(EEndPlayReason::Type InReason) override;

private:
    UPROPERTY()
    TArray<TObjectPtr<UObject>> Services;
};
