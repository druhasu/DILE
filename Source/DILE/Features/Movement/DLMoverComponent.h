// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "MoverComponent.h"

#include "DLMoverComponent.generated.h"

UCLASS()
class DILE_API UDLMoverComponent : public UMoverComponent
{
    GENERATED_BODY()

public:
    /* Whether we are falling. Helper for access in threadsafe animation update */
    UFUNCTION(BlueprintCallable)
    bool IsFalling() const;

    /* Whether we are on the ground. Helper for access in threadsafe animation update */
    UFUNCTION(BlueprintCallable)
    bool IsOnGround() const;
};
