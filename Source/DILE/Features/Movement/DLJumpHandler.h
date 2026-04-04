// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLJumpHandler.generated.h"

class UMoverComponent;

UCLASS()
class DILE_API UDLJumpHandler : public UObject
{
    GENERATED_BODY()

public:
    void InitDependencies(UMoverComponent* InMoverComponent);

private:
    UFUNCTION()
    void OnPreSimulationTick(const FMoverTimeStep& TimeStep, const FMoverInputCmdContext& InputCmd);

private:
    UPROPERTY()
    TObjectPtr<UMoverComponent> MoverComponent;
};
