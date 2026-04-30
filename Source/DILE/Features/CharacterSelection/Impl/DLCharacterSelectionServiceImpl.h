// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/CharacterSelection/IDLCharacterSelectionService.h"
#include "Systems/Lifecycle/IDLStartupListener.h"
#include "Systems/ServiceReplication/DLReplicatedService.h"

#include "DLCharacterSelectionServiceImpl.generated.h"

class IDLCharacterCatalogService;

UCLASS()
class DILE_API UDLCharacterSelectionServiceImpl : public UDLReplicatedServiceBase, public IDLCharacterSelectionService, public IDLStartupListener
{
    GENERATED_BODY()

public:
    void InitDependencies(
        TScriptInterface<IDLCharacterCatalogService> InCharacterCatalogue
    );

    ELifetimeCondition GetReplicationCondition() const override { return ELifetimeCondition::COND_OwnerOnly; }

    // Begin IDLCharacterSelectionService
    TDLAssetId<UDLItemType_Character> GetSelectedCharacter() override { return SelectedCharacter; }
    void SetSelectedCharacter(TDLAssetId<UDLItemType_Character> AssetId) override;
    FChangedSignature& OnSelectedCharacterChanged() override { return SelectedCharacterChangedDelegate; }
    // End IDLCharacterSelectionService

    // Begin IDLStartupListener
    void OnStartup() override;
    // End IDLStartupListener

private:
    UFUNCTION(Server, Reliable)
    void Server_SetSelectedCharacter(TDLAssetId<UDLItemType_Character> InCharacter);

private:
    UPROPERTY()
    TScriptInterface<IDLCharacterCatalogService> CharacterCatalog;

    UPROPERTY()
    TDLAssetId<UDLItemType_Character> SelectedCharacter;

    FChangedSignature SelectedCharacterChangedDelegate;
};
