// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DLUserWidget.h"
#include "IDLClosableWidget.h"

#include "DLRootWidget.generated.h"

/*
 * Base class for widgets that can be opened or closed by user.
 */
UCLASS()
class DILE_API UDLRootWidget : public UDLUserWidget, public IDLClosableWidget
{
    GENERATED_BODY()

public:
    UDLRootWidget();

    // Begin IDLClosableWidget
    FCloseRequestSignature& OnCloseRequested() override { return CloseRequestedDelegate; }
    // End IDLClosableWidget

protected:
    /* Closes the widget removing it from its parent */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Close"))
    void K2_Close() { Close(); }

    /* Called when widget is being closed */
    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnClose"))
    void K2_OnClose();

private:
    FCloseRequestSignature CloseRequestedDelegate;
};
