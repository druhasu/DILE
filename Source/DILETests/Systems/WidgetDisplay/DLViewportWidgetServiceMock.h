// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/WidgetDisplay/IDLViewportWidgetService.h"

#include "DLViewportWidgetServiceMock.generated.h"

UCLASS()
class UDLViewportWidgetServiceMock : public UObject, public IDLViewportWidgetService
{
    GENERATED_BODY()

public:
    // Begin IViewportWidgetService implementation
    void AddViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent, const int32 ZOrder = 0) override;
    void RemoveViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent) override;
    //~End IViewportWidgetService implementation

    struct FEntry
    {
        TSharedPtr<SWidget> Widget;
        int32 ZOrder;
    };

    TArray<FEntry> ViewportContents;
};

inline void UDLViewportWidgetServiceMock::AddViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent, const int32 ZOrder)
{
    ViewportContents.Add({ViewportContent, ZOrder});
}

inline void UDLViewportWidgetServiceMock::RemoveViewportWidgetContent(const TSharedRef<SWidget>& ViewportContent)
{
    ViewportContents.RemoveAll([&](const FEntry& Entry) { return Entry.Widget.Get() == &ViewportContent.Get(); });
}
