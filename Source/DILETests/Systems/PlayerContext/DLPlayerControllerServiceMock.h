// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/PlayerContext/IDLPlayerControllerService.h"

#include "DLPlayerControllerServiceMock.generated.h"

UCLASS()
class UDLPlayerControllerServiceMock : public UObject, public IDLPlayerControllerService
{
    GENERATED_BODY()

public:
    UPROPERTY()
    APlayerController* PlayerController = nullptr;

private:
    virtual APlayerController* GetPlayerControllerImpl() const { return PlayerController; };
};
