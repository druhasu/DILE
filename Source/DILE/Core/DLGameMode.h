// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Systems/GameModeEvents/IDLGameModeEventsService.h"

#include "DLGameMode.generated.h"

class UDLGameModeConfigurationAsset;

/**
 *  Simple GameMode for a first person game
 */
UCLASS(Abstract)
class ADLGameMode : public AGameModeBase, public IDLGameModeEventsService
{
    GENERATED_BODY()

public:
    ADLGameMode();

    FPreLoginSignature& OnPreLogin() override { return PreLoginDelegate; }
    FPostLoginSignature& OnPostLogin() override { return PostLoginDelegate; }
    FLogoutSignature& OnLogout() override { return LogoutDelegate; }

    void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
    void PostLogin(APlayerController* NewPlayer) override;
    void Logout(AController* Exiting) override;
    void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    const UDLGameModeConfigurationAsset* GetConfiguration() const { return Configuration; }

protected:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UDLGameModeConfigurationAsset> Configuration;

private:
    void ForceReplicateActor(APlayerController* NewPlayer, AActor* ActorToReplicate);

    FPreLoginSignature PreLoginDelegate;
    FPostLoginSignature PostLoginDelegate;
    FLogoutSignature LogoutDelegate;
};
