// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "IPlayerPawnService.generated.h"

class APawn;

UINTERFACE(MinimalApi)
class UPlayerPawnService : public UInterface { GENERATED_BODY() };

/*
 * Provides access to Pawn of current Player.
 */
class DILE_API IPlayerPawnService
{
	GENERATED_BODY()

public:
	using FPawnChangedSignature = TMulticastDelegate<void(APawn* NewPawn)>;

	/* Pawn of current Player. Or null if not yet created/replicated/possessed */
	APawn* GetPawn() const { return GetPawnImpl(); }

	/* Called when Pawn is created/replicated/possessed */
	virtual FPawnChangedSignature& OnPawnChanged() = 0;

private:
	/* Separate private method to avoid conflicts with existing method with the same name in implementing class */
	virtual APawn* GetPawnImpl() const = 0;
};
