// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "Systems/ObjectTracking/DLTrackedObject.h"

#include "DLRespawnPoint.generated.h"

UCLASS()
class DILE_API ADLRespawnPoint : public APlayerStart, public FDLTrackedObject
{
    GENERATED_BODY()

public:
    void PostInitializeComponents() override;
    void EndPlay(EEndPlayReason::Type InReason) override;

    int32 GetAssignedPlayerIndex() const { return PlayerIndex; }

protected:
    UPROPERTY(EditInstanceOnly)
    int32 PlayerIndex = 0;
};
