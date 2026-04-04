// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Mvvm/BaseViewModel.h"
#include "DLCharacterViewModel.h"
#include "Utils/DLTask.h"

#include "DLCharacterSelectionViewModel.generated.h"

class IDLCharacterSelectionService;
class IDLCharacterCatalogService;

UCLASS()
class DILE_API UDLCharacterSelectionViewModel : public UBaseViewModel
{
    GENERATED_BODY()

    VM_PROP_AG_AS(bool, Loading);
    VM_PROP_AG_AS(const TArray<TObjectPtr<UDLCharacterViewModel>>&, Characters);
    VM_PROP_AG_AS(TObjectPtr<UDLCharacterViewModel>, SelectedCharacter, public);

public:
    void InitDependencies(
        TScriptInterface<IDLCharacterCatalogService> InCharacterCatalogueService,
        TScriptInterface<IDLCharacterSelectionService> InCharacterSelectionService
    );

    UFUNCTION(BlueprintCallable)
    void ApplySelection();

private:
    FDLTask InitCharacters();

private:
    UPROPERTY()
    TScriptInterface<IDLCharacterCatalogService> CharacterCatalogService;

    UPROPERTY()
    TScriptInterface<IDLCharacterSelectionService> CharacterSelectionService;
};
