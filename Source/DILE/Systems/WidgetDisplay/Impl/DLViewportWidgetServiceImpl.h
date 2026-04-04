// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/Lifecycle/IDLShutdownListener.h"
#include "Systems/WidgetDisplay/IDLViewportWidgetService.h"

#include "DLViewportWidgetServiceImpl.generated.h"

UCLASS()
class UDLViewportWidgetServiceImpl : public UObject, public IDLViewportWidgetService, public IDLShutdownListener
{
    GENERATED_BODY()

public:
    void InitDependencies();

    // Begin IDLViewportWidgetService implementation
    void AddViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent, const int32 ZOrder = 0) override;
    void RemoveViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent) override;
    //~End IDLViewportWidgetService implementation

    // Begin IDLShutdownListener
    void OnShutdown() override;
    // End IDLShutdownListener

private:
    TWeakObjectPtr<UGameViewportClient> GameViewportClient;
    TArray<TSharedRef<SWidget>> AddedWidgets;
};
