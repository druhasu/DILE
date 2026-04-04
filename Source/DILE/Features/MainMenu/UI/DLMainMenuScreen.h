// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Utils/Widgets/DLUserWidget.h"
#include "Mvvm/BaseView.h"
#include "DLMainMenuViewModel.h"

#include "DLMainMenuScreen.generated.h"

UCLASS()
class UDLMainMenuScreen : public UDLUserWidget, public TBaseView<UDLMainMenuScreen, UDLMainMenuViewModel>
{
    GENERATED_BODY()

public:
    void InitDependencies(UDLMainMenuViewModel* InViewModel);
};
