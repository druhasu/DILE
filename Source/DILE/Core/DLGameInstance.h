// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "Systems/Lifecycle/Impl/DLStartupShutdownHelper.h"

#include "DLGameInstance.generated.h"

class UObjectContainer;

/*
 * GameInstance responsible for creating DI container
 */
UCLASS()
class UDLGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    void Init() override;
    void Shutdown() override;

    void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;

    UObjectContainer* GetContainer() const { return Container; }

private:
    void InitContainer();

private:
    UPROPERTY()
    TObjectPtr<UObjectContainer> Container;

    UPROPERTY()
    FDLStartupShutdownHelper StartupShutdownHelper;
};
