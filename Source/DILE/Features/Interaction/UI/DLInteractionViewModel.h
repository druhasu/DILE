// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Mvvm/BaseViewModel.h"

#include "DLInteractionViewModel.generated.h"

class IDLInteractionService;
class UDLInteractionDefinitionAsset;

UCLASS()
class UDLInteractionViewModel : public UBaseViewModel
{
    GENERATED_BODY()

    VM_PROP_MG_NF(bool, IsAvailable);
    VM_PROP_AG_AS(FText, InteractionTitle);

public:
    void InitDependencies(
        TScriptInterface<IDLInteractionService> InInteractionService
    );

    UFUNCTION(BlueprintCallable)
    void Interact();

private:
    void OnInteractionChanged(const UDLInteractionDefinitionAsset* NewInteraction);

private:
    UPROPERTY()
    TScriptInterface<IDLInteractionService> InteractionService;
};
