// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Mvvm/BaseViewModel.h"
#include "MainMenuViewModel.generated.h"

class IMapLoadService;

UCLASS()
class DILE_API UMainMenuViewModel : public UBaseViewModel
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IMapLoadService>&& InMapLoadService);

    UFUNCTION(BlueprintCallable)
    void StartPlaying();

private:
    UPROPERTY()
    TScriptInterface<IMapLoadService> MapLoadService;
};
