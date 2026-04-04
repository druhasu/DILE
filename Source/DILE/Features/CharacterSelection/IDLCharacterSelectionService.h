// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLAssetId.h"

#include "IDLCharacterSelectionService.generated.h"

class UDLItemType_Character;

UINTERFACE(MinimalApi)
class UDLCharacterSelectionService : public UInterface
{
    GENERATED_BODY()
};

/*
 * Allows selection of Characters in Lobby
 */
class DILE_API IDLCharacterSelectionService
{
    GENERATED_BODY()

public:
    using FChangedSignature = TMulticastDelegate<void(const TDLAssetId<UDLItemType_Character>& NewSelectedCharacter)>;

    /* Currently selected Character */
    virtual TDLAssetId<UDLItemType_Character> GetSelectedCharacter() = 0;

    /* Selects new Character */
    virtual void SetSelectedCharacter(TDLAssetId<UDLItemType_Character> AssetId) = 0;

    /* Called when selected Character changes */
    virtual FChangedSignature& OnSelectedCharacterChanged() = 0;
};
