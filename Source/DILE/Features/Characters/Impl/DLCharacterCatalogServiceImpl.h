// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Characters/IDLCharacterCatalogService.h"

#include "DLCharacterCatalogServiceImpl.generated.h"

UCLASS()
class DILE_API UDLCharacterCatalogServiceImpl : public UObject, public IDLCharacterCatalogService
{
    GENERATED_BODY()

public:
    void InitDependencies();

    // Begin IDLCharacterCatalogService
    const TArray<TDLAssetId<UDLItemType_Character>>& GetCharacters() const override { return AllCharacters; }
    TDLTask<UDLItemType_Character*> GetCharacterData(const TDLAssetId<UDLItemType_Character>& Character, const TArray<FName>& LoadBundles) const override;
    // End IDLCharacterCatalogService

private:
    TArray<TDLAssetId<UDLItemType_Character>> AllCharacters;
};
