// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/Streamable/IDLStreamableService.h"

#include "DLStreamableServiceImpl.generated.h"

UCLASS()
class UDLStreamableServiceImpl : public UObject, public IDLStreamableService
{
    GENERATED_BODY()

public:
    // Begin IDLStreamableService implementation
    TDLTask<TArray<UObject*>> AsyncLoad(TArray<FSoftObjectPath> Paths, TAsyncLoadPriority Priority) override;
    TDLTask<UObject*> AsyncLoad(const FSoftObjectPath& Path, TAsyncLoadPriority Priority = DefaultAsyncLoadPriority) override;
    TDLTask<TArray<UObject*>> AsyncLoad(TArray<FPrimaryAssetId> AssetIds, TAsyncLoadPriority Priority = DefaultAsyncLoadPriority) override;
    TDLTask<UObject*> AsyncLoad(const FPrimaryAssetId& AssetId, TAsyncLoadPriority Priority = DefaultAsyncLoadPriority) override;
    //~End IDLStreamableService implementation
};
