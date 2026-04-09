// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLAssetId.h"
#include "Utils/DLTask.h"

#include "IDLCharacterCatalogService.generated.h"

class UDLItemType_Character;

UINTERFACE(MinimalApi)
class UDLCharacterCatalogService : public UInterface
{
    GENERATED_BODY()
};

/*
 * Provides info about all existing characters
 */
class DILE_API IDLCharacterCatalogService
{
    GENERATED_BODY()

public:
    /* List of all characters */
    virtual const TArray<TDLAssetId<UDLItemType_Character>>& GetCharacters() const = 0;

    /* Loads data of a character with given AssetId */
    virtual TDLTask<UDLItemType_Character*> GetCharacterData(const TDLAssetId<UDLItemType_Character>& Character, const TArray<FName>& LoadBundles = {}) const = 0;
};
