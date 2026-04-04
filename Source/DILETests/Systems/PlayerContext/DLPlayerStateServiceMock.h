// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/PlayerContext/IDLPlayerStateService.h"

#include "DLPlayerStateServiceMock.generated.h"

UCLASS()
class UDLPlayerStateServiceMock : public UObject, public IDLPlayerStateService
{
    GENERATED_BODY()

public:
    UPROPERTY()
    APlayerState* PlayerState = nullptr;

private:
    virtual APlayerState* GetPlayerStateImpl() const { return PlayerState; }
};
