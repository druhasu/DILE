// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UI/Common/DILEWidget.h"
#include "Mvvm/BaseView.h"
#include "MainMenuViewModel.h"
#include "MainMenuScreen.generated.h"

UCLASS()
class UMainMenuScreen : public UDILEWidget, public TBaseView<UMainMenuScreen, UMainMenuViewModel>
{
    GENERATED_BODY()

public:
    void InitDependencies(UMainMenuViewModel* InViewModel);
};
