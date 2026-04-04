// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLCharacterViewModel.h"
#include "Features/Characters/DLItemType_Character.h"
#include "Utils/DLEnsure.h"

void UDLCharacterViewModel::SetModel(UDLItemType_Character* InCharacterAsset)
{
    DL_ENSURE_RETURN(InCharacterAsset != nullptr);

    CharacterAsset = InCharacterAsset;

    SetCharacterID(TDLAssetId<UDLItemType_Character>(InCharacterAsset));
    SetCharacterName(InCharacterAsset->Common.Name);
    SetPortrait(InCharacterAsset->Character.Portrait.Get());
}
