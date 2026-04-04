// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLCharacterSelectionServiceImpl.h"
#include "Features/Characters/IDLCharacterCatalogService.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

void UDLCharacterSelectionServiceImpl::InitDependencies(
    TScriptInterface<IDLCharacterCatalogService> InCharacterCatalog
)
{
    CharacterCatalog = InCharacterCatalog;
}

void UDLCharacterSelectionServiceImpl::SetSelectedCharacter(TDLAssetId<UDLItemType_Character> AssetId)
{
    Server_SetSelectedCharacter(AssetId);
}

void UDLCharacterSelectionServiceImpl::OnStartup()
{
    APlayerController* Controller = GetWorld()->GetFirstPlayerController();
    APlayerState* PlayerState = GetTypedOuter<APlayerState>();

    if (Controller->GetPlayerState<APlayerState>() == PlayerState)
    {
        const TArray<TDLAssetId<UDLItemType_Character>>& Characters = CharacterCatalog->GetCharacters();
        if (Characters.Num() > 0)
        {
            SetSelectedCharacter(Characters[/*FMath::RandRange(0, Characters.Num() - 1)*/0]);
        }
    }
}

void UDLCharacterSelectionServiceImpl::Server_SetSelectedCharacter_Implementation(TDLAssetId<UDLItemType_Character> InCharacter)
{
    if (SelectedCharacter != InCharacter)
    {
        SelectedCharacter = InCharacter;
        SelectedCharacterChangedDelegate.Broadcast(SelectedCharacter);
    }
}
