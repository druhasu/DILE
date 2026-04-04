// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLCharacterSelectionViewModel.h"
#include "Features/Characters/IDLCharacterCatalogService.h"
#include "Features/CharacterSelection/IDLCharacterSelectionService.h"

#include "Mvvm/MvvmUtils.h"

void UDLCharacterSelectionViewModel::InitDependencies(
    TScriptInterface<IDLCharacterCatalogService> InCharacterCatalogService,
    TScriptInterface<IDLCharacterSelectionService> InCharacterSelectionService
)
{
    CharacterCatalogService = InCharacterCatalogService;
    CharacterSelectionService = InCharacterSelectionService;

    InitCharacters();
}

void UDLCharacterSelectionViewModel::ApplySelection()
{
    if (SelectedCharacterField != nullptr)
    {
        CharacterSelectionService->SetSelectedCharacter(SelectedCharacterField->GetCharacterID());
    }
}

FDLTask UDLCharacterSelectionViewModel::InitCharacters()
{
    SetLoading(true);

    TArray<TDLTask<UDLItemType_Character*>> AssetTasks;
    Algo::Transform(CharacterCatalogService->GetCharacters(), AssetTasks, [&](const TDLAssetId<UDLItemType_Character>& ID)
    {
        return CharacterCatalogService->GetCharacterData(ID, { "UI" });
    });

    TArray<UDLItemType_Character*> Assets;
    Assets.Reserve(AssetTasks.Num());

    for (auto& AssetTask : AssetTasks)
    {
        if (UDLItemType_Character* CharacterData = co_await AssetTask)
        {
            Assets.Emplace(CharacterData);
        }
    }

    MvvmUtils::SyncViewModelCollection(CharactersField, Assets);
    RaiseChanged(CharactersProperty());

    TDLAssetId<UDLItemType_Character> SelectedID = CharacterSelectionService->GetSelectedCharacter();
    TObjectPtr<UDLCharacterViewModel>* NewSelectedViewModel = CharactersField.FindByPredicate([&](UDLCharacterViewModel* ViewModel)
    {
        return ViewModel->GetCharacterID() == SelectedID;
    });

    if (NewSelectedViewModel != nullptr)
    {
        SetSelectedCharacter(*NewSelectedViewModel);
    }
    else if(CharactersField.Num() > 0)
    {
        SetSelectedCharacter(CharactersField[0]);
    }
    else
    {
        SetSelectedCharacter(nullptr);
    }

    SetLoading(false);
}
