// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "TimerManager.h"

#include "IDLTimerService.generated.h"

UINTERFACE(MinimalApi)
class UDLTimerService : public UInterface { GENERATED_BODY() };

/**
 * Interface for service that provides timers
 */
class DILE_API IDLTimerService
{
    GENERATED_BODY()

public:
    /**
     * Sets a timer to call the given native function at a set interval.  If a timer is already set
     * for this handle, it will replace the current timer.
     *
     * @param InOutHandle			If the passed-in handle refers to an existing timer, it will be cleared before the new timer is added. A new handle to the new timer is returned in either case.
     * @param InObj					Object to call the timer function on.
     * @param InTimerMethod			Method to call when timer fires.
     * @param InRate				The amount of time (in seconds) between set and firing.  If <= 0.f, clears existing timers.
     * @param InbLoop				true to keep firing at Rate intervals, false to fire only once.
     * @param InFirstDelay			The time (in seconds) for the first iteration of a looping timer. If < 0.f InRate will be used.
     */
    template<class UserClass>
    FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, UserClass* InObj, typename FTimerDelegate::TMethodPtr<UserClass> InTimerMethod, float InRate, bool InbLoop = false, float InFirstDelay = -1.f)
    {
        SetTimer(InOutHandle, FTimerUnifiedDelegate(FTimerDelegate::CreateUObject(InObj, InTimerMethod)), InRate, InbLoop, InFirstDelay);
    }

    template<class UserClass>
    FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, UserClass* InObj, typename FTimerDelegate::TConstMethodPtr<UserClass> InTimerMethod, float InRate, bool InbLoop = false, float InFirstDelay = -1.f)
    {
        SetTimer(InOutHandle, FTimerUnifiedDelegate(FTimerDelegate::CreateUObject(InObj, InTimerMethod)), InRate, InbLoop, InFirstDelay);
    }

    /** Version that takes any generic delegate. */
    FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, FTimerDelegate const& InDelegate, float InRate, bool InbLoop, float InFirstDelay = -1.f)
    {
        SetTimer(InOutHandle, FTimerUnifiedDelegate(InDelegate), InRate, InbLoop, InFirstDelay);
    }

    /** Version that takes a dynamic delegate (e.g. for UFunctions). */
    FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, FTimerDynamicDelegate const& InDynDelegate, float InRate, bool InbLoop, float InFirstDelay = -1.f)
    {
        SetTimer(InOutHandle, FTimerUnifiedDelegate(InDynDelegate), InRate, InbLoop, InFirstDelay);
    }

    /** Version that doesn't take a delegate */
    FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, float InRate, bool InbLoop, float InFirstDelay = -1.f)
    {
        SetTimer(InOutHandle, FTimerUnifiedDelegate(), InRate, InbLoop, InFirstDelay);
    }

    /** Version that takes a TFunction */
    FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, TFunction<void(void)>&& Callback, float InRate, bool InbLoop, float InFirstDelay = -1.f)
    {
        SetTimer(InOutHandle, FTimerUnifiedDelegate(MoveTemp(Callback)), InRate, InbLoop, InFirstDelay);
    }

    /** Version that takes FTimerUnifiedDelegate directly */
    virtual void SetTimer(FTimerHandle& InOutHandle, FTimerUnifiedDelegate&& InDelegate, float InRate, bool InbLoop, float InFirstDelay) = 0;

    /**
     * Sets a timer to call the given native function on the next tick.
     *
     * @param inObj					Object to call the timer function on.
     * @param inTimerMethod			Method to call when timer fires.
     */
    template<class UserClass>
    FORCEINLINE FTimerHandle SetTimerForNextTick(UserClass* inObj, typename FTimerDelegate::TMethodPtr<UserClass> inTimerMethod)
    {
        return SetTimerForNextTick(FTimerUnifiedDelegate(FTimerDelegate::CreateUObject(inObj, inTimerMethod)));
    }

    template<class UserClass>
    FORCEINLINE FTimerHandle SetTimerForNextTick(UserClass* inObj, typename FTimerDelegate::TConstMethodPtr<UserClass> inTimerMethod)
    {
        return SetTimerForNextTick(FTimerUnifiedDelegate(FTimerDelegate::CreateUObject(inObj, inTimerMethod)));
    }

    /** Version that takes any generic delegate. */
    FORCEINLINE FTimerHandle SetTimerForNextTick(FTimerDelegate const& InDelegate)
    {
        return SetTimerForNextTick(FTimerUnifiedDelegate(InDelegate));
    }

    /** Version that takes a dynamic delegate (e.g. for UFunctions). */
    FORCEINLINE FTimerHandle SetTimerForNextTick(FTimerDynamicDelegate const& InDynDelegate)
    {
        return SetTimerForNextTick(FTimerUnifiedDelegate(InDynDelegate));
    }

    /** Version that takes a TFunction */
    FORCEINLINE FTimerHandle SetTimerForNextTick(TFunction<void(void)>&& Callback)
    {
        return SetTimerForNextTick(FTimerUnifiedDelegate(MoveTemp(Callback)));
    }

    /** Version that takes FTimerUnifiedDelegate directly */
    virtual FTimerHandle SetTimerForNextTick(FTimerUnifiedDelegate&& InDelegate) = 0;

    /**
     * Clears a previously set timer, identical to calling SetTimer() with a <= 0.f rate.
     * Invalidates the timer handle as it should no longer be used.
     *
     * @param InHandle The handle of the timer to clear.
     */
    virtual void ClearTimer(FTimerHandle& InHandle) = 0;

    /** Clears all timers that are bound to functions on the given object. */
    virtual void ClearAllTimersForObject(void const* Object) = 0;

    /**
     * Returns true if the specified timer exists and is not paused
     *
     * @param InHandle The handle of the timer to check for being active.
     * @return true if the timer exists and is active, false otherwise.
     */
    virtual bool IsTimerActive(FTimerHandle InHandle) const = 0;

    /**
     * Pauses a previously set timer.
     *
     * @param InHandle The handle of the timer to pause.
     */
    virtual void PauseTimer(FTimerHandle InHandle) = 0;

    /**
     * Unpauses a previously set timer
     *
     * @param InHandle The handle of the timer to unpause.
     */
    virtual void UnPauseTimer(FTimerHandle InHandle) = 0;

    /**
     * Returns true if the specified timer exists and is paused
     *
     * @param InHandle The handle of the timer to check for being paused.
     * @return true if the timer exists and is paused, false otherwise.
     */
    virtual bool IsTimerPaused(FTimerHandle InHandle) const = 0;

    /**
     * Gets the current elapsed time for the specified timer.
     *
     * @param InHandle The handle of the timer to check the elapsed time of.
     * @return The current time elapsed or -1.f if the timer does not exist.
     */
    virtual float GetTimerElapsed(FTimerHandle InHandle) const = 0;
};
