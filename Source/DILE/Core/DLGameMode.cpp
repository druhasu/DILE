// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLGameMode.h"
#include "DLGameState.h"
#include "DLPlayerController.h"
#include "DLPlayerState.h"
#include "DLPawn.h"
#include "Utils/DLEnsure.h"

#include "Engine/ActorChannel.h"
#include "Engine/NetConnection.h"

ADLGameMode::ADLGameMode()
{
    GameStateClass = ADLGameState::StaticClass();
    PlayerControllerClass = ADLPlayerController::StaticClass();
    PlayerStateClass = ADLPlayerState::StaticClass();
    DefaultPawnClass = ADLPawn::StaticClass();
}

void ADLGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
    PreLoginDelegate.Broadcast(Options, Address, UniqueId, ErrorMessage);

    if (ErrorMessage.IsEmpty())
    {
        Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
    }
}

void ADLGameMode::PostLogin(APlayerController* NewPlayer)
{
    // for proper container initialization on clients we need to replicate GameState before PlayerController
    // PlayerController is replicated from Super::PostLogin with first sent RPC
    ForceReplicateActor(NewPlayer, GameState);

    Super::PostLogin(NewPlayer);
    PostLoginDelegate.Broadcast(NewPlayer);

    APlayerState* NewPlayerState = NewPlayer->PlayerState;
    for (APlayerState* State : GameState->PlayerArray)
    {
        ForceReplicateActor(State->GetPlayerController(), NewPlayerState);
        ForceReplicateActor(NewPlayer, State);
    }
}

void ADLGameMode::Logout(AController* Exiting)
{
    LogoutDelegate.Broadcast(Exiting);
    Super::Logout(Exiting);
}

void ADLGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    // skip default logic for spawning a pawn for new player
}

void ADLGameMode::ForceReplicateActor(APlayerController* NewPlayer, AActor* ActorToReplicate)
{
    DL_ENSURE_RETURN(NewPlayer != nullptr);
    DL_ENSURE_RETURN(ActorToReplicate != nullptr);

    if (UNetConnection* Connection = NewPlayer->GetNetConnection())
    {
        UActorChannel* Channel = Connection->FindActorChannelRef(ActorToReplicate);
        if (Channel == nullptr)
        {
            Channel = Cast<UActorChannel>(Connection->CreateChannelByName(NAME_Actor, EChannelCreateFlags::OpenedLocally));
            if (!ensure(Channel != nullptr))
            {
                return;
            }

            Channel->SetChannelActor(ActorToReplicate, ESetChannelActorFlags::None);
        }
        Channel->ReplicateActor();
    }
}
