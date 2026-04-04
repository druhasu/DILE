// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLClosableWidget.generated.h"

class UDLUserWidget;

UINTERFACE(MinimalApi)
class UDLClosableWidget : public UInterface { GENERATED_BODY() };

/*
 *
 */
class DILE_API IDLClosableWidget
{
    GENERATED_BODY()

public:
    using FCloseRequestSignature = TMulticastDelegate<void(UDLUserWidget* Widget)>;

    void Close()
    {
        OnClose();
        OnCloseRequested().Broadcast(Cast<UDLUserWidget>(_getUObject()));
    }

    virtual FCloseRequestSignature& OnCloseRequested() = 0;

protected:
    virtual void OnClose() {}
};
