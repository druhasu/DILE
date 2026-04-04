// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Respawn/IDLRespawnService.h"
#include "Utils/DLTask.h"
#include "Utils/DLAssetId.h"

#include "DLRespawnServiceImpl.generated.h"

class IDLObjectTrackerService;
class IDLGameModeEventsService;
class IDLStreamableService;
class APlayerController;
class UDLItemType_Character;

UCLASS()
class DILE_API UDLRespawnServiceImpl : public UObject, public IDLRespawnService
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLObjectTrackerService> InObjectTracker,
        TScriptInterface<IDLGameModeEventsService> InGameModeEvents,
        TScriptInterface<IDLStreamableService> InStreamableService
    );

    // Begin IDLRespawnService

    // End IDLRespawnService

private:
    void OnPlayerPostLogin(APlayerController* Controller);
    void OnSelectedCharacterChanged(const TDLAssetId<UDLItemType_Character>& NewCharacter, APlayerController* Controller);
    FDLTask RespawnPlayerByID(TDLAssetId<UDLItemType_Character> NewCharacter, APlayerController* Controller);
    void RespawnPlayer(APlayerController* Controller, UClass* PawnClass);

    static TDLAssetId<UDLItemType_Character> GetSelectedCharacter(APlayerController* Controller);

private:
    UPROPERTY()
    TScriptInterface<IDLObjectTrackerService> ObjectTracker;

    UPROPERTY()
    TScriptInterface<IDLGameModeEventsService> GameModeEvents;

    UPROPERTY()
    TScriptInterface<IDLStreamableService> StreamableService;

    TArray<TWeakObjectPtr<APlayerController>> Players;
};
