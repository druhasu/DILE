// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInteractionAction_DisplayWidget.h"
#include "Systems/ContentPresentation/DLViewContent.h"
#include "Systems/WidgetDisplay/IDLWidgetDisplayService.h"
#include "Utils/DLEnsure.h"
#include "Utils/Widgets/IDLClosableWidget.h"
#include "Utils/Widgets/DLUserWidget.h"

void UDLInteractionAction_DisplayWidget::InitDependencies(
    TScriptInterface<IDLWidgetDisplayService> InWidgetDisplayService
)
{
    WidgetDisplayService = InWidgetDisplayService;
}

FDLTask UDLInteractionAction_DisplayWidget::ExecuteAsync(TScriptInterface<IDLInteractable> Interactable) const
{
    DL_ENSURE_CORETURN(WidgetSlot.IsValid());
    DL_ENSURE_CORETURN(WidgetClass != nullptr);

    UDLUserWidget* Widget = co_await WidgetDisplayService->DisplayWidget(WidgetSlot, WidgetClass, FDLViewContent(Interactable.GetObject()));
    DL_ENSURE_CORETURN(Widget != nullptr);

    IDLClosableWidget* ClosableWidget = Cast<IDLClosableWidget>(Widget);
    if (ClosableWidget != nullptr)
    {
        co_await ClosableWidget->OnCloseRequested();
    }
}
