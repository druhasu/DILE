// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"

#include "DLUserWidget.generated.h"

/*
 * Base class for all UserWidgets in the project
 * Use this one instead of inheriting directly from UUserWidget
 */
UCLASS()
class DILE_API UDLUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /* Whether this widget needs mouse input. If false, mouse cursor won't show */
    UFUNCTION(BlueprintCallable)
    bool NeedsMouseInput() const { return bNeedsMouseInput; }

protected:
    void NativeDestruct() override;

protected:
    /* Whether this widget needs mouse input. If false, mouse cursor won't show */
    UPROPERTY(EditDefaultsOnly, Category = "Defaults")
    bool bNeedsMouseInput = false;
};
