// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/MapLoad/IDLMapLoadService.h"
#include "Core/DLSettings.h"
#include "DLMapLoadServiceImpl.generated.h"

class UDLMapLoadServiceSettings;

/*
 * Implementation of IDLMapLoadService using LoadMap functions
 */
UCLASS()
class DILE_API UDLMapLoadServiceImpl : public UObject, public IDLMapLoadService
{
    GENERATED_BODY()

public:
    void InitDependencies(UDLMapLoadServiceSettings* InMapsSettings);

    void OpenMainMenuLevel() override;
    void OpenGameLevel() override;

private:
    UPROPERTY()
    TObjectPtr<UDLMapLoadServiceSettings> Settings;
};

UCLASS()
class DILE_API UDLMapLoadServiceSettings : public UDLSettings
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
