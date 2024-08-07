// Copyright Andrei Sudarikov. All Rights Reserved.

#include "MainMenuViewModel.h"
#include "Systems/MapLoad/IMapLoadService.h"

void UMainMenuViewModel::InitDependencies(TScriptInterface<IMapLoadService>&& InMapLoadService)
{
    // save MapLoadService to use it later
    MapLoadService = InMapLoadService;
}

void UMainMenuViewModel::StartPlaying()
{
    // Ask MapLoadService to load a map for us
    MapLoadService->OpenGameLevel();
}
