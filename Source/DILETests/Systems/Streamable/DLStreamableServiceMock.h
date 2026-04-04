// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/Streamable/IDLStreamableService.h"

#include "DLStreamableServiceMock.generated.h"

UCLASS()
class UDLStreamableServiceMock : public UObject, public IDLStreamableService
{
    GENERATED_BODY()

public:
    TDLTask<TArray<UObject*>> AsyncLoad(TArray<FSoftObjectPath> Paths, TAsyncLoadPriority Priority) override
    {
        return TDLTask<TArray<UObject*>>::FromResult({});
    }

    TDLTask<UObject*> AsyncLoad(const FSoftObjectPath& Path, TAsyncLoadPriority Priority) override
    {
        return TDLTask<UObject*>::FromResult({});
    }

    TDLTask<TArray<UObject*>> AsyncLoad(TArray<FPrimaryAssetId> AssetIds, TAsyncLoadPriority Priority) override
    {
        return TDLTask<TArray<UObject*>>::FromResult({});
    }

    TDLTask<UObject*> AsyncLoad(const FPrimaryAssetId& AssetId, TAsyncLoadPriority Priority) override
    {
        return TDLTask<UObject*>::FromResult({});
    }
};

