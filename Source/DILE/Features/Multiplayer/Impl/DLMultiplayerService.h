// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Features/Multiplayer/IDLConnectionService.h"
#include "Features/Multiplayer/IDLHostingService.h"
#include "Features/Multiplayer/IDLSessionListService.h"

#include "DLMultiplayerService.generated.h"

UCLASS()
class DILE_API UDLMultiplayerServiceImpl : public UObject, public IDLConnectionService, public IDLHostingService, public IDLSessionListService
{
    GENERATED_BODY()

public:
    // Begin IDLConnectionService

    // End IDLConnectionService

    // Begin IDLHostingService

    // End IDLHostingService

    // Begin IDLSessionListService
    const TArray<FDLSessionListEntry>& GetAvailableSessions() const override { return AvailableSessions; }
    UE5Coro::TCoroutine<> RefreshSessionList() override;
    // End IDLSessionListService

private:
    TArray<FDLSessionListEntry> AvailableSessions;
};
