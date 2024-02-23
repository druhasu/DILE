// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Services/MapLoadServiceImpl.h"
#include "GameMapsSettings.h"
#include "Kismet/GameplayStatics.h"

void UMapLoadServiceImpl::InitDependencies(UMapLoadServiceSettings* InMapsSettings)
{
	Settings = InMapsSettings;
}

void UMapLoadServiceImpl::OpenMainMenuLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Settings->MainMenuMap);
}

void UMapLoadServiceImpl::OpenGameLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Settings->GameMap);
}
