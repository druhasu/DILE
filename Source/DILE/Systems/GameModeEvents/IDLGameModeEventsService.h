// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "IDLGameModeEventsService.generated.h"

UINTERFACE(MinimalApi)
class UDLGameModeEventsService : public UInterface { GENERATED_BODY() };

/*
 * Provides access to GameMode events. Available only on server
 */
class DILE_API IDLGameModeEventsService
{
    GENERATED_BODY()

public:
    using FPreLoginSignature = TMulticastDelegate<void(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)>;
    using FPostLoginSignature = TMulticastDelegate<void(APlayerController* NewPlayer)>;
    using FLogoutSignature = TMulticastDelegate<void(AController* Exiting)>;

    /* Called during user PreLogin stage */
    virtual FPreLoginSignature& OnPreLogin() = 0;

    /* Called after user Login is completed */
    virtual FPostLoginSignature& OnPostLogin() = 0;

    /* Called after user Logout is completed */
    virtual FLogoutSignature& OnLogout() = 0;
};

