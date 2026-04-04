// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Systems/Timers/IDLTimerService.h"

#include "DLTimerServiceImpl.generated.h"

/**
 *
 */
UCLASS()
class DILE_API UDLTimerServiceImpl : public UObject, public IDLTimerService
{
    GENERATED_BODY()

public:
    void InitDependencies();

    // Begin IDLTimerService
    void SetTimer(FTimerHandle& InOutHandle, FTimerUnifiedDelegate&& InDelegate, float InRate, bool InbLoop, float InFirstDelay) override;
    FTimerHandle SetTimerForNextTick(FTimerUnifiedDelegate&& InDelegate) override;
    void ClearTimer(FTimerHandle& InHandle) override;
    void ClearAllTimersForObject(const void* Object) override;
    bool IsTimerActive(FTimerHandle InHandle) const override;
    void PauseTimer(FTimerHandle InHandle) override;
    void UnPauseTimer(FTimerHandle InHandle) override;
    bool IsTimerPaused(FTimerHandle InHandle) const override;
    float GetTimerElapsed(FTimerHandle InHandle) const override;
    // End IDLTimerService

private:
    FTimerManager* TimerManager;
};
