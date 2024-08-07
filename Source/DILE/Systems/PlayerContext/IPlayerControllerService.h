// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "IPlayerControllerService.generated.h"

class APlayerController;

UINTERFACE(MinimalApi)
class UPlayerControllerService : public UInterface { GENERATED_BODY() };

/*
 * Provides access to PlayerController of current Player.
 */
class DILE_API IPlayerControllerService
{
	GENERATED_BODY()

public:
	/* Player state of current Player. Or null if not yet created/replicated */
	APlayerController* GetPlayerController() const { return GetPlayerControllerImpl(); }

private:
	/* Separate private method to avoid conflicts with existing method with the same name in implementing class */
	virtual APlayerController* GetPlayerControllerImpl() const = 0;
};
