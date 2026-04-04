// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLViewportWidgetServiceImpl.h"
#include "Utils/DLEnsure.h"

#include "Engine/GameViewportClient.h"

void UDLViewportWidgetServiceImpl::InitDependencies()
{
    GameViewportClient = GetWorld()->GetGameViewport();
}

void UDLViewportWidgetServiceImpl::AddViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent, const int32 ZOrder)
{
    UGameViewportClient* ViewportClient = GameViewportClient.Get();
    DL_ENSURE_RETURN(ViewportClient != nullptr);

    ViewportClient->AddViewportWidgetContent(ViewportContent, ZOrder);
    AddedWidgets.Add(ViewportContent);
}

void UDLViewportWidgetServiceImpl::RemoveViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent)
{
    UGameViewportClient* ViewportClient = GameViewportClient.Get();
    DL_ENSURE_RETURN(ViewportClient != nullptr);

    ViewportClient->RemoveViewportWidgetContent(ViewportContent);
    AddedWidgets.RemoveSingleSwap(ViewportContent);
}

void UDLViewportWidgetServiceImpl::OnShutdown()
{
    UGameViewportClient* ViewportClient = GameViewportClient.Get();
    DL_ENSURE_RETURN(ViewportClient != nullptr);

    for (auto& Widget : AddedWidgets)
    {
        ViewportClient->RemoveViewportWidgetContent(Widget);
    }

    AddedWidgets.Reset();
}
