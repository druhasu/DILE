// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/PlayerContext/IDLPlayerPawnService.h"

#include "DLPlayerPawnServiceMock.generated.h"

UCLASS()
class UDLPlayerPawnServiceMock : public UObject, public IDLPlayerPawnService
{
    GENERATED_BODY()

public:
    virtual FPawnChangedSignature& OnPawnChanged() override { return PawnChangedDelegate; }

    UPROPERTY()
    APawn* Pawn = nullptr;

    FPawnChangedSignature PawnChangedDelegate;

private:
    virtual APawn* GetPawnImpl() const { return Pawn; }
};
