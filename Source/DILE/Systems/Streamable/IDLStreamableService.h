// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "Utils/DLTask.h"

#include "IDLStreamableService.generated.h"

struct FStreamableHandle;

UINTERFACE(MinimalApi)
class UDLStreamableService : public UInterface { GENERATED_BODY() };

/*
 * Manages streaming operations. Intended to hide direct dependency on FStreamableManager
 */
class DILE_API IDLStreamableService
{
    GENERATED_BODY()

public:
    using FStreamableDelegate = TDelegate<void()>;

    /* Loads several objects asynchronously */
    virtual TDLTask<TArray<UObject*>> AsyncLoad(TArray<FSoftObjectPath> Paths, TAsyncLoadPriority Priority = DefaultAsyncLoadPriority) = 0;

    /* Loads single object asynchronously */
    virtual TDLTask<UObject*> AsyncLoad(const FSoftObjectPath& Path, TAsyncLoadPriority Priority = DefaultAsyncLoadPriority) = 0;

    /* Loads several FPrimaryAssetIds asynchronously */
    virtual TDLTask<TArray<UObject*>> AsyncLoad(TArray<FPrimaryAssetId> AssetIds, TAsyncLoadPriority Priority = DefaultAsyncLoadPriority) = 0;

    /* Loads single FPrimaryAssetId asynchronously */
    virtual TDLTask<UObject*> AsyncLoad(const FPrimaryAssetId& AssetId, TAsyncLoadPriority Priority = DefaultAsyncLoadPriority) = 0;

    /* Default priority for all async loads */
    static const TAsyncLoadPriority DefaultAsyncLoadPriority = 0;

    /* Priority to try and load immediately */
    static const TAsyncLoadPriority AsyncLoadHighPriority = 100;
};
