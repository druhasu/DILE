// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Core/DILEHUD.h"
#include "Blueprint/UserWidget.h"

void ADILEHUD::InitDependencies(TScriptInterface<IInjector>&& InInjector)
{
    // check if we have a widget to show at startup
    if (!StartupWidgetClass.IsNull())
    {
        // the most minimalistic "Add Widget to Screen" code
        UUserWidget* StartupWidget = CreateWidget(GetOwningPlayerController(), StartupWidgetClass.LoadSynchronous());

        // inject dependencies to newly created widget, calling its InitDependencies
        InInjector->Inject(StartupWidget);

        // add to viewport
        StartupWidget->AddToViewport();
    }
}

void ADILEHUD::BeginPlay()
{
    Super::BeginPlay();

    // this object is created by Unreal itself, so we cannot rely on DI container in calling InitDependencies
    // ask current World container to do it for us
    TryInitDependencies(this);
}
