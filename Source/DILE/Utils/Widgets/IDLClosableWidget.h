// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLClosableWidget.generated.h"

class UDLUserWidget;

UINTERFACE(MinimalApi)
class UDLClosableWidget : public UInterface { GENERATED_BODY() };

/*
 * Interface to a widget that can be Closed
 */
class DILE_API IDLClosableWidget
{
    GENERATED_BODY()

public:
    using FCloseRequestSignature = TMulticastDelegate<void(UDLUserWidget* Widget)>;

    /* Closes the widget removing it from its parent */
    void Close()
    {
        OnClose();
        OnCloseRequested().Broadcast(Cast<UDLUserWidget>(_getUObject()));
    }

    /* Called when widget needs to be closed */
    virtual FCloseRequestSignature& OnCloseRequested() = 0;

protected:
    /* Called when widget is being closed */
    virtual void OnClose() {}
};
