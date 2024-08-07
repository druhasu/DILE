// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "IPlayerStateService.generated.h"

class APlayerState;

UINTERFACE(MinimalApi)
class UPlayerStateService: public UInterface { GENERATED_BODY() };

/*
 * Provides access to PlayerState of current Player.
 */
class DILE_API IPlayerStateService
{
	GENERATED_BODY()

public:
	using FPlayerStateChangedSignature = TMulticastDelegate<void(APlayerState* NewPlayerState)>;

	/* Player state of current Player. Or null if not yet created/replicated */
	APlayerState* GetPlayerState() const { return GetPlayerStateImpl(); }

	/* Called when PlayerState is created or replicated */
	virtual FPlayerStateChangedSignature& OnPlayerStateChanged() = 0;

private:
	/* Separate private method to avoid conflicts with existing method with the same name in implementing class */
	virtual APlayerState* GetPlayerStateImpl() const = 0;
};
