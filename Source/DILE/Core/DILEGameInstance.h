// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "DILEGameInstance.generated.h"

class UObjectContainer;
class FObjectContainerBuilder;

/*
 * GameInstance responsible for creating DI container
 */
UCLASS()
class UDILEGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    void Init() override;
    void Shutdown() override;

    void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;

    TObjectPtr<UObjectContainer> GetContainer() const { return Container; }

private:
    void CreateContainer();
    void ConfigureContainer(FObjectContainerBuilder& Builder);

    // Global DI container
    UPROPERTY()
    TObjectPtr<UObjectContainer> Container;
};