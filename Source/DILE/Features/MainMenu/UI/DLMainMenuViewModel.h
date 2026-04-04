// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Mvvm/BaseViewModel.h"

#include "DLMainMenuViewModel.generated.h"

class IDLMapLoadService;

UCLASS()
class DILE_API UDLMainMenuViewModel : public UBaseViewModel
{
    GENERATED_BODY()

public:
    void InitDependencies(TScriptInterface<IDLMapLoadService>&& InMapLoadService);

    UFUNCTION(BlueprintCallable)
    void StartPlaying();

private:
    UPROPERTY()
    TScriptInterface<IDLMapLoadService> MapLoadService;
};
