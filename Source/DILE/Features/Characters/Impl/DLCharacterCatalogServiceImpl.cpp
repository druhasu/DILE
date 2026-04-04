// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLCharacterCatalogServiceImpl.h"
#include "Features/Characters/DLItemType_Character.h"
#include "Utils/DLEnsure.h"

#include "Engine/AssetManager.h"

void UDLCharacterCatalogServiceImpl::InitDependencies()
{
    TArray<FPrimaryAssetId> AssetIDs;
    UAssetManager::Get().GetPrimaryAssetIdList(TDLAssetId<UDLItemType_Character>::GetAssetType(), AssetIDs);

    Algo::Transform(AssetIDs, AllCharacters, [](auto& Id) { return TDLAssetId<UDLItemType_Character>(Id); });
}

TDLTask<UDLItemType_Character*> UDLCharacterCatalogServiceImpl::GetCharacterData(const TDLAssetId<UDLItemType_Character>& CharacterId, const TArray<FName>& LoadBundles) const
{
    DL_ENSURE_CORETURN(CharacterId.IsValid(), nullptr);

    UDLItemType_Character* Result = co_await UE5Coro::Latent::AsyncLoadPrimaryAsset<UDLItemType_Character>(CharacterId.GetAssetId(), LoadBundles);
    co_return Result;
}
