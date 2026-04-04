// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLTimerServiceImpl.h"

void UDLTimerServiceImpl::InitDependencies()
{
    TimerManager = &GetWorld()->GetTimerManager();
    ensureAlways(TimerManager);
}

void UDLTimerServiceImpl::SetTimer(FTimerHandle& InOutHandle, FTimerUnifiedDelegate&& InDelegate, float InRate, bool InbLoop, float InFirstDelay)
{
    if (FTimerFunction* TimerFunction = InDelegate.VariantDelegate.TryGet<FTimerFunction>())
    {
        TimerManager->SetTimer(InOutHandle, MoveTemp(*TimerFunction), InRate, InbLoop, InFirstDelay);
        return;
    }
    if (FTimerDelegate* TimerDelegate = InDelegate.VariantDelegate.TryGet<FTimerDelegate>())
    {
        TimerManager->SetTimer(InOutHandle, *TimerDelegate, InRate, InbLoop, InFirstDelay);
        return;
    }
    if (FTimerDynamicDelegate* TimerDynamicDelegate = InDelegate.VariantDelegate.TryGet<FTimerDynamicDelegate>())
    {
        TimerManager->SetTimer(InOutHandle, *TimerDynamicDelegate, InRate, InbLoop, InFirstDelay);
        return;
    }
    ensure(!"No bound delegates found!");
}

FTimerHandle UDLTimerServiceImpl::SetTimerForNextTick(FTimerUnifiedDelegate&& InDelegate)
{
    if (FTimerFunction* TimerFunction = InDelegate.VariantDelegate.TryGet<FTimerFunction>())
    {
        return TimerManager->SetTimerForNextTick(MoveTemp(*TimerFunction));
    }
    if (FTimerDelegate* TimerDelegate = InDelegate.VariantDelegate.TryGet<FTimerDelegate>())
    {
        return TimerManager->SetTimerForNextTick(*TimerDelegate);
    }
    if (FTimerDynamicDelegate* TimerDynamicDelegate = InDelegate.VariantDelegate.TryGet<FTimerDynamicDelegate>())
    {
        return TimerManager->SetTimerForNextTick(*TimerDynamicDelegate);
    }
    ensure(!"No bound delegates found!");
    return FTimerHandle();
}

void UDLTimerServiceImpl::ClearTimer(FTimerHandle& InHandle)
{
    TimerManager->ClearTimer(InHandle);
}

void UDLTimerServiceImpl::ClearAllTimersForObject(const void* Object)
{
    TimerManager->ClearAllTimersForObject(Object);
}

bool UDLTimerServiceImpl::IsTimerActive(FTimerHandle InHandle) const
{
    return TimerManager->IsTimerActive(InHandle);
}

void UDLTimerServiceImpl::PauseTimer(FTimerHandle InHandle)
{
    TimerManager->PauseTimer(InHandle);
}

void UDLTimerServiceImpl::UnPauseTimer(FTimerHandle InHandle)
{
    TimerManager->UnPauseTimer(InHandle);
}

bool UDLTimerServiceImpl::IsTimerPaused(FTimerHandle InHandle) const
{
    return TimerManager->IsTimerPaused(InHandle);
}

float UDLTimerServiceImpl::GetTimerElapsed(FTimerHandle InHandle) const
{
    return TimerManager->GetTimerElapsed(InHandle);
}
