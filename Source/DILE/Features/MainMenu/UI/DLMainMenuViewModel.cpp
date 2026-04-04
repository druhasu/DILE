// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLMainMenuViewModel.h"
#include "Systems/MapLoad/IDLMapLoadService.h"

void UDLMainMenuViewModel::InitDependencies(TScriptInterface<IDLMapLoadService>&& InMapLoadService)
{
    // save MapLoadService to use it later
    MapLoadService = InMapLoadService;
}

void UDLMainMenuViewModel::StartPlaying()
{
    // Ask MapLoadService to load a map for us
    MapLoadService->OpenGameLevel();
}
