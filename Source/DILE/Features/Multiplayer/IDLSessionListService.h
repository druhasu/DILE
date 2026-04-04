// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "DLSessionHandle.h"
#include "Utils/DLTask.h"

#include "IDLSessionListService.generated.h"

USTRUCT()
struct FDLSessionListEntry
{
    GENERATED_BODY()

public:
    FDLSessionHandle Handle;

    UPROPERTY()
    FText SessionName;
};

UINTERFACE(MinimalApi)
class UDLSessionListService : public UInterface
{
    GENERATED_BODY()
};

/*
 * Allows to explore available sessions to connect
 */
class DILE_API IDLSessionListService
{
    GENERATED_BODY()

public:
    /* Last known session list */
    virtual const TArray<FDLSessionListEntry>& GetAvailableSessions() const = 0;

    /* Refreshes session list asynchronously */
    virtual FDLTask RefreshSessionList() = 0;
};
