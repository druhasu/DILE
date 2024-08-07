// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/MapLoad/IMapLoadService.h"
#include "Core/DILESettings.h"
#include "MapLoadServiceImpl.generated.h"

class UMapLoadServiceSettings;

/*
 * Implementation of IMapLoadService using LoadMap functions
 */
UCLASS()
class DILE_API UMapLoadServiceImpl : public UObject, public IMapLoadService
{
    GENERATED_BODY()

public:
    void InitDependencies(UMapLoadServiceSettings* InMapsSettings);

    void OpenMainMenuLevel() override;
    void OpenGameLevel() override;

private:
    UPROPERTY()
    TObjectPtr<UMapLoadServiceSettings> Settings;
};

UCLASS()
class DILE_API UMapLoadServiceSettings : public UDILESettings
{
    GENERATED_BODY()

public:
    /* The map that will be loaded as Main Menu level */
    UPROPERTY(Config, EditAnywhere)
    TSoftObjectPtr<UWorld> MainMenuMap;

    /* The map that will be loaded as Game level */
    UPROPERTY(Config, EditAnywhere)
    TSoftObjectPtr<UWorld> GameMap;
};
