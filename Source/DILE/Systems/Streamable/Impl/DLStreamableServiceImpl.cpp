// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLStreamableServiceImpl.h"
#include "Engine/AssetManager.h"

TDLTask<TArray<UObject*>> UDLStreamableServiceImpl::AsyncLoad(TArray<FSoftObjectPath> Paths, TAsyncLoadPriority Priority)
{
    co_await UE5Coro::Latent::AsyncLoadObjects(Paths, Priority);
    TArray<UObject*> Result;
    Result.Reserve(Paths.Num());
    Algo::Transform(Paths, Result, [](const FSoftObjectPath& Path) { return Path.ResolveObject(); });
    co_return Result;
}

TDLTask<UObject*> UDLStreamableServiceImpl::AsyncLoad(const FSoftObjectPath& Path, TAsyncLoadPriority Priority)
{
    co_return co_await UE5Coro::Latent::AsyncLoadObject<UObject>(TSoftObjectPtr<UObject>(Path), Priority);
}

TDLTask<TArray<UObject*>> UDLStreamableServiceImpl::AsyncLoad(TArray<FPrimaryAssetId> AssetIds, TAsyncLoadPriority Priority)
{
    co_return co_await UE5Coro::Latent::AsyncLoadPrimaryAssets<UObject>(MoveTemp(AssetIds));
}

TDLTask<UObject*> UDLStreamableServiceImpl::AsyncLoad(const FPrimaryAssetId& AssetId, TAsyncLoadPriority Priority)
{
    co_return co_await UE5Coro::Latent::AsyncLoadPrimaryAsset<UObject>(AssetId);
}
