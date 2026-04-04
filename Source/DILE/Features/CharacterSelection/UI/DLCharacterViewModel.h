// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Mvvm/BaseViewModel.h"
#include "Utils/DLAssetId.h"

#include "DLCharacterViewModel.generated.h"

class UDLItemType_Character;

UCLASS()
class UDLCharacterViewModel : public UBaseViewModel
{
    GENERATED_BODY()

    VM_PROP_AG_AS(TDLAssetId<UDLItemType_Character>, CharacterID);
    VM_PROP_AG_AS(FText, CharacterName);
    VM_PROP_AG_AS(TObjectPtr<UTexture2D>, Portrait);

public:
    void SetModel(UDLItemType_Character* InCharacterAsset);

private:
    UPROPERTY()
    TObjectPtr<UDLItemType_Character> CharacterAsset;
};
