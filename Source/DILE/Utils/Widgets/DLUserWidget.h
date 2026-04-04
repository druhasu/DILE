// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"

#include "DLUserWidget.generated.h"

UCLASS()
class DILE_API UDLUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    bool NeedsMouseInput() const { return bNeedsMouseInput; }

protected:
    void NativeDestruct() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Defaults")
    bool bNeedsMouseInput = false;
};
