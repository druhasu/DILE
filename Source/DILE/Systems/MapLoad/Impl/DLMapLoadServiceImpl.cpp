// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLMapLoadServiceImpl.h"

#include "GameMapsSettings.h"
#include "Kismet/GameplayStatics.h"

void UDLMapLoadServiceImpl::InitDependencies(UDLMapLoadServiceSettings* InMapsSettings)
{
    Settings = InMapsSettings;
}

void UDLMapLoadServiceImpl::OpenMainMenuLevel()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, Settings->MainMenuMap);
}

void UDLMapLoadServiceImpl::OpenGameLevel()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, Settings->GameMap);
}
