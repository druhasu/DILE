// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInitialWidgetsDisplayer.h"
#include "Systems/ContentPresentation/DLViewContent.h"
#include "Systems/WidgetDisplay/IDLWidgetDisplayService.h"

void UDLInitialWidgetDisplayer::InitDependencies(
    TScriptInterface<IDLWidgetDisplayService> InWidgetDisplayService
)
{
    WidgetDisplayService = InWidgetDisplayService;
}

void UDLInitialWidgetDisplayer::OnStartup()
{
    for (const FDLInitialHUDWidget& Entry : InitialWidgets)
    {
        WidgetDisplayService->DisplayWidget(Entry.Slot, Entry.Class, {});
    }
}
