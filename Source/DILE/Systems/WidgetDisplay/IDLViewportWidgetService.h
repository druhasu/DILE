// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLViewportWidgetService.generated.h"

class SWidget;

UINTERFACE(MinimalApi)
class UDLViewportWidgetService : public UInterface
{
    GENERATED_BODY()
};

/*
 * Allows to add Widgets directly to Viewport. Intended to hide direct dependency on UGameViewportClient
 */
class DILE_API IDLViewportWidgetService
{
    GENERATED_BODY()

public:
    /* Adds widget to Viewport. Does same as UGameViewportClient::AddViewportWidgetContent */
    virtual void AddViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent, const int32 ZOrder = 0) = 0;

    /* Removes widget from Viewport */
    virtual void RemoveViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent) = 0;
};
