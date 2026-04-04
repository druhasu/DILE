// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLUserWidget.h"
#include "IDLClosableWidget.h"

#include "DLRootWidget.generated.h"

UCLASS()
class DILE_API UDLRootWidget : public UDLUserWidget, public IDLClosableWidget
{
    GENERATED_BODY()

public:
    UDLRootWidget();

    FCloseRequestSignature& OnCloseRequested() override { return CloseRequestedDelegate; }

protected:
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Close"))
    void K2_Close() { Close(); }

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnClose"))
    void K2_OnClose();

private:
    FCloseRequestSignature CloseRequestedDelegate;
};
