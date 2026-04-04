// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLRespawnServiceImpl.h"
#include "DLRespawnPoint.h"
#include "Core/DLPawn.h"
#include "Features/Characters/DLItemType_Character.h"
#include "Features/CharacterSelection/IDLCharacterSelectionService.h"
#include "Systems/GameModeEvents/IDLGameModeEventsService.h"
#include "Systems/ObjectTracking/IDLObjectTrackerService.h"
#include "Systems/Streamable/IDLStreamableService.h"
#include "Utils/DLEnsure.h"
#include "Utils/DLInterfaceStatics.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

void UDLRespawnServiceImpl::InitDependencies(
    TScriptInterface<IDLObjectTrackerService> InObjectTracker,
    TScriptInterface<IDLGameModeEventsService> InGameModeEvents,
    TScriptInterface<IDLStreamableService> InStreamableService
)
{
    GameModeEvents = InGameModeEvents;
    ObjectTracker = InObjectTracker;
    StreamableService = InStreamableService;

    GameModeEvents->OnPostLogin().AddUObject(this, &ThisClass::OnPlayerPostLogin);
}

void UDLRespawnServiceImpl::OnPlayerPostLogin(APlayerController* Controller)
{
    Players.Add(Controller);

    TScriptInterface<IDLCharacterSelectionService> CharacterSelection = GetInterfaceFromObject<IDLCharacterSelectionService>(Controller->GetPlayerState<APlayerState>());

    CharacterSelection->OnSelectedCharacterChanged().AddUObject(this, &ThisClass::OnSelectedCharacterChanged, Controller);
    OnSelectedCharacterChanged(CharacterSelection->GetSelectedCharacter(), Controller);
}

void UDLRespawnServiceImpl::OnSelectedCharacterChanged(const TDLAssetId<UDLItemType_Character>& NewCharacter, APlayerController* Controller)
{
    if (!NewCharacter.IsValid())
    {
        // wait for valid character
        return;
    }

    RespawnPlayerByID(NewCharacter, Controller);
}

FDLTask UDLRespawnServiceImpl::RespawnPlayerByID(TDLAssetId<UDLItemType_Character> NewCharacter, APlayerController* Controller)
{
    UDLItemType_Character* CharacterType = co_await UE5Coro::Latent::AsyncLoadPrimaryAsset<UDLItemType_Character>(NewCharacter.GetAssetId(), { "Client" });
    UClass* PawnClass = CharacterType->Character.PawnClass.Get();
    DL_ENSURE_CORETURN(PawnClass != nullptr);

    // make sure that player has still the same character selected as when we started loading
    if (GetSelectedCharacter(Controller) == NewCharacter)
    {
        RespawnPlayer(Controller, PawnClass);
    }
}

void UDLRespawnServiceImpl::RespawnPlayer(APlayerController* Controller, UClass* PawnClass)
{
    FVector Location(0);
    FRotator Rotation(0);

    ADLRespawnPoint* FoundPoint = nullptr;
    int32 PlayerIndex = Players.IndexOfByKey(Controller);
    ObjectTracker->ForEachObject<ADLRespawnPoint>([&](ADLRespawnPoint* Point)
    {
        if (Point->GetAssignedPlayerIndex() == PlayerIndex)
        {
            FoundPoint = Point;
        }
        return FoundPoint == nullptr;
    });

    if (FoundPoint != nullptr)
    {
        Location = FoundPoint->GetActorLocation();
        Rotation = FoundPoint->GetActorRotation();
        Rotation.Roll = 0;
    }

    APawn* OldPawn = Controller->GetPawn();

    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, Location, Rotation);
    Controller->SetPawn(NewPawn);
    Controller->Possess(NewPawn);

    // Set initial control rotation to starting rotation rotation
    Controller->ClientSetRotation(Rotation, true);
    Controller->SetControlRotation(Rotation);

    if (OldPawn != nullptr)
    {
        OldPawn->Destroy();
    }
}

TDLAssetId<UDLItemType_Character> UDLRespawnServiceImpl::GetSelectedCharacter(APlayerController* Controller)
{
    APlayerState* PlayerState = Controller->GetPlayerState<APlayerState>();
    return GetInterfaceFromObject<IDLCharacterSelectionService>(PlayerState)->GetSelectedCharacter();
}
